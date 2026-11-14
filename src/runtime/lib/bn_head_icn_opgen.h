#ifndef BN_HEAD_ICN_OPGEN_H
#define BN_HEAD_ICN_OPGEN_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t magic; const char *nm; int64_t idx; int kind; int64_t cur, lim, step; } ICN_OPGEN_t;
#ifdef __cplusplus
}
#endif
#endif
