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
    "core_icn_by_zero_at\0"
    "from\0"
    "to\0"
    "by\0"
    "core_icn_int_operand_ok\0"
    "core_icn_limit_count_check\0"
    "core_icn_to_int_check\0"
    "core_icn_toby_ctx\0"
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
    "rt_defer_open_entry\0"
    "ival_flag\0"
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
    "cur\0"
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
    "rt_icn_needle_coerce\0"
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
    { 110, { 130, 135, 138, 0, 0, 0 } },
    { 141, { 99, 102, 0, 0, 0, 0 } },
    { 165, { 99, 102, 0, 0, 0, 0 } },
    { 192, { 99, 102, 0, 0, 0, 0 } },
    { 214, { 130, 135, 138, 0, 0, 0 } },
    { 232, { 248, 248, 250, 250, 0, 0 } },
    { 252, { 266, 272, 272, 0, 0, 0 } },
    { 276, { 283, 286, 289, 0, 0, 0 } },
    { 291, { 303, 308, 314, 319, 0, 0 } },
    { 331, { 303, 308, 343, 314, 0, 0 } },
    { 347, { 360, 360, 0, 0, 0, 0 } },
    { 362, { 248, 248, 250, 250, 375, 378 } },
    { 382, { 396, 401, 0, 0, 0, 0 } },
    { 407, { 420, 360, 360, 0, 0, 0 } },
    { 424, { 437, 445, 28, 0, 0, 0 } },
    { 455, { 437, 445, 473, 0, 0, 0 } },
    { 477, { 248, 248, 250, 250, 375, 0 } },
    { 500, { 520, 520, 527, 527, 530, 0 } },
    { 536, { 520, 520, 527, 527, 0, 0 } },
    { 561, { 520, 520, 527, 527, 530, 0 } },
    { 590, { 396, 401, 606, 0, 0, 0 } },
    { 608, { 396, 401, 420, 0, 0, 0 } },
    { 623, { 28, 0, 0, 0, 0, 0 } },
    { 642, { 520, 520, 662, 289, 530, 0 } },
    { 667, { 530, 289, 0, 0, 0, 0 } },
    { 690, { 520, 520, 662, 289, 0, 0 } },
    { 715, { 520, 520, 662, 289, 530, 0 } },
    { 744, { 760, 765, 445, 437, 0, 0 } },
    { 777, { 248, 250, 0, 0, 0, 0 } },
    { 786, { 802, 378, 805, 0, 0, 0 } },
    { 811, { 828, 833, 378, 805, 0, 0 } },
    { 839, { 802, 378, 805, 0, 0, 0 } },
    { 856, { 802, 378, 805, 0, 0, 0 } },
    { 872, { 248, 248, 0, 0, 0, 0 } },
    { 886, { 901, 0, 0, 0, 0, 0 } },
    { 903, { 445, 473, 0, 0, 0, 0 } },
    { 918, { 938, 943, 0, 0, 0, 0 } },
    { 945, { 437, 965, 473, 0, 0, 0 } },
    { 975, { 606, 473, 0, 0, 0, 0 } },
    { 993, { 437, 445, 1012, 0, 0, 0 } },
    { 1017, { 266, 1038, 0, 0, 0, 0 } },
    { 1044, { 11, 1059, 1070, 1078, 1087, 1099 } },
    { 1102, { 11, 1038, 0, 0, 0, 0 } },
    { 1123, { 1132, 1132, 0, 0, 0, 0 } },
    { 1134, { 938, 0, 0, 0, 0, 0 } },
    { 1147, { 266, 272, 272, 0, 0, 0 } },
    { 1167, { 1183, 1188, 0, 0, 0, 0 } },
    { 1202, { 11, 0, 0, 0, 0, 0 } },
    { 1218, { 1239, 1246, 1250, 0, 0, 0 } },
    { 1253, { 1274, 1246, 0, 0, 0, 0 } },
    { 1278, { 1305, 1070, 1312, 0, 0, 0 } },
    { 1320, { 360, 360, 0, 0, 0, 0 } },
    { 1333, { 1346, 1346, 1350, 1350, 375, 0 } },
    { 1354, { 360, 360, 0, 0, 0, 0 } },
    { 1374, { 360, 360, 0, 0, 0, 0 } },
    { 1395, { 1410, 420, 0, 0, 0, 0 } },
    { 1415, { 360, 360, 0, 0, 0, 0 } },
    { 1437, { 1410, 0, 0, 0, 0, 0 } },
    { 1453, { 1410, 0, 0, 0, 0, 0 } },
    { 1477, { 360, 360, 0, 0, 0, 0 } },
    { 1498, { 11, 0, 0, 0, 0, 0 } },
    { 1513, { 420, 0, 0, 0, 0, 0 } },
    { 1528, { 420, 360, 360, 0, 0, 0 } },
    { 1544, { 272, 272, 1560, 0, 0, 0 } },
    { 1564, { 272, 272, 1560, 0, 0, 0 } },
    { 1585, { 272, 272, 1560, 0, 0, 0 } },
    { 1605, { 272, 272, 1560, 0, 0, 0 } },
    { 1625, { 1646, 1246, 760, 0, 0, 0 } },
    { 1650, { 99, 102, 0, 0, 0, 0 } },
    { 1665, { 11, 1682, 1689, 1696, 343, 1702 } },
    { 1708, { 1735, 0, 0, 0, 0, 0 } },
    { 1740, { 1735, 473, 0, 0, 0, 0 } },
    { 1760, { 1250, 805, 0, 0, 0, 0 } },
    { 1779, { 437, 0, 0, 0, 0, 0 } },
    { 1796, { 1817, 1825, 805, 0, 0, 0 } },
    { 1833, { 1859, 1862, 1864, 0, 0, 0 } },
    { 1867, { 1859, 1862, 1864, 965, 473, 0 } },
    { 1892, { 1864, 1906, 1917, 1930, 401, 420 } },
    { 1933, { 1952, 1952, 289, 0, 0, 0 } },
    { 1957, { 901, 1980, 1982, 0, 0, 0 } },
    { 1987, { 11, 401, 0, 0, 0, 0 } },
    { 2005, { 420, 401, 0, 0, 0, 0 } },
    { 2027, { 530, 0, 0, 0, 0, 0 } },
    { 2048, { 248, 248, 250, 250, 375, 378 } },
    { 2061, { 248, 248, 250, 250, 378, 0 } },
    { 2081, { 2097, 2103, 2106, 2112, 2115, 2120 } },
    { 2126, { 2097, 2103, 2106, 2112, 2115, 2120 } },
    { 2143, { 99, 102, 0, 0, 0, 0 } },
    { 2157, { 2171, 2183, 2195, 0, 0, 0 } },
    { 2205, { 99, 102, 0, 0, 0, 0 } },
    { 2220, { 303, 1246, 0, 0, 0, 0 } },
    { 2241, { 308, 0, 0, 0, 0, 0 } },
    { 2258, { 2275, 2280, 0, 0, 0, 0 } },
    { 2286, { 2275, 2280, 0, 0, 0, 0 } },
    { 2303, { 99, 102, 0, 0, 0, 0 } },
    { 2313, { 2336, 0, 0, 0, 0, 0 } },
    { 2340, { 71, 2356, 28, 0, 0, 0 } },
    { 2361, { 2385, 2385, 420, 420, 0, 0 } },
    { 2390, { 2400, 248, 250, 0, 0, 0 } },
    { 2406, { 2418, 2418, 2421, 2421, 0, 0 } },
    { 2424, { 2385, 2385, 420, 420, 53, 53 } },
    { 2445, { 266, 1930, 2385, 0, 0, 0 } },
    { 2466, { 266, 28, 0, 0, 0, 0 } },
    { 2487, { 266, 0, 0, 0, 0, 0 } },
    { 2506, { 266, 28, 2527, 0, 0, 0 } },
    { 2533, { 266, 2556, 0, 0, 0, 0 } },
    { 2558, { 2583, 99, 102, 2556, 0, 0 } },
    { 2589, { 11, 28, 0, 0, 0, 0 } },
    { 2611, { 28, 0, 0, 0, 0, 0 } },
    { 2631, { 28, 0, 0, 0, 0, 0 } },
    { 2649, { 2583, 2556, 0, 0, 0, 0 } },
    { 2670, { 266, 2691, 2691, 0, 0, 0 } },
    { 2698, { 266, 2691, 2691, 28, 2527, 0 } },
    { 2721, { 2356, 0, 0, 0, 0, 0 } },
    { 2735, { 2583, 99, 102, 2356, 0, 0 } },
    { 2757, { 378, 2771, 28, 0, 0, 0 } },
    { 2777, { 11, 28, 0, 0, 0, 0 } },
    { 2797, { 2819, 11, 0, 0, 0, 0 } },
    { 2825, { 2847, 2854, 2857, 2860, 2865, 2874 } },
    { 2879, { 2899, 2915, 1087, 2920, 2865, 0 } },
    { 2932, { 2944, 2947, 2950, 0, 0, 0 } },
    { 2962, { 266, 0, 0, 0, 0, 0 } },
    { 2977, { 266, 2691, 2691, 0, 0, 0 } },
    { 2994, { 248, 248, 250, 250, 0, 0 } },
};
