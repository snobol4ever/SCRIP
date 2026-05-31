#ifndef LOWER_PL_H
#define LOWER_PL_H
#include "BB.h"
struct tree_t;
IR_graph_t *lower_pl_predicate(struct tree_t *choice);
IR_t *lower_pl_threaded(IR_graph_t *bbg, const struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
#endif
