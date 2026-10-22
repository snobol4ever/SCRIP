#include "g_runtime.h"
g_runtime_t g_runtime = { .eval = { .eval_cache = NULL, .eval_cache_n = 0, .eval_cache_cap = 0, .rt_fragment_emit = 0, .eval_ret_v = 1, .lbl_tab = NULL, .lbl_n = 0, .lbl_cap = 0,
    .sno_stmt_compiled = 0, }, .zsm = { .zsm_tr_n = 0UL, .zsm_violations = 0UL, .zsm_beta_no_alpha = 0UL, .zsm_events = 0UL, .zsm_fsm_illegal = 0UL, .zsm_gamma_events = 0UL, .zsm_leaked_at_exit = 0UL,
    .zsm_alpha_while_live = 0UL, .zsm_rsp0 = 0UL, .zsm_stmt0 = 0UL, .zsm_seen_n = 0UL, .zdp_rbp_violations = 0UL, .zdp_rbp_nobank = 0UL, }, .slab = { .pool_bytes = 0, .mx = PTHREAD_MUTEX_INITIALIZER,
    }, .agg = { .agg_list_ser = 1, .agg_table_ser = 1, .agg_set_ser = 1, .agg_external_ser = 1, .sno_dumpno = 0, }, .atoms = { .atom_dot = -1, .atom_nil = -1, .atom_true = -1, .atom_fail = -1,
    .atom_cut = -1, .functor_dot2 = -1 }, };
