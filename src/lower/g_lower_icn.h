#ifndef G_LOWER_ICN_H
#define G_LOWER_ICN_H
#include "lower/lc_vec.h"
typedef struct tree_t tree_t;
typedef struct {
    lc_vec fill_pnames_ps;
    const tree_t * icn_progname_as_global_done_prog;
    int icn_progname_as_global_used;
    lc_vec lower_icon_proc_pnv;
    lc_vec lower_icon_proc_lnv;
    lc_vec lower_icon_proc_gnv;
    lc_vec lower_icon_stage2_excl;
    lc_vec lower_icon_stage2_impl;
    lc_vec synth_excl;
    lc_vec synth_locals;
    int pm_hidden_n;
    lc_vec reassigned;
    const tree_t * reassigned_prog;
    lc_vec breassigned;
    const tree_t * breassigned_prog;
    void * (*parse)(const char *, const char *);
    const char * icn_record_names[512];
    int icn_record_name_count;
} g_lower_icn_t;
#define G_LOWER_ICN_INIT { 0 }
#ifdef __cplusplus
extern "C" {
#endif
extern g_lower_icn_t g_lower_icn;
#ifdef __cplusplus
}
#endif
#endif
