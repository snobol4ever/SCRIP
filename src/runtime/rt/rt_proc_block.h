#ifndef RT_PROC_BLOCK_H
#define RT_PROC_BLOCK_H
#include <stdint.h>
#include "descr.h"
#include "bb_box.h"
typedef struct rt_proc_s {
    const char *name;
    bb_box_fn fn;
    const char **pnames;
    int nparams;
    int frame_nslots;
    int decl_level;
    int alpha_slot;
    uint64_t byref_mask;
    int frame_bytes;
    int gen_region_ft;
    DESCR_t **pcells;
    DESCR_t *rcell;
    int cells_done;
    int is_generator;
    int dyn_scope;
    int stage_var;
    const char *result_name;
    int is_variadic;
    int rest_kind;
    int named_rest;
    int jmp_entry;
    unsigned int redefined : 15;
    unsigned int thunk : 1;
    unsigned int pinned : 16;
    int zstatic;
    int pnames_owned;
    int nformals;
} rt_proc_t;
#endif
