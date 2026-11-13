#ifndef G_LOWER_H
#define G_LOWER_H
#include "ir/ct_vec.h"
#include "lc_vec.h"
typedef struct IR_t IR_t;
typedef struct { const char * name; IR_t * landing; } bb_label_entry_t;
typedef struct { cv_t v, ix; } gname_set_t;
typedef struct { const IR_t ** nd; const char ** src; int * line; int n; int max; } lc_bb_src_t;
typedef struct { const IR_t * k; int v; } bb_src_slot_t;
typedef struct { bb_src_slot_t * s; int cap; int n; long runs; } lc_bb_src_ix_t;
typedef struct {
    lc_vec bb_labels;
    gname_set_t gnames;
    gname_set_t icn_gnames;
    gname_set_t rbi_names;
    const char ** global_names;
    int global_count;
    lc_bb_src_t bb_src;
    lc_bb_src_ix_t bb_src_ix;
    int bb_src_of_sd;
    int bb_src_reset_sr;
} g_lower_lc_t;
typedef struct { g_lower_lc_t lc; } g_lower_t;
#define G_LOWER_INIT { .lc = { .bb_labels = { NULL, 0, 0, (int) sizeof(bb_label_entry_t) }, .bb_src_ix = { 0, 0, -1, -1 }, . bb_src_of_sd = -1, .bb_src_reset_sr = -1 } }
extern g_lower_t g_lower;
#endif
