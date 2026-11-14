#ifndef BN_HEAD_PLWOTFRAME_H
#define BN_HEAD_PLWOTFRAME_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char *buf; size_t sz; int saved_out; int idx; } PlWotFrame;
#ifdef __cplusplus
}
#endif
#endif
