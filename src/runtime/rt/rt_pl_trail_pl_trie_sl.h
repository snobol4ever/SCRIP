#ifndef RT_PL_TRAIL_PL_TRIE_SL_H
#define RT_PL_TRAIL_PL_TRIE_SL_H
#include <stdint.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { int64_t depth, low, next, done, uncond, sched; DESCR_t pcons, ncons; } pl_trie_sl_t;
#ifdef __cplusplus
}
#endif
#endif
