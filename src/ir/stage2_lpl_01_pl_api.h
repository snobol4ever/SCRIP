#ifndef STAGE2_LPL_01_PL_API_H
#define STAGE2_LPL_01_PL_API_H
#include <stdint.h>
#include "ir/ast.h"
#include "ir/stage2_resolve_predentry.h"
#include "ir/stage2_resolve_predtable.h"
Resolve_PredEntry *resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
#endif
