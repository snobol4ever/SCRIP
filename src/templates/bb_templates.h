/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifdef __cplusplus
#include <string>
#include "IR.h"
extern "C" { void bb_pattern_stub(const char * which); }
extern "C++" {
std::string bb_call_define();
std::string bb_match_any();
std::string bb_match_notany();
std::string bb_match_span();
std::string bb_match_break();
std::string bb_match_breakx();
std::string bb_match_rtab();
std::string bb_match_span_var();
std::string bb_indirect_assign_lit_s();
std::string bb_indirect_assign_var();
std::string bb_match_pos();
std::string bb_match_rpos();
std::string bb_match_tab();
std::string bb_coerce_string();
std::string bb_coerce_numeric();
std::string bb_cmp_test();
std::string bb_coerce_integer();
std::string bb_coerce_real();
std::string bb_match_atp();
std::string bb_match_len();
std::string bb_match_rem();
std::string bb_match_arb();
std::string bb_match_bal();
std::string bb_match_arbno();
std::string bb_match_abort();
std::string bb_match_fence1();
std::string bb_match_alternate();


std::string bb_scan_sequence();
std::string bb_scan_alternate();
std::string bb_match_cat();
std::string bb_subject();
std::string bb_match_begin();
std::string bb_match_end();
std::string bb_match_replace();
std::string bb_match_retry();
std::string bb_match_advance();
std::string bb_match_capture();
std::string bb_arith();
std::string bb_conjunction();
std::string bb_subscript();
std::string bb_deref();
std::string bb_random();
std::string bb_var_ref();
std::string bb_assign_var();
std::string bb_rev_assign_var();
std::string bb_goto();
std::string bb_bound();
std::string bb_statement();
std::string bb_glue_flat_enter();     /* GLUE-1 (Lon s21x-n): per-BB storage bracket, no frame pointer -- the ~99.999% case (s21x-c law 3). */
std::string bb_glue_flat_leave();
std::string bb_glue_framed_enter();   /* GLUE-2 (Lon s21x-n): same + rbp activation base; ONLY the four RBP constructs (STATEMENT/FUNCTION/ARBNO/FENCE1, s21x-c law 4).  Wiring this wires the prologue -- s21x-m law 3. */
std::string bb_glue_framed_leave();
std::string bb_glue_outer_γ();
std::string bb_glue_outer_ω();
std::string bb_glue_wire_exit(int is_gamma);   /* WIRE-EXIT (s22v): snap the open pcall record, restore caller rsp/rbp(/r12 island) from it, jmp the port's wire -- gw for γ (RETURN), ww for ω (FRETURN).  ONE authority; the role-1/2 floaters and the stub-blob shared ports both consume it. */
std::string bb_glue_wire_γ();
std::string bb_glue_wire_ω();
std::string bb_glue_pass_wires(int gid, int wid);   /* PASS-THROUGH (s22v): the bare wire contract -- lea rcx,<L(gid)>; lea rdx,<L(wid)>; jmp rax.  One-shot = this + the pcall record; pass-through = this alone. */
int wreg_on(void);   /* ⭐ LADDER WREG killswitch (SCRIP_WREG=1 opt-in): 1 = γ/ω ride r10/r11 into PAT$ blobs, blob α shim + CLASS D exit ceremony suppressed; 0 = byte-identical legacy rcx/rdx + carve + g_zctx. */
std::string bb_glue_pass_wires_blob(int gid, int wid);   /* ⭐ LADDER WREG blob-entry glue: lea r10,<L(gid)>; lea r11,<L(wid)>; jmp rax.  Falls back to bb_glue_pass_wires verbatim when the switch is off. */
std::string bb_disjunction();
std::string bb_cut();
std::string bb_fail();
std::string bb_every();
std::string bb_call(IR_t * pBB);
std::string bb_iterate();
std::string bb_key_gen(IR_t * pBB);
std::string bb_binop_relop();
std::string bb_binop_relop_val();
std::string bb_binop_arith();
std::string bb_binop_gvar_arith();
std::string bb_binop_gvar_arith_slot();
std::string bb_binop_concat_slot();
std::string bb_binop_xrep_slot();
std::string bb_lit();
std::string bb_lit_scalar();
std::string bb_var();
std::string bb_var_global();
std::string bb_return();
std::string bb_unop();
std::string bb_unop_gvar_slot();
std::string bb_succeed();
std::string bb_match_defer();
std::string bb_match_value();
std::string bb_pattern_len();
std::string bb_pattern_break();
std::string bb_pattern_capture();
std::string bb_pattern_cat();
std::string bb_ref_invariant();
std::string bb_pattern_lit();
std::string bb_keyword_icon();
std::string bb_keyword_snobol4();
std::string bb_keyword_assign();
std::string bb_goto_dyn();
std::string bb_save_restore();
std::string bb_func_activate();   /* LADDER AB (2026-08-08): per-DEFINE activation block */
std::string bb_ab_bind();   /* AB-3a: role-2 DEFINE residual bind — fn_cell$<FN> <- &<FN>_act_α */
void * bb_ab_fn_cell_ptr(const char * fname);   /* AB-3b: fn_cell$<FN> binary slot pointer for the call-site indirect jmp; TEXT uses the label string directly */
std::string bb_scan_stmt();
std::string bb_scan_splice_empty();
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
std::string bb_rev_swap();
std::string bb_case_arm();
std::string bb_var_frame();
std::string bb_var_frame_ref();
std::string bb_to();
std::string bb_match_len();   /* SN4-PAT: SNOBOL4 LEN(n) matcher */
std::string bb_match_lit();   /* SN4-PAT: SNOBOL4 literal-string matcher */
std::string bb_to_by();
std::string bb_make_list();
std::string bb_limit();
std::string bb_suspend();
std::string bb_enter_init();
std::string bb_activate();
std::string bb_create();
std::string bb_coret();
std::string bb_cofail();
std::string bb_move_label();
std::string bb_indirect_goto();
std::string xa_coexpr_body_lea(const char * dst);
std::string bb_initial();
std::string bb_limit_init();
std::string bb_repalt_clear();
std::string bb_repalt_yield();
std::string bb_repalt_test();
std::string bb_idx_set();
std::string bb_idx_get();
std::string bb_scan_pos();
std::string bb_scan_any();
std::string bb_scan_match();
std::string bb_rk_glit();
std::string bb_rk_gcc();
std::string bb_rk_galt();
std::string bb_scan_many();
std::string bb_scan_tab();
std::string bb_scan_move();
std::string bb_scan_upto();
std::string bb_scan_find();
std::string bb_scan_bal();
std::string bb_alt();
std::string bb_gather();
std::string bb_mapgrep();
std::string bb_query_frame();
std::string bb_det_nl();
std::string bb_callee_frame();
std::string bb_cell_cut();
std::string bb_cell_ite();
}
#endif
