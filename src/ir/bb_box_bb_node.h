#ifndef BB_BOX_BB_NODE_H
#define BB_BOX_BB_NODE_H
#include <stddef.h>
#include "ir/bb_box_fn.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { bb_box_fn fn; void * ζ; size_t ζ_size; } bb_node_t;
#ifdef __cplusplus
}
#endif
#endif
