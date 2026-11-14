#ifndef STAGE2_H
#define STAGE2_H
#include <stdint.h>
#include "ir/bb_program.h"
#include "ir/stage2_labelentry.h"
#include "ir/stage2_procentry.h"
#include "ir/stage2_resolve_predtable.h"
#include "ir/stage2_scripmoduleregistry.h"
typedef struct stage2_t {
    bb_program_t bbp;
    LabelEntry *label_table;
    int label_count;
    int label_cap;
    ProcEntry *proc_table;
    int proc_count;
    int proc_cap;
    int *proc_ix;
    unsigned proc_ix_mask;
    int proc_ix_n;
    int proc_ix_nul;
    ProcEntry *proc_ix_tbl;
    Resolve_PredTable resolve_pred_table;
    ScripModuleRegistry module_registry;
    void * rt_root;
    int rt_lowering;
    int depth_counted;
    int rk_infix_armed;
    int ball_exit_by_code;
    int harness_warm;
} stage2_t;
#endif
