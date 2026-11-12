#ifndef RUNTIME_UNIFICATION_UN_02_PL_H
#define RUNTIME_UNIFICATION_UN_02_PL_H
#include "runtime/unification/un_head.h"
__attribute__((visibility("hidden"))) int plc_unify_cells_cx(pl_cell_t *a, pl_cell_t *b, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_acyclic_walk(pl_cell_t *c);
__attribute__((visibility("hidden"))) long plc_list_cells(void *list_cell, int dot_id);
__attribute__((visibility("hidden"))) const char *rt_pl_cell_name(pl_cell_t *d);
__attribute__((visibility("hidden"))) int rt_pl_cell_compare(pl_cell_t *ca, pl_cell_t *cb);
__attribute__((visibility("hidden"))) int plc_var_occ(pl_cell_t *c);
__attribute__((visibility("hidden"))) void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n);
__attribute__((visibility("hidden"))) int pl_acyclic_walk(pl_cell_t *c);
#endif
