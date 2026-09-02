#ifndef RESOLUTION_H
#define RESOLUTION_H
#include "ast.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include "../../parsers/prolog/prolog_driver.h"
#include "bb_box.h"
#include "IR.h"
#include "SM.h"
#include "stage2.h"
#define RESOLVE_PRED_TABLE_SIZE_FWD STAGE2_PL_PRED_TABLE_SIZE
typedef struct Resolve_PredEntry_t {
    const char *key; tree_t *choice; struct Resolve_PredEntry_t *next;
    int entry_pc;
} Resolve_PredEntry;
extern int           g_resolve_cut_flag;
extern int           g_resolve_active;
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
void    resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup_global(const char *key);
Resolve_PredEntry *resolve_pred_entry_lookup(const char *key);
#endif
