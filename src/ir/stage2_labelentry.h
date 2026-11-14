#ifndef STAGE2_LABELENTRY_H
#define STAGE2_LABELENTRY_H
#include <stdint.h>
#include "ir/ast.h"
typedef struct LabelEntry { const char *name; const tree_t *stmt; } LabelEntry;
#endif
