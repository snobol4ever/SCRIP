#include "g_runtime.h"
g_runtime_t g_runtime = { .bn = { .tweak_gen_bump = 0, .pl_edin_out_name = "", .pl_edin_in_name = "", .ctor_ic_site = (const char *)0, .main_args_n = -1, .bidprof_on = -1, .bidjmp_on = 1, },
    .tap = { .tap_run = 0, .tap_failed = 0, .tap_planned = 0, .tap_no_plan = 1, .tap_todo_upto = 0, .tap_todo_reason = "", .tap_done_run = 0, }, .pas = { .pas_heap_ctr = 0, .pas_heap = (DESCR_t *)0,
    .pas_heap_cap = 0, .pas_heap_dead = (unsigned char *)0, .pas_heap_free = (long *)0, .pas_heap_nfree = 0, .pas_heap_freecap = 0, }, .coexpr = { .inited = 0, .coexp_stksize = 8L * 1024 * 1024,
    .co_main_set = 0, .coexpr_serial = 1, }, .kw = { .kw_maxint = 9223372036854775807LL, .lastfile = NULL, .sno_errtext = NULL, .kw_cset_names = NULL, .kw_cset_count = 0, .kw_cset_cap = 0,
    .kw_cset_hidx = NULL, .kw_cset_hcap = 0, .kw_cset_cidx = NULL, .kw_cset_hidx_gen = -1, }, .rt = { .subject_dbg_base = 0, .subject_dbg_len = -1, .rt_gen_proc_count = 0, .rt_gen_proc_cap = 0,
    .proc_hsl = (int *)0, .proc_hcap = 0, .gva_mapped = 0, }, .core = { .monitor_ack_fd = -1, .monitor_ready = 0, .trace_tap_off = 0, .trace_access_n = 0, .trace_recursion_depth = 0,
    .trace_depth_override = -1, .trace_stmt_seen = 0, .trace_stmt_gen = 0, .bin_names = NULL, .bin_name_lens = NULL, .bin_n_names = 0, .bin_names_cap = 0, .kw_fullscan = 0, .kw_maxlngth = 524288,
    .kw_trim = 0, .kw_code = 0, .host_u = NULL, .sw_stack_bytes = 0, .start_ns = -1, .setexit_resume = -1, .data_ntypes = 0, .facc_n = 0, .udef_types = NULL, .var_count = 0, .input_buf = NULL,
    .input_cap = 0, .input_rlen = 0, .terminal_fp = NULL, .terminal_tried = 0, }, .eval = { .eval_cache = NULL, .eval_cache_n = 0, .eval_cache_cap = 0, .rt_fragment_emit = 0, .eval_ret_v = 1,
    .lbl_tab = NULL, .lbl_n = 0, .lbl_cap = 0, .sno_stmt_compiled = 0, }, .zsm = { .zsm_tr_n = 0UL, .zsm_violations = 0UL, .zsm_beta_no_alpha = 0UL, .zsm_events = 0UL, .zsm_fsm_illegal = 0UL,
    .zsm_gamma_events = 0UL, .zsm_leaked_at_exit = 0UL, .zsm_alpha_while_live = 0UL, .zsm_rsp0 = 0UL, .zsm_stmt0 = 0UL, .zsm_seen_n = 0UL, .zdp_rbp_violations = 0UL, .zdp_rbp_nobank = 0UL, },
    .slab = { .pool_bytes = 0, .mx = PTHREAD_MUTEX_INITIALIZER, }, .agg = { .agg_list_ser = 1, .agg_table_ser = 1, .agg_set_ser = 1, .agg_external_ser = 1, .sno_dumpno = 0, },
    .atoms = { .atom_dot = -1, .atom_nil = -1, .atom_true = -1, .atom_fail = -1, .atom_cut = -1, .functor_dot2 = -1 }, };
