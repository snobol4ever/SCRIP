#ifndef PATTERN_BLOCKS_H
#define PATTERN_BLOCKS_H
#include <stdint.h>
#include "core/core.h"
typedef struct dtp_rcp { int tt; const char *s; uint32_t slen; int64_t ival; struct dtp_rcp *l; struct dtp_rcp *r; } dtp_rcp_t;
typedef struct DTP { void *fn; dtp_rcp_t *rcp; int64_t zsz; int32_t zstatic; int32_t zpad; DESCR_t *snap; int64_t nsnap; } DTP_t;
#endif
