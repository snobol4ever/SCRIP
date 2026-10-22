#ifndef G_RUNTIME_ZSM_H
#define G_RUNTIME_ZSM_H
#include "ct_vec.h"
#define ZSM_N 65536
#define ZSM_TRACE 2048
typedef struct { unsigned long node, E, F, rsp_a, live; int state; } zsm_ent;
typedef struct {
    zsm_ent zsm[ZSM_N];
    unsigned long zsm_tr_node[ZSM_TRACE];
    unsigned long zsm_tr_rbp[ZSM_TRACE];
    unsigned long zsm_tr_rsp[ZSM_TRACE];
    long zsm_tr_kind[ZSM_TRACE];
    long zsm_tr_depth[ZSM_TRACE];
    long zsm_tr_stno[ZSM_TRACE];
    unsigned long zsm_tr_n;
    unsigned long zsm_violations;
    unsigned long zsm_beta_no_alpha;
    unsigned long zsm_events;
    unsigned long zsm_fsm_illegal;
    unsigned long zsm_gamma_events;
    unsigned long zsm_leaked_at_exit;
    unsigned long zsm_alpha_while_live;
    unsigned long zsm_rsp0;
    unsigned long zsm_stmt0;
    cv_t zsm_seen;
    unsigned long zsm_seen_n;
    unsigned long zdp_rbp_violations;
    unsigned long zdp_rbp_nobank;
} g_zsm_t;
#endif
