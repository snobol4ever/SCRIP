#include "g_runtime.h"
g_runtime_t g_runtime = { .core = { .monitor_ack_fd = -1, .monitor_ready = 0, .trace_tap_off = 0, .trace_access_n = 0, .trace_recursion_depth = 0, .trace_depth_override = -1, .trace_stmt_seen = 0,
    .trace_stmt_gen = 0, .bin_names = NULL, .bin_name_lens = NULL, .bin_n_names = 0, .bin_names_cap = 0, .kw_fullscan = 0, .kw_maxlngth = 524288, .kw_trim = 0, .kw_code = 0, .host_u = NULL,
    .sw_stack_bytes = 0, .start_ns = -1, .setexit_resume = -1, .data_ntypes = 0, .facc_n = 0, .udef_types = NULL, .var_count = 0, .input_buf = NULL, .input_cap = 0, .input_rlen = 0,
    .terminal_fp = NULL, .terminal_tried = 0, }, .eval = { .eval_cache = NULL, .eval_cache_n = 0, .eval_cache_cap = 0, .rt_fragment_emit = 0, .eval_ret_v = 1, .lbl_tab = NULL, .lbl_n = 0,
    .lbl_cap = 0, .sno_stmt_compiled = 0, }, .zsm = { .zsm_tr_n = 0UL, .zsm_violations = 0UL, .zsm_beta_no_alpha = 0UL, .zsm_events = 0UL, .zsm_fsm_illegal = 0UL, .zsm_gamma_events = 0UL,
    .zsm_leaked_at_exit = 0UL, .zsm_alpha_while_live = 0UL, .zsm_rsp0 = 0UL, .zsm_stmt0 = 0UL, .zsm_seen_n = 0UL, .zdp_rbp_violations = 0UL, .zdp_rbp_nobank = 0UL, }, .slab = { .pool_bytes = 0,
    .mx = PTHREAD_MUTEX_INITIALIZER, }, .agg = { .agg_list_ser = 1, .agg_table_ser = 1, .agg_set_ser = 1, .agg_external_ser = 1, .sno_dumpno = 0, }, .atoms = { .atom_dot = -1, .atom_nil = -1,
    .atom_true = -1, .atom_fail = -1, .atom_cut = -1, .functor_dot2 = -1 }, };
