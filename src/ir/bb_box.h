#ifndef BB_BOX_H
#define BB_BOX_H
#include <stddef.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef DESCR_t (*bb_box_fn)(void * zeta, int entry);
typedef struct { bb_box_fn fn; void * ζ; size_t ζ_size; } bb_node_t;
static const int α = 0;
static const int β = 1;
extern const char * Σ;
extern int Δ;
extern int Ω;
extern int Σlen;
#ifdef __cplusplus
}
#endif
#endif
