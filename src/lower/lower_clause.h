#ifndef LOWER_PL_H
#define LOWER_PL_H
#include "BB.h"
struct tree_t;
BB_graph_t *lower_pl_predicate(struct tree_t *choice);
BB_t *lower_pl_threaded(BB_graph_t *bbg, const struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
#endif
