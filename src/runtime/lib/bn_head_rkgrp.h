#ifndef BN_HEAD_RKGRP_H
#define BN_HEAD_RKGRP_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char kind; int idx; const char *nm; int nl; int rep; int start, end; } RkGrp;
#ifdef __cplusplus
}
#endif
#endif
