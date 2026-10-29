#include <stddef.h>
#include "g_runtime.h"
#include "rt_diag.h"
#include "g_runtime_abi.inc"
g_runtime_t __attribute__((visibility("protected"))) g_runtime = { .misc = { .sn4_direct_v = -1, .monitor_quiet_depth = 0, .kw_stcount = 0, .kw_anchor = 0, .kw_stlimit = -1, .kw_trace = 0,
    .core_stack_floor_raised = 0, .core_err_active = 0, .kw_ctx = 0, .icn_errnumber = 0, .icn_errtext = "", .icn_err_valid = 0, .root = NULL, .bb_rnd_seed = 12345UL, .sno_defer_pair_hwm = 0,
    .dcap_base = 0, }, .abi = { .hp_fr = { .zfull = -1 }, .sxt_fr = { .off = -1 }, .error = 0, .trace = 0, .dump = 0, .random = 0, .stno = 0, .stcount = 0, .lastno = 0, .line = 0, .lastline = 0,
    .file = NULL, .kw_ftrace = 0, .kw_errlimit = 0, .kw_fnclevel = 0, .kw_rtntype = "", .trace_budget = 0, .call_fastpath_off = 0, .monitor_fd = -1, .monitor_bin = 0, .mon_max_stno = 0,
    .trace_set_n = 0, .comm_dbg = RT_DIAG ? -1 : 0, .sno_etrace_n = 0, .var_nbuckets = 0, .var_init_done = 0, .protected_pat_vars_armed = 0, .dcap_trace = RT_DIAG ? -1 : 0, .cap_gen = 1,
    .cap_gen_next = 1, .repl_trace = RT_DIAG ? -1 : 0, .rt_k_level = 1, .zdp_anchor_rsp = 0UL, .zdp_anchor_rbp = 0UL, .gc_polls = 0, .gc_poll_site = (const char *)0, .hp_gcline = (char *)0,
    .anchor = 0, .maxlngth = 5000000, .rtcc_on = 1, .rt_gen_procs = 0, }, .gc = { .gc_chain_knob = -1, .gc_line_mbv = 128, .hp_arena = 0, .hp_win = 0, .hp_wend = 0, .hp_cap_end = 0, .hp_chunk = 0,
    .hp_sw_win_kb = 0, .hp_sw_cap_kb = 0, .hp_capped = 0, .hp_grown = 0, .hp_grows = 0, .hp_live = 0, .hp_report_reg = 0, .hp_cap_due = 0, .gc_vac = 0, .gc_vacn = 0, .gc_vaccap = 0, .gc_vacover = 0,
    .gc_vacgen = 0, .hp_qlo = 0, .hp_flo = 0, .hp_fhi = 0, .gc_flip_to = 0, .gc_flips = 0, .gc_flip_declined = 0, .gc_flip_live = 0, .hp_qhi = 0, .hp_qgen = 0, .hp_qarm = 0, .hp_qbytes = 0,
    .ah_reg = 0, .ah_on = -1, .alloc_detax = 0, .hp_cap_road = 0, .gc_rrng_n = 0, .gc_rrng_cap = 0, .gc_nseg = -1, .gc_seg_cap = 0, .gc_idx = 0, .gc_idxbuf = 0, .gc_icap = 0, .gc_nblk = 0,
    .gc_runs = 0, .gc_interior = 0, .gc_stktop = 0, .gc_seam_sp = 0, .gc_shield_arr = 0, .gc_shield_n = 0, .gc_shield_r = 0, .gc_shield_r_is_probe = 0, .gc_dvec_elems = 0, .gc_dvec_nondvec = 0,
    .gc_rrng_ss = 0, .gc_maps = 0, .gc_maps_n = 0, .gc_maps_cap = 0, .gc_sblk = 0, .gc_sblk_n = 0, .gc_sblk_cap = 0, .gc_program_top = 0, .gc_seg_main = 0, .gc_maps_rep = -1, .gc_cas_bytes = 0, },
    .bn = { .tweak_gen_bump = 0, .pl_edin_out_name = "", .pl_edin_in_name = "", .ctor_ic_site = 0, .main_args_n = -1, .bidprof_on = -1, .bidjmp_on = 1, }, .tap = { .tap_run = 0, .tap_failed = 0,
    .tap_planned = 0, .tap_no_plan = 1, .tap_todo_upto = 0, .tap_todo_reason = "", .tap_done_run = 0, }, .pas = { .pas_heap_ctr = 0, .pas_heap = 0, .pas_heap_cap = 0, .pas_heap_dead = 0,
    .pas_heap_free = 0, .pas_heap_nfree = 0, .pas_heap_freecap = 0, }, .coexpr = { .inited = 0, .coexp_stksize = 8L * 1024 * 1024, .co_main_set = 0, .coexpr_serial = 1, }, .kw = { .kwb_own = { 0, 0,
    0, 0, 0, 0, 0, 1 }, .kw_maxint = 9223372036854775807LL, .lastfile = NULL, .sno_errtext = NULL, .kw_cset_names = NULL, .kw_cset_count = 0, .kw_cset_cap = 0, .kw_cset_hidx = NULL, .kw_cset_hcap = 0,
    .kw_cset_cidx = NULL, .kw_cset_hidx_gen = -1, }, .rt = { .subject_dbg_base = 0, .subject_dbg_len = -1, .rt_gen_proc_count = 0, .rt_gen_proc_cap = 0, .proc_hsl = 0, .proc_hcap = 0, .gva_mapped = 0,
    }, .core = { .monitor_ack_fd = -1, .monitor_ready = 0, .trace_tap_off = 0, .trace_access_n = 0, .trace_recursion_depth = 0, .trace_depth_override = -1, .trace_stmt_seen = 0, .trace_stmt_gen = 0,
    .bin_names = NULL, .bin_name_lens = NULL, .bin_n_names = 0, .bin_names_cap = 0, .kw_fullscan = 0, .kw_maxlngth = 524288, .kw_trim = 0, .kw_code = 0, .host_u = NULL, .sw_stack_bytes = 0,
    .start_ns = -1, .setexit_resume = -1, .data_ntypes = 0, .facc_n = 0, .udef_types = NULL, .var_count = 0, .input_buf = NULL, .input_cap = 0, .input_rlen = 0, .terminal_fp = NULL,
    .terminal_tried = 0, }, .eval = { .eval_cache = NULL, .eval_cache_n = 0, .eval_cache_cap = 0, .rt_fragment_emit = 0, .eval_ret_v = 1, .lbl_tab = NULL, .lbl_n = 0, .lbl_cap = 0,
    .sno_stmt_compiled = 0, }, .zsm = { .zsm_tr_n = 0UL, .zsm_violations = 0UL, .zsm_beta_no_alpha = 0UL, .zsm_events = 0UL, .zsm_fsm_illegal = 0UL, .zsm_gamma_events = 0UL, .zsm_leaked_at_exit = 0UL,
    .zsm_alpha_while_live = 0UL, .zsm_rsp0 = 0UL, .zsm_stmt0 = 0UL, .zsm_seen_n = 0UL, .zdp_rbp_violations = 0UL, .zdp_rbp_nobank = 0UL, }, .slab = { .pool_bytes = 0, .mx = PTHREAD_MUTEX_INITIALIZER,
    }, .agg = { .agg_list_ser = 1, .agg_table_ser = 1, .agg_set_ser = 1, .agg_external_ser = 1, .sno_dumpno = 0, }, .atoms = { .atom_dot = -1, .atom_nil = -1, .atom_true = -1, .atom_fail = -1,
    .atom_cut = -1, .functor_dot2 = -1 }, };
#include "g_runtime_abi_check.inc"
