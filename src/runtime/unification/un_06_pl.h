#ifndef RUNTIME_UNIFICATION_UN_06_PL_H
#define RUNTIME_UNIFICATION_UN_06_PL_H
#include "runtime/unification/un_head.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) pl_cell_t plc_nil_cell(void);
__attribute__((visibility("hidden"))) pl_cell_t plc_cons(int fid, pl_cell_t head, pl_cell_t tail);
__attribute__((visibility("hidden"))) pl_trie_t * pl_trie_at(void *root, int64_t id);
__attribute__((visibility("hidden"))) int pl_trie_give(pl_cell_t src, void *out_cell, pl_tr_ctx_t *cx);
#endif
