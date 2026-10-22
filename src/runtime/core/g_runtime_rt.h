#ifndef G_RUNTIME_RT_H
#define G_RUNTIME_RT_H
#include <stddef.h>
#include "g_runtime_vec.h"
#include "ct_vec.h"
#define DCR_CELL_CACHE_SIZE 2048
struct g_dcr_cell { const char *name; struct DESCR_t *cell; int valid; };
typedef struct {
    const char *subject_dbg_base;
    long subject_dbg_len;
    int rt_gen_proc_count;
    int rt_gen_proc_cap;
    int *proc_hsl;
    unsigned proc_hcap;
    size_t gva_mapped;
    cv_t proc_loc;
    gv_t blob_fbv;
    cv_t initial_fired;
    int proc_idx_slot[DCR_CELL_CACHE_SIZE];
    const char *proc_idx_key[DCR_CELL_CACHE_SIZE];
    cv_t rt_dc_chunks;
    struct g_dcr_cell cell_cache[DCR_CELL_CACHE_SIZE];
} g_rt_t;
#endif
