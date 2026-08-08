/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_H
#define RT_H
#include <stdint.h>
struct IR_graph_t;
#ifndef DESCR_T_DEFINED
#define DESCR_T_DEFINED
typedef struct DESCR_t DESCR_t;
#endif
#ifdef __cplusplus
extern "C" {
#endif
void rt_init(int argc, char **argv);
int  rt_finalize(void);
void    rt_unhandled_op(int op);
void    rt_bomb(const char *msg);
void rt_nv_get(const char *name);
void rt_nv_set(const char *name);
int  rt_case_eq(const DESCR_t *sel, const DESCR_t *key);
void rt_frame_enter(int nparams);
void rt_frame_leave(void);
void rt_load_frame(int slot);
void rt_store_frame(int slot);
int  rt_last_ok(void);
void rt_set_last_ok(int ok);
void rt_exec_stmt_pat(void *blob_α, const char *subj_name, int has_repl);
void rt_match_blob(void *blob_α,
                         const char *subj_name,
                         int has_repl);
void rt_match_variant(const char *subj_name, int has_repl);
void rt_concat(void);
void rt_set_stno(int64_t stno);
void rt_coerce_num(void);
void rt_exp(void);
void rt_neg(void);
void rt_incr(int64_t n);
void rt_decr(int64_t n);
void rt_acomp(int op);
void rt_lcomp(int op);
void rt_define_entry(void);
void rt_define(void);
void rt_unhandled_sm(int op);
void rt_call(const char *name, int nargs);
int rt_do_return(int kind, int cond);
int rt_do_nreturn(const char *fname, int cond);
typedef struct {
    const char *name;
    void       *fn;
} rt_expression_entry;
void rt_register_expressions(const rt_expression_entry *tbl);
void rt_write_atom(const char *s);
void rt_cut_set(void);
void rt_write_var(int slot);
void rt_write_term_ptr(void *t);
void rt_writeq_term_ptr(void *t);
void rt_write_canonical_term_ptr(void *t);
void rt_env_alloc(int nslots);
void rt_main_init(void);
struct Term;
void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
int   rt_unify_terms(void *l, void *r);
int   rt_trail_mark(void);
void  rt_trail_unwind(int mark);
void  rt_trail_mark_push(void);
void  rt_trail_unwind_top(void);
void  rt_trail_mark_pop(void);
void *rt_e_bump(int nbytes);
int   rt_e_mark(void);
void  rt_e_reset(int off);
void  rt_cp_save_caller_env(void *caller_env);
void  rt_choice_cut_enter(void *cp);
void  rt_choice_cut_exit(void *cp);
void  rt_choice_cut_unwind(void *cp);
int   rt_get_cut_flag(void);
long rt_arith(int lk, long li, const char *ls,
                  int rk, long ri, const char *rs, const char *op);
void rt_init_arbno(void **slot_ptr, void *child_fn);
typedef struct {
    void (*push)       (const DESCR_t *d);
    void (*pop)        (DESCR_t *out);
    void (*peek)       (DESCR_t *out);
    int  (*depth)      (void);
    void (*set_depth)  (int n);
    int  (*get_last_ok)(void);
    void (*set_last_ok)(int x);
} rt_vstack_ops_t;
#include "bb_box.h"
void rt_proc_register(const char *name, const char **pnames, int nparams);
void rt_proc_set_fn(const char *name, bb_box_fn fn);
void rt_proc_reset(void);
void rt_call_proc(const char *name, int nargs);
DESCR_t rt_call_proc_descr(const char *name, int nargs);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout);
DESCR_t rt_proc_resume_frame(void *frame);
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
DESCR_t rt_call_named_proc_sl(const char *name, DESCR_t *args, int nargs, void *sl);
DESCR_t rt_call_proc_direct(long idx, DESCR_t *args, int nargs);
int rt_proc_index_of(const char *name);
void rt_proc_table_fill(int64_t *tab, const char **names, int n);
void rt_proc_set_frame(const char *name, int nslots, int decl_level);
void rt_proc_set_frame_bytes(const char *name, int bytes);
void rt_proc_set_zstatic(const char *name, int bit);
long rt_fn_zstatic_known(void *fn);
int  rt_proc_frame_nslots(const char *name);
int  rt_proc_decl_level(const char *name);
void rt_proc_set_byref(const char *name, uint64_t mask);
uint64_t rt_proc_byref_mask(const char *name);
DESCR_t *rt_gvar_cell(const char *name);
int64_t rt_gvar_arith(const char *a, const char *b, int op);
int64_t rt_relop_descr2(int64_t l_lo, int64_t l_hi, int64_t r_lo, int64_t r_hi, int op);
int64_t rt_gvar_get_int(const char *name);
DESCR_t rt_gvar_get_descr(const char *name);
DESCR_t rt_proc_define(const char *spec);
void rt_arg_stage(int idx, DESCR_t v);
void rt_lcl_proc_args_install(void *rbp_base, int nparams, int nlocals);   /* ICN-PROC-FRAME (s211): copy g_call_args into lexical-proc frame param slots and zero locals; nargs from g_pcall top. */
void rt_icn_zframe_args_install(void *rbp_base, int nparams, int nlocals);   /* ICN-FR-2: ζ-frame variant — reads g_call_args[0..nparams-1] directly (no pcall-nargs clamp); correct for both dc-stub path (no pcall record) and jmp-entry C path. */
void  rt_pl_retry_push(void *addr);   /* PL-FR-4: push retry continuation onto the choice-point stack (WAM ALTB write; frame-independent — rt.c). */
void *rt_pl_retry_pop(void);          /* PL-FR-4: pop retry continuation (WAM `return ALTB(B)` read); 0 = exhausted = fail. */
void  rt_pl_cp_push(void *addr);      /* PL-FR-4 ZFRAME: push β-resume addr onto zframe retry stack (separate from cells g_pl_retry — rt.c). */
void *rt_pl_cp_pop(void);             /* PL-FR-4 ZFRAME: pop β-resume addr; 0 = exhausted = fail, jump to omega. */
void  rt_pl_cp_push3(long tm_lo, long tm_hi, void *cont);   /* PL-FR-4 ZFRAME TRIPLE: push {trail_mark_lo, trail_mark_hi, cont_addr} as one entry (3 slots); called by bb_suspend zframe arm at each yield. */
void *rt_pl_cp_pop3(long *tm_lo, long *tm_hi);               /* PL-FR-4 ZFRAME TRIPLE: pop triple and return cont_addr (0 = exhausted = omega); writes trail marks to *tm_lo/*tm_hi for caller to restore. */
void  rt_pl_zf_resume_set(void *cursor, long tm_lo, long tm_hi, int tm_off, int cursor_off);   /* PL-FR-4 ZFRAME RESUME: set pending-resume globals before re-calling rt_proc_call_open_det; xa_flat epilogue-γ picks them up while callee frame is live. */
void  rt_pl_zf_resume_clear(void);   /* PL-FR-4 ZFRAME RESUME: clear g_pl_zf_pending_cursor after the intercept writes the cursor. */
extern DESCR_t g_call_args[];
int  rt_proc_is_registered(const char *name);
void rt_c2b_arm_trap(void);
int  rt_proc_has_native_fn(const char *name);
void rt_proc_set_generator(const char *name, int is_gen);
void rt_proc_set_variadic(const char *name, int is_var);
void rt_proc_set_rest_kind(const char *name, int kind);
void rt_proc_set_named_rest(const char *name, int slot);
int rt_proc_named_rest(const char *name);
int  rt_proc_is_generator(const char *name);
void rt_grammar_register(const char *qname, const char *body, int flavor);
int  rt_grammar_count(void);
const char *rt_grammar_qname(int i);
const char *rt_grammar_body(int i);
int  rt_grammar_flavor(int i);
int  rt_grammar_has_top(const char *gname);
void rt_call_builtin(const char *name, int nargs);
int  rt_builtin_is_known(const char *name);
int  rt_builtin_is_generator(const char *name);
int  rt_is_truthy(DESCR_t v);
DESCR_t rt_size_d(uint64_t lo, uint64_t hi);
int  rt_field_get(const char *fname);
int  rt_field_set(const char *fname);
int  rt_idx_get(void);
int  rt_idx_set(void);
DESCR_t rt_list_bang_at(DESCR_t obj, int64_t idx);
DESCR_t rt_list_bang_var_at(DESCR_t obj, int64_t idx);
DESCR_t rt_random_var(DESCR_t base);
int  rt_limit_begin(DESCR_t *max_slot, int64_t *count_slot);
int  rt_limit_more(DESCR_t *max_slot, int64_t *count_slot);
int  rt_limit_inc(int64_t *count_slot);
int  rt_toby_real(DESCR_t *cur_slot, int64_t lo_bits, int64_t hi_bits, int64_t step_bits, int reset);
void *  rt_cs_new    (const char *chars);
void rt_cap_assign(const char *varname, const char *base, int len);
long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
void rt_cap_finish(DESCR_t fret);
void rt_cap_match_begin(void);
void rt_cap_push(void *slot, int delta);
void rt_cap_pop(void *slot);
int rt_cap_top(void *slot);
void rt_subject_load_nv(const char *name, void *slot);
long rt_defer_open(const char *varname, int ival_flag);
long rt_defer_step(DESCR_t fret);
int  rt_defer_close(int cur_delta);
void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
void *c_rt_defer_get_pat_fn(const char *varname, int ival_flag);
int rt_is(int dst_slot, const char *op, int lk, long li, int rk, long ri);
int rt_atom_length(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_upcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_downcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_char_type(int k0, long i0, const char *s0, const char *ty, int is_compound, int ki, long ii, const char *si);
int rt_numbervars_term(void *t0, long start, int k2, long i2, const char *s2);
int rt_atom_concat(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
int rt_atom_string_pair(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_number_string_pair(int num_first, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_term_to_atom_term(void *t0, int k1, long i1, const char *s1);
int rt_atomic_list_concat_term(void *list, int arity, int ksep, long isep, const char *ssep, int kres, long ires, const char *sres);
int rt_copy_term(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_copy_term_term(void *t0, int k1, long i1, const char *s1);
int rt_copy_term_terms(void *t0, void *t1);
int rt_nb_setval_term(void *key, void *val);
int rt_nb_getval_term(void *key, int kres, long ires, const char *sres);
int rt_aggregate_all_term(void *tmpl, void *goal, int kres, long ires, const char *sres);
int rt_atom_chars_codes(int as_codes, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_atom_chars_codes_term(int as_codes, int k0, long i0, const char *s0, void *t1);
int rt_type_test(const char *fn, int k0, long i0, const char *s0);
int rt_type_test_term(const char *fn, void *t0);
int rt_sort_msort(int do_msort, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_sort_msort_term(int do_msort, void *t0, int k1, long i1, const char *s1);
int rt_term_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_arith_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
void *rt_compound_build_n(const char *functor_name, int arity, void *args_ptr);
int rt_term_cmp_terms(const char *op, void *t0, void *t1);
int rt_pl_is_cell_int(void *lhs_cell, long val);
int rt_pl_is_cell_float(void *lhs_cell, double val);
int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op);
int rt_pl_is_cell(void *lhs_cell, void *rhs_node);
int rt_pl_arith_cmp_cells(const char *op, void *lhs_cell, void *lhs_nd, void *rhs_cell, void *rhs_nd);
int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival);
int rt_pl_is_cell_arith(void *lhs_cell, void *rhs_cell, const char *op, long rhs_ival);
int rt_arith_cmp_nodes(const char *op, void *lhs_node, void *rhs_node);
int rt_term_cmp_nodes(const char *op, void *lhs_node, void *rhs_node);
int rt_pl_type_test_cell(void *cell_term, const char *fn);
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell);
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell);
int rt_pl_univ_cell(void *t0_cell, void *list_cell);
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell);
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell);
long size_value(DESCR_t v, int *failed);
#ifdef __cplusplus
}
#endif
#endif
void rt_gvar_assign_concat_parts(const char *dst, void *parts, int n);
DESCR_t rt_concat_parts_d(void *parts, int n);
void rt_nofail_abort(void);
