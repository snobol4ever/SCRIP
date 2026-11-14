#ifndef STAGE2_LOWER_SEG_H
#define STAGE2_LOWER_SEG_H
#include <stdint.h>
#include "ir/ast.h"
#include "ir/stage2_lower_entry_fn.h"
typedef struct { const tree_t *prog; lower_entry_fn fn; } lower_seg_t;
#endif
