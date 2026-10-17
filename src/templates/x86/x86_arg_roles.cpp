/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "x86_arg_roles.h"
const char x86_argrole_blob[] =
    "\0"
    "NV_GET_fn\0"
    "name\0"
    "NV_GET_open\0"
    "pend\0"
    "NV_PTR_fn\0"
    "NV_SET_fn\0"
    "val\0"
    "comm_var_open\0"
    "stno\0"
    "core_icn_argtype_check\0"
    "lo\0"
    "hi\0"
    "code\0"
    "core_icn_by_zero_check\0"
    "by\0"
    "core_icn_int_operand_ok\0"
    "core_icn_limit_count_check\0"
    "core_icn_to_int_check\0"
    "descr_identical\0"
    "a\0"
    "b\0"
    "icn_field_get\0"
    "fname\0"
    "obj\0"
    "memcmp\0"
    "s1\0"
    "s2\0"
    "n\0"
    "rk_box_back\0"
    "subj\0"
    "delta\0"
    "blob\0"
    "floor_delta\0"
    "rk_box_step\0"
    "end\0"
    "rk_is_truthy\0"
    "v\0"
    "rk_rat_binop\0"
    "op\0"
    "out\0"
    "rt_apply_open\0"
    "args\0"
    "nargs\0"
    "rt_arg_stage\0"
    "idx\0"
    "rt_at_cursor\0"
    "varname\0"
    "cur_delta\0"
    "rt_at_cursor_open\0"
    "rec\0"
    "rt_binop_overload_open\0"
    "rt_call_apply_gen_h\0"
    "callee\0"
    "lv\0"
    "hslot\0"
    "rt_call_apply_spine_prep\0"
    "rt_call_apply_spine_prep_blk\0"
    "rt_call_fld_sn4\0"
    "r\0"
    "rt_call_pas_bi\0"
    "rt_call_pend_close\0"
    "rt_call_value_gen_h\0"
    "argv\0"
    "rt_call_value_resume_h\0"
    "rt_call_value_spine_prep\0"
    "rt_call_value_spine_prep_blk\0"
    "rt_cap_open_gva\0"
    "cell\0"
    "saved_delta\0"
    "rt_cmp_d\0"
    "rt_coerce_int_d\0"
    "in\0"
    "codes\0"
    "rt_coerce_num2_d\0"
    "self\0"
    "other\0"
    "rt_coerce_real_d\0"
    "rt_coerce_str_d\0"
    "rt_cset_compl\0"
    "rt_dcap_resume\0"
    "c\0"
    "rt_defer_close\0"
    "rt_defer_land_chain\0"
    "word\0"
    "s\0"
    "rt_defer_open_cell\0"
    "ival_flag\0"
    "rt_defer_open_entry\0"
    "rt_defer_open_rec\0"
    "rt_defer_probe_run\0"
    "site\0"
    "rt_define_bind_entry\0"
    "entry\0"
    "rt_define_site\0"
    "params_csv\0"
    "nparams\0"
    "nformals\0"
    "frame_bytes\0"
    "fn\0"
    "rt_define_site_entry\0"
    "rt_deref\0"
    "d\0"
    "rt_eval_land\0"
    "rt_field_var_strict\0"
    "rt_gcheap_alloc\0"
    "type\0"
    "payload_bytes\0"
    "rt_goto_resolve\0"
    "rt_icn_cset_member_n\0"
    "needle\0"
    "len\0"
    "ch\0"
    "rt_icn_cset_register\0"
    "ptr\0"
    "rt_icn_zframe_args_install\0"
    "base_p\0"
    "nlocals\0"
    "rt_is_truthy\0"
    "rt_jct_relop\0"
    "lhs\0"
    "rhs\0"
    "rt_keyword_dump_set\0"
    "rt_keyword_error_set\0"
    "rt_keyword_gen\0"
    "sval\0"
    "rt_keyword_random_set\0"
    "rt_keyword_read\0"
    "rt_keyword_read_snobol4\0"
    "rt_keyword_trace_set\0"
    "rt_keyword_var\0"
    "rt_kw_read_idx\0"
    "rt_kw_write_idx\0"
    "rt_list_bang_at\0"
    "rt_list_bang_elem_at\0"
    "rt_list_bang_key_at\0"
    "rt_list_bang_var_at\0"
    "rt_match_ctx_restore\0"
    "sig\0"
    "rt_match_enter\0"
    "rt_match_replace\0"
    "sub_lo\0"
    "sub_hi\0"
    "start\0"
    "replp\0"
    "rt_match_value_get_pat_dtp\0"
    "pval\0"
    "rt_match_value_open\0"
    "rt_pat_prim_member\0"
    "rt_pat_prim_open\0"
    "rt_pat_prim_str_take\0"
    "out_ptr\0"
    "out_len\0"
    "rt_patv_defer_get_pat_dtp\0"
    "hv\0"
    "i\0"
    "fb\0"
    "rt_patv_defer_open_entry\0"
    "rt_pl_dc_prep\0"
    "suffix_off\0"
    "region_bytes\0"
    "np\0"
    "rt_pl_goal_resolve\0"
    "goal\0"
    "rt_pl_unify_const_cold\0"
    "k\0"
    "atom\0"
    "rt_proc_call_open\0"
    "rt_proc_call_open_det\0"
    "rt_proc_drop_frame_h\0"
    "rt_relop_rat\0"
    "rt_relop_val_coerce\0"
    "rt_rev_swap_fwd\0"
    "lkind\0"
    "lp\0"
    "rkind\0"
    "rp\0"
    "save\0"
    "spill\0"
    "rt_rev_swap_undo\0"
    "rt_scan_enter\0"
    "rt_scan_leave\0"
    "outer_sigma\0"
    "outer_delta\0"
    "outer_len\0"
    "rt_scan_needle\0"
    "rt_scan_reenter_live\0"
    "rt_scan_sync_out\0"
    "rt_shim_nv_gamma\0"
    "proc\0"
    "cells\0"
    "rt_shim_nv_omega\0"
    "rt_size_d\0"
    "rt_sno_goto_special_is\0"
    "enc\0"
    "rt_stmt_enter_p\0"
    "line\0"
    "rt_subscript_val_strict\0"
    "base\0"
    "rt_substr\0"
    "sigma\0"
    "rt_swap_var\0"
    "va\0"
    "vb\0"
    "rt_table_assign_fast\0"
    "rt_trace_call_hook_f\0"
    "rt_trace_call_hook_p\0"
    "rt_trace_fail_hook\0"
    "rt_trace_fail_hook_p\0"
    "rstno\0"
    "rt_trace_gen_fail_hook\0"
    "h\0"
    "rt_trace_gen_return_hook\0"
    "pname\0"
    "rt_trace_label_hook_p\0"
    "rt_trace_pend_close\0"
    "rt_trace_pend_run\0"
    "rt_trace_resume_hook\0"
    "rt_trace_return_hook\0"
    "retval\0"
    "rt_trace_return_hook_p\0"
    "rt_trace_stmt\0"
    "rt_trace_suspend_hook\0"
    "rt_tweak_open\0"
    "cname\0"
    "rt_value_trace_open\0"
    "rt_var_ref_cell_named\0"
    "cellp\0"
    "scrip_coexpr_activate\0"
    "target\0"
    "x0\0"
    "x1\0"
    "out2\0"
    "procname\0"
    "tagw\0"
    "scrip_coexpr_create\0"
    "body_entry_addr\0"
    "regs\0"
    "below_bytes\0"
    "scrip_coret\0"
    "d0\0"
    "d1\0"
    "resume_addr\0"
    "sno_trace_call\0"
    "sno_trace_return\0"
    "str_concat_d\0" ;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const x86_argrole_t x86_argroles[130] = {
    { 1, { 11, 0, 0, 0, 0, 0 } },
    { 16, { 11, 28, 0, 0, 0, 0 } },
    { 33, { 11, 0, 0, 0, 0, 0 } },
    { 43, { 11, 53, 53, 0, 0, 0 } },
    { 57, { 11, 53, 53, 71, 28, 0 } },
    { 76, { 99, 102, 105, 0, 0, 0 } },
    { 110, { 133, 0, 0, 0, 0, 0 } },
    { 136, { 99, 102, 0, 0, 0, 0 } },
    { 160, { 99, 102, 0, 0, 0, 0 } },
    { 187, { 99, 102, 0, 0, 0, 0 } },
    { 209, { 225, 225, 227, 227, 0, 0 } },
    { 229, { 243, 249, 249, 0, 0, 0 } },
    { 253, { 260, 263, 266, 0, 0, 0 } },
    { 268, { 280, 285, 291, 296, 0, 0 } },
    { 308, { 280, 285, 320, 291, 0, 0 } },
    { 324, { 337, 337, 0, 0, 0, 0 } },
    { 339, { 225, 225, 227, 227, 352, 355 } },
    { 359, { 373, 378, 0, 0, 0, 0 } },
    { 384, { 397, 337, 337, 0, 0, 0 } },
    { 401, { 414, 422, 28, 0, 0, 0 } },
    { 432, { 414, 422, 450, 0, 0, 0 } },
    { 454, { 225, 225, 227, 227, 352, 0 } },
    { 477, { 497, 497, 504, 504, 507, 0 } },
    { 513, { 497, 497, 504, 504, 0, 0 } },
    { 538, { 497, 497, 504, 504, 507, 0 } },
    { 567, { 373, 378, 583, 0, 0, 0 } },
    { 585, { 373, 378, 397, 0, 0, 0 } },
    { 600, { 28, 0, 0, 0, 0, 0 } },
    { 619, { 497, 497, 639, 266, 507, 0 } },
    { 644, { 507, 266, 0, 0, 0, 0 } },
    { 667, { 497, 497, 639, 266, 0, 0 } },
    { 692, { 497, 497, 639, 266, 507, 0 } },
    { 721, { 737, 742, 422, 414, 0, 0 } },
    { 754, { 225, 227, 0, 0, 0, 0 } },
    { 763, { 779, 355, 782, 0, 0, 0 } },
    { 788, { 805, 810, 355, 782, 0, 0 } },
    { 816, { 779, 355, 782, 0, 0, 0 } },
    { 833, { 779, 355, 782, 0, 0, 0 } },
    { 849, { 225, 225, 0, 0, 0, 0 } },
    { 863, { 878, 0, 0, 0, 0, 0 } },
    { 880, { 422, 450, 0, 0, 0, 0 } },
    { 895, { 915, 920, 0, 0, 0, 0 } },
    { 922, { 737, 941, 450, 0, 0, 0 } },
    { 951, { 414, 941, 450, 0, 0, 0 } },
    { 971, { 583, 450, 0, 0, 0, 0 } },
    { 989, { 414, 422, 1008, 0, 0, 0 } },
    { 1013, { 243, 1034, 0, 0, 0, 0 } },
    { 1040, { 11, 1055, 1066, 1074, 1083, 1095 } },
    { 1098, { 11, 1034, 0, 0, 0, 0 } },
    { 1119, { 1128, 1128, 0, 0, 0, 0 } },
    { 1130, { 915, 0, 0, 0, 0, 0 } },
    { 1143, { 243, 249, 249, 0, 0, 0 } },
    { 1163, { 1179, 1184, 0, 0, 0, 0 } },
    { 1198, { 11, 0, 0, 0, 0, 0 } },
    { 1214, { 1235, 1242, 1246, 0, 0, 0 } },
    { 1249, { 1270, 1242, 0, 0, 0, 0 } },
    { 1274, { 1301, 1066, 1308, 0, 0, 0 } },
    { 1316, { 337, 337, 0, 0, 0, 0 } },
    { 1329, { 1342, 1342, 1346, 1346, 352, 0 } },
    { 1350, { 337, 337, 0, 0, 0, 0 } },
    { 1370, { 337, 337, 0, 0, 0, 0 } },
    { 1391, { 1406, 397, 0, 0, 0, 0 } },
    { 1411, { 337, 337, 0, 0, 0, 0 } },
    { 1433, { 1406, 0, 0, 0, 0, 0 } },
    { 1449, { 1406, 0, 0, 0, 0, 0 } },
    { 1473, { 337, 337, 0, 0, 0, 0 } },
    { 1494, { 11, 0, 0, 0, 0, 0 } },
    { 1509, { 397, 0, 0, 0, 0, 0 } },
    { 1524, { 397, 337, 337, 0, 0, 0 } },
    { 1540, { 249, 249, 397, 0, 0, 0 } },
    { 1556, { 249, 249, 397, 0, 0, 0 } },
    { 1577, { 249, 249, 397, 0, 0, 0 } },
    { 1597, { 249, 249, 397, 0, 0, 0 } },
    { 1617, { 1638, 1242, 737, 0, 0, 0 } },
    { 1642, { 99, 102, 0, 0, 0, 0 } },
    { 1657, { 11, 1674, 1681, 1688, 320, 1694 } },
    { 1700, { 1727, 0, 0, 0, 0, 0 } },
    { 1732, { 1727, 450, 0, 0, 0, 0 } },
    { 1752, { 1246, 782, 0, 0, 0, 0 } },
    { 1771, { 414, 0, 0, 0, 0, 0 } },
    { 1788, { 1809, 1817, 782, 0, 0, 0 } },
    { 1825, { 1851, 1854, 1856, 0, 0, 0 } },
    { 1859, { 1851, 1854, 1856, 941, 450, 0 } },
    { 1884, { 1856, 1898, 1909, 1922, 378, 397 } },
    { 1925, { 1944, 1944, 266, 0, 0, 0 } },
    { 1949, { 878, 1972, 1974, 0, 0, 0 } },
    { 1979, { 11, 378, 0, 0, 0, 0 } },
    { 1997, { 397, 378, 0, 0, 0, 0 } },
    { 2019, { 507, 0, 0, 0, 0, 0 } },
    { 2040, { 225, 225, 227, 227, 352, 355 } },
    { 2053, { 225, 225, 227, 227, 355, 0 } },
    { 2073, { 2089, 2095, 2098, 2104, 2107, 2112 } },
    { 2118, { 2089, 2095, 2098, 2104, 2107, 2112 } },
    { 2135, { 99, 102, 0, 0, 0, 0 } },
    { 2149, { 2163, 2175, 2187, 0, 0, 0 } },
    { 2197, { 99, 102, 0, 0, 0, 0 } },
    { 2212, { 280, 1242, 0, 0, 0, 0 } },
    { 2233, { 285, 0, 0, 0, 0, 0 } },
    { 2250, { 2267, 2272, 0, 0, 0, 0 } },
    { 2278, { 2267, 2272, 0, 0, 0, 0 } },
    { 2295, { 99, 102, 0, 0, 0, 0 } },
    { 2305, { 2328, 0, 0, 0, 0, 0 } },
    { 2332, { 71, 2348, 28, 0, 0, 0 } },
    { 2353, { 2377, 2377, 397, 397, 0, 0 } },
    { 2382, { 2392, 225, 227, 0, 0, 0 } },
    { 2398, { 2410, 2410, 2413, 2413, 0, 0 } },
    { 2416, { 2377, 2377, 397, 397, 53, 53 } },
    { 2437, { 243, 1922, 2377, 0, 0, 0 } },
    { 2458, { 243, 28, 0, 0, 0, 0 } },
    { 2479, { 243, 0, 0, 0, 0, 0 } },
    { 2498, { 243, 28, 2519, 0, 0, 0 } },
    { 2525, { 243, 2548, 0, 0, 0, 0 } },
    { 2550, { 2575, 99, 102, 2548, 0, 0 } },
    { 2581, { 11, 28, 0, 0, 0, 0 } },
    { 2603, { 28, 0, 0, 0, 0, 0 } },
    { 2623, { 28, 0, 0, 0, 0, 0 } },
    { 2641, { 2575, 2548, 0, 0, 0, 0 } },
    { 2662, { 243, 2683, 2683, 0, 0, 0 } },
    { 2690, { 243, 2683, 2683, 28, 2519, 0 } },
    { 2713, { 2348, 0, 0, 0, 0, 0 } },
    { 2727, { 2575, 99, 102, 2348, 0, 0 } },
    { 2749, { 355, 2763, 28, 0, 0, 0 } },
    { 2769, { 11, 28, 0, 0, 0, 0 } },
    { 2789, { 2811, 11, 0, 0, 0, 0 } },
    { 2817, { 2839, 2846, 2849, 2852, 2857, 2866 } },
    { 2871, { 2891, 2907, 1083, 2912, 2857, 0 } },
    { 2924, { 2936, 2939, 2942, 0, 0, 0 } },
    { 2954, { 243, 0, 0, 0, 0, 0 } },
    { 2969, { 243, 2683, 2683, 0, 0, 0 } },
    { 2986, { 225, 225, 227, 227, 0, 0 } },
};
