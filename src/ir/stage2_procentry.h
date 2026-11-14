#ifndef STAGE2_PROCENTRY_H
#define STAGE2_PROCENTRY_H
#include <stdint.h>
#include "ir/ast.h"
#include "ir/stage2_scope.h"
#include "emitter/emit.h"
typedef struct ProcEntry {
    const char *name;
    tree_t *proc;
    int entry_pc;
    int nparams;
    int nformals;
    Scope lower_sc;
    int bb_idx;
    int is_generator;
    int is_variadic;
    int rest_kind;
    int named_rest;
    int dyn_scope;
    int lex_startup;
    const char *result_name;
    uint64_t byref_mask;
    int decl_level;
    IR_t * proc_entry_node;
    int thunk_kind;
    unsigned thunk_flags;
    const char *stage_var;
    int port_exit;
    int origin;
} ProcEntry;
#endif
