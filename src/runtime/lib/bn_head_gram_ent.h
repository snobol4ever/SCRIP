#ifndef BN_HEAD_GRAM_ENT_H
#define BN_HEAD_GRAM_ENT_H
#include <glob.h>
#include "runtime/core/g_runtime_bn.h"
#include "ir/ct_vec.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *qname; const char *body; int flavor; } gram_ent_t;
#define GRAM(k) CV_AT(g_rt_bn.gram, gram_ent_t, (k))
#ifdef __cplusplus
}
#endif
#endif
