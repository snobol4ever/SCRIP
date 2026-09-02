/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef STAGE2_H
#define STAGE2_H
#include <stdint.h>
#include "SM.h"
#include "bb_program.h"
#include "ast.h"
#define STAGE2_LABEL_MAX           4096
#define STAGE2_PROC_TABLE_MAX       256
#define STAGE2_PL_PRED_TABLE_SIZE   256
typedef struct Resolve_PredEntry_t { const char *key; tree_t *choice; struct Resolve_PredEntry_t *next; int entry_pc; } Resolve_PredEntry;
#define STAGE2_MOD_MAX               64
#define STAGE2_FRAME_SLOT_MAX        64
typedef struct LabelEntry {
    const char    *name;
    const tree_t  *stmt;
} LabelEntry;
typedef struct ScopeEnt { const char *name; int slot; } ScopeEnt;
typedef struct Scope    { ScopeEnt e[STAGE2_FRAME_SLOT_MAX]; int n; } Scope;
struct IR_graph_t;
typedef struct ProcEntry {
    const char         *name;
    tree_t             *proc;
    int                 entry_pc;
    int                 nparams;
    int                 nformals;
    Scope            lower_sc;
    int                 bb_idx;
    int                 is_generator;
    int                 is_variadic;
    int                 rest_kind;
    int                 named_rest;
    int                 dyn_scope;
    const char         *result_name;
    uint64_t            byref_mask;
    int                 decl_level;
    IR_t *              proc_entry_node;
} ProcEntry;
struct Resolve_PredEntry_t;
typedef struct Resolve_PredTable {
    struct Resolve_PredEntry_t *buckets[STAGE2_PL_PRED_TABLE_SIZE];
} Resolve_PredTable;
typedef struct ScripModule {
    const char   *name;
    const tree_t *first;
    const tree_t *last;
    int           nstmts;
    int           core_label_start;
    int           core_label_count;
    int           proc_start;
    int           nprocs;
} ScripModule;
typedef struct ScripModuleRegistry {
    ScripModule mods[STAGE2_MOD_MAX];
    int         nmod;
    int         main_mod;
} ScripModuleRegistry;
typedef struct stage2_t {
    bb_program_t         bbp;
    LabelEntry          *label_table;
    int                  label_count;
    int                  label_cap;
    ProcEntry        *proc_table;
    int                  proc_count;
    int                  proc_cap;
    Resolve_PredTable         resolve_pred_table;
    ScripModuleRegistry  module_registry;
    const char          *pl_dyn_name[64];
    int                  pl_dyn_arity[64];
    int                  pl_dyn_n;
} stage2_t;
typedef stage2_t *(*lower_entry_fn)(const tree_t *prog);
typedef struct { const tree_t *prog; lower_entry_fn fn; } lower_seg_t;
extern stage2_t g_stage2;
void stage2_reset(void);
void ir_delete_all(stage2_t *s2);
int  stage2_label_grow(stage2_t *s2);
int  stage2_proc_grow (stage2_t *s2);
void    resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
#endif
