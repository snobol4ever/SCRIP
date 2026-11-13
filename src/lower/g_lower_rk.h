#ifndef G_LOWER_RK_H
#define G_LOWER_RK_H
#include "ir/ct_vec.h"
typedef struct {
    cv_t gram_names;
    cv_t class_names;
    cv_t multi_names;
    int user_write_meth;
    int listlike_overridden[10];
    int rk_case_desugar_cid;
    int lower_rv_forlist_ctr;
    int lower_rv_tern_n;
    int rk_hoist_anon_blocks_blk_ctr;
    int rk_gather_ctr;
    int eval_road;
} g_lower_rk_t;
#define G_LOWER_RK_INIT { 0 }
#ifdef __cplusplus
extern "C" {
#endif
extern g_lower_rk_t g_lower_rk;
#ifdef __cplusplus
}
#endif
#endif
