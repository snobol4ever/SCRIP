#ifndef G_LOWER_H
#define G_LOWER_H
#include "ct_vec.h"
typedef struct {
    cv_t bb_cv;
    const char ** decl_dyn_name;
    int * decl_dyn_arity;
    int decl_dyn_n;
    int decl_dyn_cap;
    const char ** decl_other_name;
    int * decl_other_arity;
    int decl_other_n;
    int decl_other_cap;
    int fresh_next;
    long trace_stmtno;
    int seed_var_base;
    int fence_on;
    char * var_name_cache[1024];
    cv_t param_name_cache;
} g_lower_pl_t;
typedef struct { g_lower_pl_t pl; } g_lower_t;
extern g_lower_t g_lower;
#endif
