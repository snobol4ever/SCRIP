#ifndef EVAL_BLOCKS_H
#define EVAL_BLOCKS_H
#include "core/core.h"
typedef DESCR_t (*eval_chain_fn)(void *zeta, int entry);
typedef struct g_eval_cache_ent { char *key; eval_chain_fn fn; long gen; } eval_cache_ent_t;
typedef struct g_lbl_ent { char *key; eval_chain_fn fn; } lbl_ent_t;
#endif
