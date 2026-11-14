#ifndef BN_HEAD_PL_PSCHED_S_H
#define BN_HEAD_PL_PSCHED_S_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct pl_psched_s pl_psched_t;
struct pl_psched_s { uint64_t entry_rsp, ret_pc, rbp, rbx, r12, r13, r14, r15; uint64_t planes[4]; int64_t k_level; int64_t leader; };
#ifdef __cplusplus
}
#endif
#endif
