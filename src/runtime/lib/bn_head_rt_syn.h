#ifndef BN_HEAD_RT_SYN_H
#define BN_HEAD_RT_SYN_H
#include <glob.h>
#include "runtime/core/g_runtime_bn.h"
#include "ir/ct_vec.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *newname; const char *oldname; } rt_syn_t;
#define SYN(k) CV_AT(g_rt_bn.rt_syn, rt_syn_t, (k))
#ifdef __cplusplus
}
#endif
#endif
