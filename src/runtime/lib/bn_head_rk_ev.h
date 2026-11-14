#ifndef BN_HEAD_RK_EV_H
#define BN_HEAD_RK_EV_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define RK_EV(m, k) (((int *) (m)->blk) + 4 * (m)->ngroups + 3 * (m)->ncaplog + 10 * (k))
#ifdef __cplusplus
}
#endif
#endif
