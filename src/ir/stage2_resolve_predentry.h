#ifndef STAGE2_RESOLVE_PREDENTRY_H
#define STAGE2_RESOLVE_PREDENTRY_H
#include <stdint.h>
#include "ir/ast.h"
struct Resolve_PredEntry_t;
typedef struct Resolve_PredEntry_t { const char *key; tree_t *choice; struct Resolve_PredEntry_t *next; int entry_pc; const char *file; } Resolve_PredEntry;
#endif
