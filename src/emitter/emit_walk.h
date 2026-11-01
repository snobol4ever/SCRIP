#ifndef EMITTER_EMIT_WALK_H
#define EMITTER_EMIT_WALK_H
#include <stdint.h>
struct bb_label_t;
#ifdef __cplusplus
extern "C" {
#endif
void * bb_ab_cell_addr (const char * fname);
const char * emit_intern_str(const char * s);
extern int g_flat_node_id;
typedef struct { int tag; int64_t val; struct bb_label_t * to; } pl_ix_arm_t;
#ifdef __cplusplus
}
#endif
#endif
