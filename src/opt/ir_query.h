/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef IR_QUERY_H
#define IR_QUERY_H
#include "IR.h"
int scan_pat_is_single_lit(IR_graph_t *pg);
int scan_val_is_single_lit(IR_graph_t *g);
int scan_pat_m3_native_safe(IR_graph_t *pg);
#endif
