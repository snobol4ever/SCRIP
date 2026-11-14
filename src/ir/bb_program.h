#ifndef SCRIP_BB_PROGRAM_H
#define SCRIP_BB_PROGRAM_H
#include "ir/ir_fwd.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct bb_program_t { IR_graph_t **table; int count; int cap; int serial_next; } bb_program_t;
#ifdef __cplusplus
}
#endif
#endif
