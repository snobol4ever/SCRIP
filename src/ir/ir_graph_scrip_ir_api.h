#ifndef IR_GRAPH_SCRIP_IR_API_H
#define IR_GRAPH_SCRIP_IR_API_H
#include <stdio.h>
#include <stdint.h>
#include "ir/ir_fwd.h"
#include "ir/ir_graph.h"
#ifdef __cplusplus
extern "C" {
#endif
int ir_graph_has_suspend(const IR_graph_t * g);
IR_graph_t * IR_alloc(int max_nodes);
uint32_t ir_name_hash(const char * s);
int ir_varslot_of(const IR_graph_t * g, const char * name);
int graph_has_local(const IR_graph_t * g, const char * name);
void IR_free(IR_graph_t * cfg);
void bb_print(const IR_graph_t * cfg, FILE * fp);
void bb_print_v(const IR_graph_t * cfg, FILE * fp, int verbose);
#ifdef __cplusplus
}
#endif
#endif
