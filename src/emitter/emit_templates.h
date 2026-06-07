#ifndef RUNTIME_X86_TEMPLATES_EMIT_TEMPLATES_H
#define RUNTIME_X86_TEMPLATES_EMIT_TEMPLATES_H
#include "core.h"
#include "emit.h"
void emit_sm_halt           ();
void emit_sm_push_lit_i     (int64_t val);
void emit_sm_push_lit_s     (const char * str_lbl, uint64_t str_ptr, int len);
void emit_sm_push_var       (const char * name_lbl, uint64_t name_ptr);
void emit_sm_store_var      (const char * name_lbl, uint64_t name_ptr);
void emit_sm_push_expression(uint64_t entry_ptr, int arity);
void emit_sm_call_expression(const char * tgt_sym);
void emit_sm_exec_stmt      (const char * subj_lbl, uint64_t subj_ptr, int has_repl);
void emit_sm_call_fn        (const char * name_lbl, uint64_t name_ptr, int nargs);
void emit_sm_define         ();         void emit_sm_define_entry   ();
void emit_sm_jump           (int target_pc);
void emit_sm_jump_s         (int target_pc);
void emit_sm_jump_f         (int target_pc);
void emit_sm_label          ();
void emit_sm_return         ();
void emit_sm_return_variant (int kind, int cond, int pc);
void emit_sm_freturn        (int pc);   void emit_sm_nreturn        (int pc);
void emit_sm_return_s       (int pc);   void emit_sm_return_f       (int pc);
void emit_sm_freturn_s      (int pc);   void emit_sm_freturn_f      (int pc);
void emit_sm_nreturn_s      (int pc);   void emit_sm_nreturn_f      (int pc);
void emit_sm_suspend        ();         void emit_sm_suspend_value  ();
void emit_sm_bb_pump        ();         void emit_sm_bb_once        ();
void emit_sm_bb_pump_case   ();         void emit_sm_bb_pump_sm     ();
void emit_sm_bb_pump_every  ();         void emit_sm_bb_pump_ast    ();
void emit_sm_load_glocal    ();         void emit_sm_store_glocal   ();
void emit_sm_icmp_gt        ();         void emit_sm_icmp_lt        ();
void emit_sm_load_frame     ();         void emit_sm_store_frame    ();
void emit_sm_pat_lit        (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_refname    (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_usercall   (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_capture        (const char * name_lbl, uint64_t name_ptr, int kind);
void emit_sm_pat_usercall_args  (const char * name_lbl, uint64_t name_ptr, int nargs);
void emit_sm_pat_capture_fn     (const char * fname_lbl, uint64_t fname_ptr, int is_imm,
                                  const char * namelist_lbl, uint64_t namelist_ptr);
void emit_sm_pat_capture_fn_args(const char * fname_lbl, uint64_t fname_ptr, int is_imm, int nargs);
#include "emit_bb.h"
#endif
