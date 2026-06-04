#pragma once
#include "emit_core.h"
#include "emit_globals.h"
#include "emit_ir.h"
#include <stdio.h>
void bb_pat_any    (void);
void bb_pat_notany (void);
void bb_pat_span   (void);
void bb_pat_break  (void);
void bb_pat_pos    (void);
void bb_pat_tab    (void);
void bb_pat_atp    (void);
void bb_pat_len    (void);
void bb_pat_rem    (void);
void bb_pat_arb    (void);
void bb_pat_arbno(IR_t *pBB);
void bb_pat_abort  (void);
void bb_pat_fence  (void);
void bb_pat_alt    (void);
void bb_pat_cat    (void);
void bb_subject    (void);
void bb_match      (void);
void bb_pat_capture(void);
void bb_unify   (void);
void bb_arith   (void);
void bb_conj     (void);
void bb_disj     (void);
void bb_ite     (void);
void bb_catch   (void);
void bb_cut     (void);
void bb_goal     (IR_t * pBB);
void bb_builtin  (IR_t * pBB);
void bb_choice   (IR_t * pBB);
void bb_atom     (IR_t * pBB);
void bb_logicvar (IR_t * pBB);
void bb_fail     (IR_t * pBB);
void bb_every      (IR_t * pBB);
void bb_call       (IR_t * pBB);
void bb_iterate    (IR_t * pBB);
void bb_binop_relop      (IR_t * pBB);
void bb_binop_arith      (IR_t * pBB);
void bb_binop_gvar_arith (IR_t * pBB);
void bb_binop_gvar_relop (IR_t * pBB);
void bb_binop_gvar_arith_slot(IR_t * pBB);
void bb_binop_concat_slot(IR_t * pBB);
void bb_lit        (void);
void bb_lit_scalar (IR_t * pBB);
void bb_var        (IR_t * pBB);
void bb_var_global (IR_t * pBB);
void bb_return (IR_t * pBB);
void bb_unop       (void);
void bb_succeed    (void);
void bb_pat_defer  (void);
