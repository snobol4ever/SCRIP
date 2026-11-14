#ifndef BB_PROGRAM_SCRIP_IR_API_H
#define BB_PROGRAM_SCRIP_IR_API_H
#include "ir/ir_fwd.h"
#include "ir/bb_program.h"
#ifdef __cplusplus
extern "C" {
#endif
int bb_program_add (bb_program_t *p, IR_graph_t *bbg);
void bb_program_free(bb_program_t *p);
#ifdef __cplusplus
}
#endif
#endif
