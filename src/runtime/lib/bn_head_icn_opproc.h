#ifndef BN_HEAD_ICN_OPPROC_H
#define BN_HEAD_ICN_OPPROC_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char opsym[6]; signed char ar; } icn_opproc_t;
#ifdef __cplusplus
}
#endif
#endif
