#ifndef BN_HEAD_PL_CTV_H
#define BN_HEAD_PL_CTV_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { void **addr; char **nm; int n; int cap; int *hs; int hmask; } pl_ctv_t;
#ifdef __cplusplus
}
#endif
#endif
