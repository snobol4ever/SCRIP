#ifndef STAGE2_RESOLVE_PREDTABLE_H
#define STAGE2_RESOLVE_PREDTABLE_H
#include <stdint.h>
#include "ir/stage2_label_max.h"
#include "ir/stage2_resolve_predentry.h"
struct Resolve_PredEntry_t;
typedef struct Resolve_PredTable { struct Resolve_PredEntry_t *buckets[STAGE2_PL_PRED_TABLE_SIZE]; } Resolve_PredTable;
#endif
