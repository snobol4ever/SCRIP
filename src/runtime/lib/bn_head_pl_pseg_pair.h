#ifndef BN_HEAD_PL_PSEG_PAIR_H
#define BN_HEAD_PL_PSEG_PAIR_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { void *blk; void *sched; } pl_pseg_pair_t;
#ifdef __cplusplus
}
#endif
#endif
