#ifndef CORE_EVAL_FRAME_H
#define CORE_EVAL_FRAME_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { DESCR_t saved; DESCR_t key; DESCR_t fn; DESCR_t mark; DESCR_t built; DESCR_t esv; DESCR_t flags; } eval_frame_t;
#ifdef __cplusplus
}
#endif
#endif
