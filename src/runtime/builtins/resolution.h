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
#define RESOLVE_SCOPE_SLOT_MAX       64
#define RESOLVE_BB_TABLE_MAX       256
typedef struct Resolve_PredEntry_t {
    const char *key; tree_t *choice; struct Resolve_PredEntry_t *next;
    int entry_pc;
} Resolve_PredEntry;
typedef struct { const char *name; int slot; } PlScopeEnt;
typedef struct { PlScopeEnt e[RESOLVE_SCOPE_SLOT_MAX]; int n; } PlScope;
typedef struct { const char *name; int arity; int bb_idx; PlScope lower_sc; } Resolve_PredEntry_BB;
extern Resolve_PredEntry_BB g_resolve_bb_table[RESOLVE_BB_TABLE_MAX];
extern int             g_resolve_bb_count;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_graph_t *bb_graph_of_pred(const Resolve_PredEntry_BB *e)
{
    if (!e) return NULL;
    if (e->bb_idx >= 0 && e->bb_idx < g_stage2.bbp.count)
        return g_stage2.bbp.table[e->bb_idx];
    return NULL;
}
extern int           g_resolve_cut_flag;
extern int           g_resolve_active;
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
void    resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup_global(const char *key);
Resolve_PredEntry *resolve_pred_entry_lookup(const char *key);
Resolve_PredEntry_BB *resolve_bb_lookup(const char *name, int arity);
int         resolve_bb_pred_count(void);
const char *resolve_bb_pred_name_at(int idx);
int         resolve_bb_pred_arity_at(int idx);
IR_graph_t *resolve_bb_graph_at(int idx);
Resolve_PredEntry_BB *resolve_bb_register(const char *name, int arity, int bb_idx);
#endif
