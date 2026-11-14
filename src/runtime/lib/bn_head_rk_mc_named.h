#ifndef BN_HEAD_RK_MC_NAMED_H
#define BN_HEAD_RK_MC_NAMED_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *name; int nlen; int req; } rk_mc_named_t;
#ifdef __cplusplus
}
#endif
#endif
