#ifndef BN_HEAD_PL_META_ROW_H
#define BN_HEAD_PL_META_ROW_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *nm; int ar; int tok; } pl_meta_row_t;
#ifdef __cplusplus
}
#endif
#endif
