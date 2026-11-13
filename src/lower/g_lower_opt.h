#ifndef G_LOWER_OPT_H
#define G_LOWER_OPT_H
#include <stdint.h>
typedef struct { const char * name; int k1; } gva_hx_t;
typedef struct {
    int bc_mon_m;
    int dg_mon_m;
    int cf_spine_on_s;
    const char ** gva_names;
    int gva_n;
    int gva_max;
    gva_hx_t * gva_hx;
    uint32_t gva_hx_cap;
    int gva_hx_n;
    int gva_lo;
    const char ** gva_io_refused;
    int gva_io_refused_n;
    int gva_io_refused_max;
    const char ** gva_kw_refused;
    int gva_kw_refused_n;
    int gva_kw_refused_max;
    int gva_kw_seeded;
    int gva_sn4_family;
    int gva_trace_demote;
    const char ** proc_slot_names;
    int proc_slot_n;
    int proc_slot_max;
    int scc_taint;
    int scc_taint_inherited;
} g_lower_opt_t;
#define G_LOWER_OPT_INIT { .bc_mon_m = -1, .dg_mon_m = -1, .cf_spine_on_s = -1 }
#ifdef __cplusplus
extern "C" {
#endif
extern g_lower_opt_t g_lower_opt;
#ifdef __cplusplus
}
#endif
#endif
