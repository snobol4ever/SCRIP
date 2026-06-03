#ifndef STAGE2_H
#define STAGE2_H
#include <stdint.h>
#include "SM.h"
#include "bb_program.h"
#include "ast.h"
#define STAGE2_LABEL_MAX           4096
#define STAGE2_PROC_TABLE_MAX       256
#define STAGE2_PL_PRED_TABLE_SIZE   256
#define STAGE2_MOD_MAX               64
#define STAGE2_FRAME_SLOT_MAX        64
typedef struct LabelEntry {
    const char    *name;
    const tree_t  *stmt;
} LabelEntry;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct ScopeEnt { const char *name; int slot; } ScopeEnt;
typedef struct Scope    { ScopeEnt e[STAGE2_FRAME_SLOT_MAX]; int n; } Scope;
struct IR_graph_t;
typedef struct ProcEntry {
    const char         *name;
    tree_t             *proc;
    int                 entry_pc;
    int                 nparams;
    Scope            lower_sc;
    int                 bb_idx;
    int                 is_generator;
    uint64_t            byref_mask;
} ProcEntry;
/*--------------------------------------------------------------------------------------------------------------------*/
struct Resolve_PredEntry_t;
typedef struct Resolve_PredTable {
    struct Resolve_PredEntry_t *buckets[STAGE2_PL_PRED_TABLE_SIZE];
} Resolve_PredTable;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct ScripModule {
    int           lang;
    const char   *name;
    const tree_t *first;
    const tree_t *last;
    int           nstmts;
    int           core_label_start;
    int           core_label_count;
    int           proc_start;
    int           nprocs;
} ScripModule;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct ScripModuleRegistry {
    ScripModule mods[STAGE2_MOD_MAX];
    int         nmod;
    int         main_mod;
} ScripModuleRegistry;
/*--------------------------------------------------------------------------------------------------------------------*/
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
    int                  lang;
} stage2_t;
/*--------------------------------------------------------------------------------------------------------------------*/
extern stage2_t g_stage2;
void stage2_reset(void);
int  stage2_label_grow(stage2_t *s2);
int  stage2_proc_grow (stage2_t *s2);
#endif
