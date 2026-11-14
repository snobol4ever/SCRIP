#ifndef BN_HEAD_RK_ADV_H
#define BN_HEAD_RK_ADV_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { int dh, dv, eh, ev, kvh, kvv, ph, pv, kh, kv2, vh, vv, nmode; } rk_adv_t;
#ifdef __cplusplus
}
#endif
#endif
