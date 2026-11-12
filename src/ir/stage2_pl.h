#ifndef STAGE2_PL_H
#define STAGE2_PL_H
#include "ir/ct_vec.h"
typedef struct stage2_pl_t {
    const char **pl_dyn_name;
    int *pl_dyn_arity;
    int pl_dyn_n;
    int pl_dyn_cap;
    long long * db_decls_words;
    int db_decls_n;
    long long * pl_ground_words;
    int pl_ground_n;
    int pl_ground_cap;
    cv_t pl_prelude_keys;
    cv_t pl_user_keys_files;
    cv_t pl_decl_multifile;
    cv_t pl_decl_meta;
    int pl_dq_mode;
    int pl_dialect;
    int pl_swi_switch;
    const char *pl_source_path;
    int pl_scc_aux_n;
    int pl_attv_armed;
    int pl_undo_armed;
    int pl_apply_macros;
} stage2_pl_t;
#ifdef __cplusplus
extern "C" {
#endif
extern stage2_pl_t g_stage2_pl;
#ifdef __cplusplus
}
#endif
#endif
