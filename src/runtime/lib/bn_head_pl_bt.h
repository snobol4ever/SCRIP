#ifndef BN_HEAD_PL_BT_H
#define BN_HEAD_PL_BT_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char **nm; int n, cap; } pl_bt_t;
#ifdef __cplusplus
}
#endif
#endif
