/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef IR_QUERY_H
#define IR_QUERY_H
#include "IR.h"
#include "box_state.h"
int scan_pat_is_single_lit(IR_graph_t *pg);
int scan_val_is_single_lit(IR_graph_t *g);
int scan_pat_m3_native_safe(IR_graph_t *pg);
int gz_node_bounded(const IR_t *g);
IR_t *descr_chain_terminal(IR_t *entry);
int ir_is_generator_kind(IR_e t);
int subchain_node_is_generator(IR_t *nd);
int resolve_ite_entries_em(const IR_t *nd, IR_t **out_cond, IR_t **out_then, IR_t **out_else);
#endif
