/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifdef __cplusplus
#include <string>
#include "IR.h"
enum { BB_SCC_NP_MAX = 60, BB_SHIM_ID_ALPHA = 10, BB_SHIM_ID_BETA = 80, BB_SHIM_ID_OMEGA = 150, BB_SHIM_ID_FIXED = 230 };
enum { BB_SHIM_TRACE_IDS = 13, BB_SHIM_TRACE_NF_MAX = 39 };
static_assert(BB_SHIM_ID_ALPHA + BB_SCC_NP_MAX < BB_SHIM_ID_BETA - 5 && BB_SHIM_ID_BETA + BB_SCC_NP_MAX < BB_SHIM_ID_OMEGA - 5
              && BB_SHIM_ID_OMEGA + BB_SCC_NP_MAX < BB_SHIM_ID_FIXED,
              "THE ROLE-4 DEFINE SHIM ADMITS EVERY DEFINE THE SCC PROBE ADMITS (bb_scc_probe: np <= BB_SCC_NP_MAX): its three per-formal "
              "chains (alpha swap, beta restore, omega restore) each take ONE one-byte internal label id per formal plus one join, from "
              "its own base, below the next base's lid-5 join and the fixed ids 230..249 (X86_INTERNAL_MAX 250); the branch is monotone "
              "in the argument count, so every formal past the arguments falls through the ext chain (it spent two ids per formal and "
              "stopped at 29 formals until 2026-09-27)");
static_assert(BB_SHIM_ID_OMEGA + BB_SHIM_TRACE_NF_MAX + 2 + 3 * BB_SHIM_TRACE_IDS <= BB_SHIM_ID_FIXED,
              "THE ROLE-4 DEFINE SHIM'S THREE TRACE-HANDLER GLUES (call, return, failing return; bb_glue_trace_pend_run in_shim) each take a compact block of "
              "BB_SHIM_TRACE_IDS one-byte internal label ids, placed after the omega chain (OMEGA + nformals + 2), below the fixed ids 230..249; a traced "
              "DEFINE with more than BB_SHIM_TRACE_NF_MAX formals keeps the C-entered hook (rt_trace_*_hook_i through the r12 island bridge: a dyn-scope procedure runs only "
              "through its role-4 shim, so the shim cannot be refused, and the id space is one byte)");
