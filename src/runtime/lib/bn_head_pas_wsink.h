#ifndef BN_HEAD_PAS_WSINK_H
#define BN_HEAD_PAS_WSINK_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct pas_wsink_t { FILE *fp; char *b; size_t n; size_t cap; } pas_wsink_t;
#ifdef __cplusplus
}
#endif
#endif
