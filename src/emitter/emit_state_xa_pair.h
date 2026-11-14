#ifndef EMIT_STATE_XA_PAIR_H
#define EMIT_STATE_XA_PAIR_H
#include <stdint.h>
#include "ir/ct_vec.h"
#include "emitter/bb_label.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { struct bb_label_t * define; struct bb_label_t * jmp; int rsp; } xa_pair_t;
#define XA_PAIR(i) CV_AT(g_emit.xa_bb_emit_pair, xa_pair_t, (i))
#ifdef __cplusplus
}
#endif
#include "emitter/emit_state_em_03_sh_api.h"
#endif
