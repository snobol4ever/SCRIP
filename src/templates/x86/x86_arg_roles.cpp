/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_arg_roles.cpp - the ONE definition of x86_argroles (declared `extern` in x86_arg_roles.h).
 *
 * WHY THIS FILE EXISTS: x86_arg_roles.h used to define this table `static const`, and every one
 * of the ~150 template TUs that include x86_asm.h (the mandated encoder header) got its own
 * private 6,832-byte copy - 123 entries x 7 pointers (callee + role[6]) = 861 R_X86_64_RELATIVE
 * relocations PER COPY, all of them written at every process start regardless of which template
 * that TU ever actually emits. 150 copies = 128,100 relocations = 96.1% of the runtime's ENTIRE
 * relocation set and 93% of .data.rel.ro (measured: FINDING-2026-08-28-hq_P-one-static-table-in-a-
 * header-is-96-percent-of-the-runtime-relocations.md). That 150->1 collapse landed 2026-08-28.
 *
 * WHY THE TABLE IS OFFSETS AND NOT POINTERS: collapsing to one copy left the LAST 861 relocations
 * -- one per `const char *` in the surviving copy -- because a pointer-to-string-literal cannot be
 * a link-time constant in a PIC object; it must be written at load time, which DIRTIES the pages
 * holding it and forces the table into .data.rel.ro (private, dirtied per process). Storing 16-bit
 * OFFSETS into one blob instead makes every field a true compile-time constant: the table and the
 * blob both become .rodata -- zero relocations, never written, shared across processes, and
 * demand-paged clean. 861 -> 0 relocations; 6,888 B of .data.rel.ro -> 4341 B of .rodata.
 *
 * GENERATED, NOT HAND-TRANSCRIBED (scratchpad gen_blob.py, seat01 2026-09-01): strings are copied
 * verbatim from the pointer table this replaces -- data identical, representation only.
 */
