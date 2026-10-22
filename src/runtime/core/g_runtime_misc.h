#ifndef G_RUNTIME_MISC_H
#define G_RUNTIME_MISC_H
#include <stdint.h>
#include "descr.h"
typedef struct {
    const void *d;
    DESCR_t *base;
    int inited;
    int busy;
    int no_reserve;
    char lang[16];
    int i_src;
    int i_path;
    int i_lang;
    int i_linked;
    int i_tree;
    int i_inited;
    int i_errpos;
    int i_incl;
} sc_module_t;
struct _DATINST_tType;
struct tree_t;
typedef struct {
    int monitor_quiet_depth;
    int64_t kw_stcount;
    int64_t kw_anchor;
    int64_t kw_stlimit;
    int64_t kw_trace;
    int core_stack_floor_raised;
    int core_err_active;
    int kw_ctx;
    long icn_errnumber;
    const char *icn_errtext;
    DESCR_t icn_errvalue;
    int icn_err_valid;
    struct tree_t *root;
    unsigned long bb_rnd_seed;
    struct _DATINST_tType *lf_type;
    int sno_defer_pair_hwm;
    const char *dcap_base;
    DESCR_t (*eval_str_hook)(const char *s);
    DESCR_t (*user_call_hook)(const char *name, DESCR_t *args, int nargs, long *rq);
    sc_module_t sc_mod;
    int sn4_direct_v;
    unsigned long zsm_pre_node;
    unsigned long zsm_pre_rsp;
    int zdecl_mode;
    unsigned long zdecl_chk[4][256];
    unsigned long zdecl_bad[4][256];
    unsigned long zsm_ek_node[ZSM_N / 4];
    unsigned long zsm_ek_rbp[ZSM_N / 4];
    unsigned long zsm_ek_e[ZSM_N / 4][8];
    unsigned char zsm_ek_n[ZSM_N / 4];
    unsigned char zsm_ek_cut[ZSM_N / 4];
    unsigned long zdecl_unanchored;
} g_misc_t;
#endif
