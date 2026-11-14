#ifndef BN_HEAD_PLU_CTX_H
#define BN_HEAD_PLU_CTX_H
#include <glob.h>
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { pl_tr_ctx_t *cx; const char *floor_; } plu_ctx_t;
#ifdef __cplusplus
}
#endif
#endif