extern "C" { void bb_pattern_stub(const char * which); }
extern "C++" {
std::string bb_match_any();
std::string bb_match_notany();
std::string bb_match_span();
std::string bb_match_break();
std::string bb_match_breakx();
std::string bb_match_rtab();
std::string bb_match_lambda();
std::string bb_match_pos();
std::string bb_match_rpos();
std::string bb_match_tab();
std::string bb_coerce_string();
std::string bb_coerce_numeric();
std::string bb_cmp_test();
std::string bb_ident();
std::string bb_differ();
std::string bb_coerce_integer();
std::string bb_coerce_real();
std::string bb_match_atp();
std::string bb_match_len();
std::string bb_match_rem();
std::string bb_match_arb();
std::string bb_match_bal();
std::string bb_match_arbno();
std::string bb_match_abort();
std::string bb_match_fence0();
std::string bb_match_flush();
std::string bb_match_fence1();
std::string bb_match_alternate();
std::string bb_scan_sequence();
std::string bb_scan_alternate();
std::string bb_match_begin();
std::string bb_match_end();
std::string bb_match_replace();
std::string bb_match_capture();
std::string bb_conjunction();
std::string bb_subscript();
std::string bb_subscript_lvck();
std::string bb_subscript2();
std::string bb_deref();
std::string bb_unify_const();
std::string bb_unify_struct();
std::string bb_unify_first();
std::string bb_unify_value();
std::string bb_random();
std::string bb_var_ref();
std::string bb_var_ref_frame();
std::string bb_assign_var();
std::string bb_assign_var_sub();
std::string bb_rev_assign_var();
std::string bb_goto();
std::string bb_bound();
std::string bb_statement();
std::string bb_stmt_mark(long stno, long line);
std::string bb_setexit_test();
std::string bb_cleanup();
std::string bb_setexit_take(int land_id, long drop);
std::string bb_line_mark(long line, const char * file);
std::string bb_glue_flat_enter();
std::string bb_glue_flat_leave();
std::string bb_glue_framed_enter();
std::string bb_glue_framed_leave();
std::string bb_glue_outer_γ();
std::string bb_glue_outer_ω();
std::string bb_main_entry_bridge();
std::string bb_main_β();
std::string bb_main_floater(int kind);
std::string bb_glue_wire_exit(int is_gamma);
std::string bb_glue_wire_γ();
std::string bb_glue_wire_ω();
std::string bb_glue_pass_wires(int gid, int wid);
std::string bb_glue_wire_land(void);
std::string bb_glue_pass_wires_blob(int gid, int wid);
std::string bb_glue_pass_wires_blob_regs(int gid, int wid);
std::string bb_glue_enter_c2bb(int base, int lg, int lw, int hi = -1);
std::string bb_glue_callee_try_enter(int base, int val_id, int join_id);
std::string bb_glue_apply_try_enter(int base, int val_id, int join_id, int in_shim = 0);
std::string bb_glue_trace_pend_run(int base, const std::string & after, int in_shim = 0);
std::string bb_glue_try_enter(const char * try_sym, uint64_t try_fp, const char * lg_sym, uint64_t lg_fp, const char * lw_sym, uint64_t lw_fp, int base, int val_id, int join_id, int stno = 0,
    int in_shim = 0);
std::string bb_glue_stno_unit_push(void);
std::string bb_glue_enter_chain_ret(int lid);
std::string bb_glue_name_or_rec_lea(const char * nm);
std::string bb_glue_prim_int(int base);
std::string bb_glue_prim_member(int base, int code);
std::string bb_glue_prim_str(int base, int ptr_d, int ptr_sd, int len_d, int len_sd, int code);
std::string bb_glue_prim_str_rsp(int base, int ptr_off, int len_off, int code);
std::string bb_disjunction();
std::string bb_cut();
std::string bb_fail();
std::string bb_call(IR_t * pBB);
std::string bb_iterate();
std::string bb_binop_relop();
std::string bb_binop_relop_val();
std::string bb_binop_arith();
std::string bb_binop_concat_slot();
std::string bb_binop_xrep_slot();
std::string bb_lit_scalar();
std::string bb_var();
std::string bb_var_global();
std::string bb_return();
std::string bb_unop();
std::string bb_succeed();
std::string bb_match_defer();
std::string bb_match_value();
std::string bb_keyword_icon();
std::string bb_keyword_snobol4();
std::string bb_keyword_assign();
std::string bb_keyword_assign_snobol4();
std::string bb_goto_deferred();
std::string bb_define();
std::string bb_nreturn_mark();
extern "C" void * bb_ab_fn_cell_ptr(const char * fname);
std::string bb_gen_scan();
std::string bb_assign_local();
std::string bb_assign_global();
std::string bb_field_get();
std::string bb_section();
std::string bb_swap();
std::string bb_swap_var();
std::string bb_proc_value();
std::string bb_call_value();
std::string bb_rev_assign();
std::string bb_rev_assign_global();
std::string bb_rev_swap();
std::string bb_assign_frame();
std::string bb_var_frame();
std::string bb_to();
std::string bb_match_len();
std::string bb_match_lit();
std::string bb_match_rkrun();
std::string bb_match_rkcaps();
std::string bb_match_rkcape();
std::string bb_match_rkland();
std::string bb_to_by();
std::string bb_make_list();
std::string bb_limit();
std::string bb_suspend();
std::string bb_enter_init();
std::string bb_activate();
std::string bb_create();
std::string bb_coret();
std::string bb_cofail();
std::string bb_gate_arm();
std::string bb_gate();
std::string xa_coexpr_body_lea(const char * dst);
std::string bb_limit_init();
std::string bb_limit_gate();
std::string bb_repalt_clear();
std::string bb_repalt_yield();
std::string bb_repalt_test();
std::string bb_scan_pos();
std::string bb_scan_any();
std::string bb_scan_match();
std::string bb_scan_many();
std::string bb_scan_tab();
std::string bb_scan_move();
std::string bb_scan_upto();
std::string bb_scan_find();
std::string bb_scan_bal();
}
#endif
