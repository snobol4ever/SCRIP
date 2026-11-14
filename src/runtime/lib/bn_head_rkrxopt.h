#ifndef BN_HEAD_RKRXOPT_H
#define BN_HEAD_RKRXOPT_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { long mode, posv, xmin, xmax; const long *nth; int nnth; } RkRxOpt;
#ifdef __cplusplus
}
#endif
#endif