#include "x86_arg_roles.h"
const char x86_argrole_blob[] =
    "\0"  /* 0 */
    "NV_GET_fn\0"  /* 1 */
    "name\0"  /* 11 */
    "NV_SET_fn\0"  /* 16 */
    "val\0"  /* 26 */
    "POWER_fn\0"  /* 30 */
    "a\0"  /* 39 */
    "b\0"  /* 41 */
    "bb_build_break_blob\0"  /* 43 */
    "cset\0"  /* 63 */
    "bb_build_len_blob\0"  /* 68 */
    "I\0"  /* 86 */
    "dat_field_get\0"  /* 88 */
    "fname\0"  /* 102 */
    "obj\0"  /* 108 */
    "dtp_fn_of\0"  /* 112 */
    "headv\0"  /* 122 */
    "memcmp\0"  /* 128 */
    "s1\0"  /* 135 */
    "s2\0"  /* 138 */
    "n\0"  /* 141 */
    "mon_emit_label_bin\0"  /* 143 */
    "stno\0"  /* 162 */
    "putchar\0"  /* 167 */
    "c\0"  /* 175 */
    "rt_arg_stage\0"  /* 177 */
    "idx\0"  /* 190 */
    "v\0"  /* 194 */
    "rt_arith\0"  /* 196 */
    "lk\0"  /* 205 */
    "li\0"  /* 208 */
    "ls\0"  /* 211 */
    "rk\0"  /* 214 */
    "ri\0"  /* 217 */
    "rs\0"  /* 220 */
    "rt_assign_var\0"  /* 223 */
    "var\0"  /* 237 */
    "rt_at_cursor\0"  /* 241 */
    "varname\0"  /* 254 */
    "cur_delta\0"  /* 262 */
    "rt_binop_overload\0"  /* 272 */
    "op\0"  /* 290 */
    "out\0"  /* 293 */
    "rt_call_apply_gen_h\0"  /* 297 */
    "callee\0"  /* 317 */
    "lv\0"  /* 324 */
    "hslot\0"  /* 327 */
    "rt_call_apply_spine_prep\0"  /* 333 */
    "rt_call_arr\0"  /* 358 */
    "fn\0"  /* 370 */
    "args\0"  /* 373 */
    "nargs\0"  /* 378 */
    "rt_call_arr_gen\0"  /* 384 */
    "resume\0"  /* 400 */
    "rt_call_value_gen_h\0"  /* 407 */
    "argv\0"  /* 427 */
    "rt_call_value_resume_h\0"  /* 432 */
    "rt_call_value_spine_prep\0"  /* 455 */
    "rt_cap_finish\0"  /* 480 */
    "fret\0"  /* 494 */
    "rt_cap_open\0"  /* 499 */
    "saved_delta\0"  /* 511 */
    "is_imm\0"  /* 523 */
    "rt_cap_open_plain\0"  /* 530 */
    "rt_cap_pop\0"  /* 548 */
    "slot\0"  /* 559 */
    "rt_cap_push\0"  /* 564 */
    "delta\0"  /* 576 */
    "rt_cap_top\0"  /* 582 */
    "rt_case_eq\0"  /* 593 */
    "sel\0"  /* 604 */
    "key\0"  /* 608 */
    "rt_cmp_d\0"  /* 612 */
    "rt_coerce_int_d\0"  /* 621 */
    "in\0"  /* 637 */
    "codes\0"  /* 640 */
    "rt_coerce_num2_d\0"  /* 646 */
    "self\0"  /* 663 */
    "other\0"  /* 668 */
    "rt_coerce_real_d\0"  /* 674 */
    "rt_coerce_str_d\0"  /* 691 */
    "rt_cset_compl\0"  /* 707 */
    "rt_defer_close\0"  /* 721 */
    "rt_defer_get_pat_fn\0"  /* 736 */
    "ival_flag\0"  /* 756 */
    "rt_defer_open\0"  /* 766 */
    "rt_defer_step\0"  /* 780 */
    "rt_deref\0"  /* 794 */
    "d\0"  /* 803 */
    "rt_field_var\0"  /* 805 */
    "rt_frame_prep\0"  /* 818 */
    "fb\0"  /* 832 */
    "fbytes\0"  /* 835 */
    "rt_goto_transfer\0"  /* 842 */
    "rt_gvar_arith\0"  /* 859 */
    "rt_gvar_assign_descr\0"  /* 873 */
    "lo\0"  /* 894 */
    "hi\0"  /* 897 */
    "rt_gvar_assign_pat\0"  /* 900 */
    "head\0"  /* 919 */
    "rt_gvar_get_int\0"  /* 924 */
    "rt_indirect_assign_str\0"  /* 940 */
    "holder\0"  /* 963 */
    "str\0"  /* 970 */
    "rt_indirect_assign_var\0"  /* 974 */
    "val_name\0"  /* 997 */
    "rt_initial_fire\0"  /* 1006 */
    "site\0"  /* 1022 */
    "rt_is_truthy\0"  /* 1027 */
    "rt_jct_relop\0"  /* 1040 */
    "lhs\0"  /* 1053 */
    "rhs\0"  /* 1057 */
    "rt_keyword_dump_set\0"  /* 1061 */
    "rt_keyword_error_set\0"  /* 1081 */
    "rt_keyword_gen\0"  /* 1102 */
    "sval\0"  /* 1117 */
    "rt_keyword_pos_set\0"  /* 1122 */
    "rt_keyword_random_set\0"  /* 1141 */
    "rt_keyword_read\0"  /* 1163 */
    "rt_keyword_read_snobol4\0"  /* 1179 */
    "rt_keyword_subject_set\0"  /* 1203 */
    "rt_keyword_trace_set\0"  /* 1226 */
    "rt_list_bang_at\0"  /* 1247 */
    "rt_list_bang_key_at\0"  /* 1263 */
    "rt_list_bang_var_at\0"  /* 1283 */
    "rt_match_ctx_restore\0"  /* 1303 */
    "sig\0"  /* 1324 */
    "len\0"  /* 1328 */
    "capgen\0"  /* 1332 */
    "rt_match_enter\0"  /* 1339 */
    "rt_match_replace\0"  /* 1354 */
    "sub_lo\0"  /* 1371 */
    "sub_hi\0"  /* 1378 */
    "start\0"  /* 1385 */
    "end\0"  /* 1391 */
    "replp\0"  /* 1395 */
    "rt_match_value_get_pat_fn\0"  /* 1401 */
    "pval\0"  /* 1427 */
    "rt_match_value_open\0"  /* 1432 */
    "rt_num_arith\0"  /* 1452 */
    "rt_num_neg\0"  /* 1465 */
    "rt_num_pos\0"  /* 1476 */
    "rt_nv_cstr\0"  /* 1487 */
    "rt_nv_slen\0"  /* 1498 */
    "rt_pl_cells_init\0"  /* 1509 */
    "cells\0"  /* 1526 */
    "rt_pl_dc_prep\0"  /* 1532 */
    "suffix_off\0"  /* 1546 */
    "region_bytes\0"  /* 1557 */
    "np\0"  /* 1570 */
    "rt_pl_dop_unify_ci\0"  /* 1573 */
    "imm\0"  /* 1592 */
    "rt_pl_dop_unify_cs\0"  /* 1596 */
    "cs\0"  /* 1615 */
    "rt_pl_gz_init\0"  /* 1618 */
    "frame\0"  /* 1632 */
    "nslots\0"  /* 1638 */
    "rt_proc_call_epilogue_ret\0"  /* 1645 */
    "rt_proc_call_gen_h\0"  /* 1671 */
    "hout\0"  /* 1690 */
    "rt_proc_call_open\0"  /* 1695 */
    "rt_proc_call_open_det\0"  /* 1713 */
    "rt_proc_call_open_slim\0"  /* 1735 */
    "rt_proc_define\0"  /* 1758 */
    "spec\0"  /* 1773 */
    "rt_proc_resume_frame_h\0"  /* 1778 */
    "rt_random_var\0"  /* 1801 */
    "base\0"  /* 1815 */
    "rt_relop_overload\0"  /* 1820 */
    "rt_rev_swap_fwd\0"  /* 1838 */
    "lkind\0"  /* 1854 */
    "lp\0"  /* 1860 */
    "rkind\0"  /* 1863 */
    "rp\0"  /* 1869 */
    "save\0"  /* 1872 */
    "spill\0"  /* 1877 */
    "rt_rev_swap_undo\0"  /* 1883 */
    "rt_scan_enter\0"  /* 1900 */
    "sigma\0"  /* 1914 */
    "Delta\0"  /* 1920 */
    "rt_scan_leave\0"  /* 1926 */
    "out3\0"  /* 1940 */
    "rt_scan_lit\0"  /* 1945 */
    "subj_name\0"  /* 1957 */
    "subj_lit\0"  /* 1967 */
    "pat_lit\0"  /* 1976 */
    "is_repl\0"  /* 1984 */
    "repl_lit\0"  /* 1992 */
    "rt_scan_needle\0"  /* 2001 */
    "rt_scan_splice_empty\0"  /* 2016 */
    "m_start\0"  /* 2037 */
    "m_end\0"  /* 2045 */
    "rt_sg_member\0"  /* 2051 */
    "char\0"  /* 2064 */
    "needle_ptr\0"  /* 2069 */
    "needle_len\0"  /* 2080 */
    "rt_sg_scan_member\0"  /* 2091 */
    "start_pos\0"  /* 2109 */
    "rt_sg_scan_nonmember\0"  /* 2119 */
    "rt_size_d\0"  /* 2140 */
    "rt_str_coerce\0"  /* 2150 */
    "rt_subject_load_lit\0"  /* 2164 */
    "s\0"  /* 2184 */
    "rt_subject_load_nv\0"  /* 2186 */
    "rt_subscript_var\0"  /* 2205 */
    "rt_substr\0"  /* 2222 */
    "rt_swap_var\0"  /* 2232 */
    "va\0"  /* 2244 */
    "vb\0"  /* 2247 */
    "rt_trail_unwind\0"  /* 2250 */
    "mark\0"  /* 2266 */
    "rt_write_any_nl\0"  /* 2271 */
    "rt_write_int_nl\0"  /* 2287 */
    "rt_zcol_push\0"  /* 2303 */
    "ptr_cell\0"  /* 2316 */
    "cap_cell\0"  /* 2325 */
    "i\0"  /* 2334 */
    "elem_sz\0"  /* 2336 */
    "rt_zls2_release_to\0"  /* 2344 */
    "rt_zls_release_to\0"  /* 2363 */
    "scrip_coexpr_activate\0"  /* 2381 */
    "target\0"  /* 2403 */
    "x0\0"  /* 2410 */
    "x1\0"  /* 2413 */
    "out2\0"  /* 2416 */
    "scrip_coexpr_create\0"  /* 2421 */
    "body_entry_addr\0"  /* 2441 */
    "regs\0"  /* 2457 */
    "frame_bytes\0"  /* 2462 */
    "scrip_coret\0"  /* 2474 */
    "d0\0"  /* 2486 */
    "d1\0"  /* 2489 */
    "resume_addr\0"  /* 2492 */
    "str_concat_d\0"  /* 2504 */
    "str_repeat_d\0"  /* 2517 */
    "strchr\0"  /* 2530 */
    "strlen\0"  /* 2537 */
    "subscript_get\0"  /* 2544 */
    "arr\0"  /* 2558 */
    "subscript_get2\0"  /* 2562 */
    "j\0"  /* 2577 */
    "subscript_get2_ext\0"  /* 2579 */
    "subscript_set\0"  /* 2598 */
    "to_int\0"  /* 2612 */;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const x86_argrole_t x86_argroles[123] = {
    {     1, {    11,     0,     0,     0,     0,     0 } },  /* NV_GET_fn */
    {    16, {    11,    26,    26,     0,     0,     0 } },  /* NV_SET_fn */
    {    30, {    39,    39,    41,    41,     0,     0 } },  /* POWER_fn */
    {    43, {    11,    63,     0,     0,     0,     0 } },  /* bb_build_break_blob */
    {    68, {    11,    86,     0,     0,     0,     0 } },  /* bb_build_len_blob */
    {    88, {   102,   108,   108,     0,     0,     0 } },  /* dat_field_get */
    {   112, {   122,     0,     0,     0,     0,     0 } },  /* dtp_fn_of */
    {   128, {   135,   138,   141,     0,     0,     0 } },  /* memcmp */
    {   143, {   162,     0,     0,     0,     0,     0 } },  /* mon_emit_label_bin */
    {   167, {   175,     0,     0,     0,     0,     0 } },  /* putchar */
    {   177, {   190,   194,   194,     0,     0,     0 } },  /* rt_arg_stage */
    {   196, {   205,   208,   211,   214,   217,   220 } },  /* rt_arith */
    {   223, {   237,   237,    26,    26,     0,     0 } },  /* rt_assign_var */
    {   241, {   254,   262,     0,     0,     0,     0 } },  /* rt_at_cursor */
    {   272, {    39,    39,    41,    41,   290,   293 } },  /* rt_binop_overload */
    {   297, {   317,   317,   324,   324,   327,     0 } },  /* rt_call_apply_gen_h */
    {   333, {   317,   317,   324,   324,     0,     0 } },  /* rt_call_apply_spine_prep */
    {   358, {   370,   373,   378,     0,     0,     0 } },  /* rt_call_arr */
    {   384, {   370,   373,   378,   400,     0,     0 } },  /* rt_call_arr_gen */
    {   407, {   317,   317,   427,   141,   327,     0 } },  /* rt_call_value_gen_h */
    {   432, {   327,     0,     0,     0,     0,     0 } },  /* rt_call_value_resume_h */
    {   455, {   317,   317,   427,   141,     0,     0 } },  /* rt_call_value_spine_prep */
    {   480, {   494,   494,     0,     0,     0,     0 } },  /* rt_cap_finish */
    {   499, {   254,   511,   262,   523,     0,     0 } },  /* rt_cap_open */
    {   530, {   254,   511,   262,   523,     0,     0 } },  /* rt_cap_open_plain */
    {   548, {   559,     0,     0,     0,     0,     0 } },  /* rt_cap_pop */
    {   564, {   559,   576,     0,     0,     0,     0 } },  /* rt_cap_push */
    {   582, {   559,     0,     0,     0,     0,     0 } },  /* rt_cap_top */
    {   593, {   604,   608,     0,     0,     0,     0 } },  /* rt_case_eq */
    {   612, {    39,    41,     0,     0,     0,     0 } },  /* rt_cmp_d */
    {   621, {   637,   293,   640,     0,     0,     0 } },  /* rt_coerce_int_d */
    {   646, {   663,   668,   293,   640,     0,     0 } },  /* rt_coerce_num2_d */
    {   674, {   637,   293,   640,     0,     0,     0 } },  /* rt_coerce_real_d */
    {   691, {   637,   293,   640,     0,     0,     0 } },  /* rt_coerce_str_d */
    {   707, {    39,    39,     0,     0,     0,     0 } },  /* rt_cset_compl */
    {   721, {   262,     0,     0,     0,     0,     0 } },  /* rt_defer_close */
    {   736, {   254,   756,     0,     0,     0,     0 } },  /* rt_defer_get_pat_fn */
    {   766, {   254,   756,     0,     0,     0,     0 } },  /* rt_defer_open */
    {   780, {   494,   494,     0,     0,     0,     0 } },  /* rt_defer_step */
    {   794, {   803,   803,     0,     0,     0,     0 } },  /* rt_deref */
    {   805, {   102,   108,   108,     0,     0,     0 } },  /* rt_field_var */
    {   818, {   832,   835,     0,     0,     0,     0 } },  /* rt_frame_prep */
    {   842, {    11,     0,     0,     0,     0,     0 } },  /* rt_goto_transfer */
    {   859, {    39,    41,   290,     0,     0,     0 } },  /* rt_gvar_arith */
    {   873, {    11,   894,   897,     0,     0,     0 } },  /* rt_gvar_assign_descr */
    {   900, {    11,   919,     0,     0,     0,     0 } },  /* rt_gvar_assign_pat */
    {   924, {    11,     0,     0,     0,     0,     0 } },  /* rt_gvar_get_int */
    {   940, {   963,   970,     0,     0,     0,     0 } },  /* rt_indirect_assign_str */
    {   974, {   963,   997,     0,     0,     0,     0 } },  /* rt_indirect_assign_var */
    {  1006, {  1022,     0,     0,     0,     0,     0 } },  /* rt_initial_fire */
    {  1027, {   194,   194,     0,     0,     0,     0 } },  /* rt_is_truthy */
    {  1040, {  1053,  1053,  1057,  1057,   290,     0 } },  /* rt_jct_relop */
    {  1061, {   194,   194,     0,     0,     0,     0 } },  /* rt_keyword_dump_set */
    {  1081, {   194,   194,     0,     0,     0,     0 } },  /* rt_keyword_error_set */
    {  1102, {  1117,   190,     0,     0,     0,     0 } },  /* rt_keyword_gen */
    {  1122, {   194,   194,     0,     0,     0,     0 } },  /* rt_keyword_pos_set */
    {  1141, {   194,   194,     0,     0,     0,     0 } },  /* rt_keyword_random_set */
    {  1163, {  1117,     0,     0,     0,     0,     0 } },  /* rt_keyword_read */
    {  1179, {  1117,     0,     0,     0,     0,     0 } },  /* rt_keyword_read_snobol4 */
    {  1203, {   894,   897,     0,     0,     0,     0 } },  /* rt_keyword_subject_set */
    {  1226, {   194,   194,     0,     0,     0,     0 } },  /* rt_keyword_trace_set */
    {  1247, {   108,   108,   190,     0,     0,     0 } },  /* rt_list_bang_at */
    {  1263, {   108,   108,   190,     0,     0,     0 } },  /* rt_list_bang_key_at */
    {  1283, {   108,   108,   190,     0,     0,     0 } },  /* rt_list_bang_var_at */
    {  1303, {  1324,  1328,  1332,     0,     0,     0 } },  /* rt_match_ctx_restore */
    {  1339, {   894,   897,     0,     0,     0,     0 } },  /* rt_match_enter */
    {  1354, {    11,  1371,  1378,  1385,  1391,  1395 } },  /* rt_match_replace */
    {  1401, {  1427,     0,     0,     0,     0,     0 } },  /* rt_match_value_get_pat_fn */
    {  1432, {  1427,     0,     0,     0,     0,     0 } },  /* rt_match_value_open */
    {  1452, {    39,    39,    41,    41,   290,     0 } },  /* rt_num_arith */
    {  1465, {    39,    39,     0,     0,     0,     0 } },  /* rt_num_neg */
    {  1476, {    39,    39,     0,     0,     0,     0 } },  /* rt_num_pos */
    {  1487, {    11,     0,     0,     0,     0,     0 } },  /* rt_nv_cstr */
    {  1498, {    11,     0,     0,     0,     0,     0 } },  /* rt_nv_slen */
    {  1509, {  1526,   141,     0,     0,     0,     0 } },  /* rt_pl_cells_init */
    {  1532, {   832,  1546,  1557,  1570,   378,   190 } },  /* rt_pl_dc_prep */
    {  1573, {   373,  1592,     0,     0,     0,     0 } },  /* rt_pl_dop_unify_ci */
    {  1596, {   373,  1615,     0,     0,     0,     0 } },  /* rt_pl_dop_unify_cs */
    {  1618, {  1632,  1638,     0,     0,     0,     0 } },  /* rt_pl_gz_init */
    {  1645, {   494,   494,     0,     0,     0,     0 } },  /* rt_proc_call_epilogue_ret */
    {  1671, {    11,   378,  1690,     0,     0,     0 } },  /* rt_proc_call_gen_h */
    {  1695, {    11,   378,     0,     0,     0,     0 } },  /* rt_proc_call_open */
    {  1713, {   190,   378,     0,     0,     0,     0 } },  /* rt_proc_call_open_det */
    {  1735, {    11,  1570,   378,     0,     0,     0 } },  /* rt_proc_call_open_slim */
    {  1758, {  1773,     0,     0,     0,     0,     0 } },  /* rt_proc_define */
    {  1778, {   327,     0,     0,     0,     0,     0 } },  /* rt_proc_resume_frame_h */
    {  1801, {  1815,  1815,     0,     0,     0,     0 } },  /* rt_random_var */
    {  1820, {    39,    39,    41,    41,   290,   293 } },  /* rt_relop_overload */
    {  1838, {  1854,  1860,  1863,  1869,  1872,  1877 } },  /* rt_rev_swap_fwd */
    {  1883, {  1854,  1860,  1863,  1869,  1872,  1877 } },  /* rt_rev_swap_undo */
    {  1900, {   894,   897,  1914,   576,  1920,     0 } },  /* rt_scan_enter */
    {  1926, {  1940,     0,     0,     0,     0,     0 } },  /* rt_scan_leave */
    {  1945, {  1957,  1967,  1976,  1984,  1992,     0 } },  /* rt_scan_lit */
    {  2001, {   894,   897,     0,     0,     0,     0 } },  /* rt_scan_needle */
    {  2016, {  1957,  2037,  2045,     0,     0,     0 } },  /* rt_scan_splice_empty */
    {  2051, {  2064,  2069,  2080,     0,     0,     0 } },  /* rt_sg_member */
    {  2091, {  2109,  2069,  2080,     0,     0,     0 } },  /* rt_sg_scan_member */
    {  2119, {  2109,  2069,  2080,     0,     0,     0 } },  /* rt_sg_scan_nonmember */
    {  2140, {   894,   897,     0,     0,     0,     0 } },  /* rt_size_d */
    {  2150, {   803,   803,     0,     0,     0,     0 } },  /* rt_str_coerce */
    {  2164, {  2184,   559,     0,     0,     0,     0 } },  /* rt_subject_load_lit */
    {  2186, {    11,   559,     0,     0,     0,     0 } },  /* rt_subject_load_nv */
    {  2205, {  1815,  1815,   190,   190,     0,     0 } },  /* rt_subscript_var */
    {  2222, {  1914,    39,    41,     0,     0,     0 } },  /* rt_substr */
    {  2232, {  2244,  2244,  2247,  2247,     0,     0 } },  /* rt_swap_var */
    {  2250, {  2266,     0,     0,     0,     0,     0 } },  /* rt_trail_unwind */
    {  2271, {   803,   803,     0,     0,     0,     0 } },  /* rt_write_any_nl */
    {  2287, {   194,     0,     0,     0,     0,     0 } },  /* rt_write_int_nl */
    {  2303, {  2316,  2325,  2334,  2336,     0,     0 } },  /* rt_zcol_push */
    {  2344, {  2266,     0,     0,     0,     0,     0 } },  /* rt_zls2_release_to */
    {  2363, {  2266,     0,     0,     0,     0,     0 } },  /* rt_zls_release_to */
    {  2381, {  2403,  2410,  2413,  2416,     0,     0 } },  /* scrip_coexpr_activate */
    {  2421, {  2441,  2457,  2462,     0,     0,     0 } },  /* scrip_coexpr_create */
    {  2474, {  2486,  2489,  2492,     0,     0,     0 } },  /* scrip_coret */
    {  2504, {    39,    39,    41,    41,     0,     0 } },  /* str_concat_d */
    {  2517, {  2184,  2184,   141,   141,     0,     0 } },  /* str_repeat_d */
    {  2530, {  2184,   175,     0,     0,     0,     0 } },  /* strchr */
    {  2537, {  2184,     0,     0,     0,     0,     0 } },  /* strlen */
    {  2544, {  2558,  2558,   190,   190,     0,     0 } },  /* subscript_get */
    {  2562, {  2558,  2558,  2334,  2334,  2577,  2577 } },  /* subscript_get2 */
    {  2579, {  2558,  2558,  2334,  2334,  1391,  1391 } },  /* subscript_get2_ext */
    {  2598, {  2558,  2558,   190,   190,    26,    26 } },  /* subscript_set */
    {  2612, {   194,   194,     0,     0,     0,     0 } },  /* to_int */
};
