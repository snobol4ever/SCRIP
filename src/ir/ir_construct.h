#ifndef SCRIP_IR_CONSTRUCT_H
#define SCRIP_IR_CONSTRUCT_H
#include "ir/ir_fwd.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { IR_CK_GRAPH = 1, IR_CK_CLAUSE, IR_CK_BOUND, IR_CK_CUT, IR_CK_COUNT } IR_construct_e;
typedef struct IR_construct_t { int kind; IR_t * begin; IR_t * end; int lo; int hi; } IR_construct_t;
int ir_construct_add(IR_graph_t * g, int kind, IR_t * begin, IR_t * end, int lo, int hi);
const IR_construct_t * ir_construct_find(const IR_graph_t * g, int kind, const IR_t * begin);
int ir_clause_bounds(const IR_graph_t * g, int * lo, int na);
const char * ir_construct_name(int kind);
#ifdef __cplusplus
}
#endif
#endif
