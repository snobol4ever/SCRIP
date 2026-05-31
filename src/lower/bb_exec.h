#pragma once
#ifndef BB_EXEC_H
#define BB_EXEC_H
#include "BB.h"
typedef int (*bb_body_fn)(DESCR_t value, void * ctx);
DESCR_t bb_exec_once(IR_graph_t * bbg);
DESCR_t bb_exec_resume(IR_graph_t * bbg);
int bb_exec_pump(IR_graph_t * bbg, bb_body_fn body_fn, void * ctx);
BB_t * bb_exec_node(BB_t * bb);
int bb_exec_pat(IR_graph_t *bbg,
                const char *subj_name,
                DESCR_t    *subj_var,
                DESCR_t    *repl,
                int         has_repl);
int rt_pl_is(int dst_slot, const char *op, int lk, long li, int rk, long ri);
int rt_pl_is_eval(void *lhs_bb, void *rhs_bb);
int rt_pl_atom_length(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_upcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_downcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_char_type(int k0, long i0, const char *s0, const char *ty, int is_compound, int ki, long ii, const char *si);
int rt_pl_numbervars_term(void *t0, long start, int k2, long i2, const char *s2);
int rt_pl_atom_concat(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
int rt_pl_atom_string_pair(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_number_string_pair(int num_first, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_term_to_atom_term(void *t0, int k1, long i1, const char *s1);
int rt_pl_atomic_list_concat_term(void *list, int arity, int ksep, long isep, const char *ssep, int kres, long ires, const char *sres);
int rt_pl_copy_term(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_copy_term_term(void *t0, int k1, long i1, const char *s1);
int rt_pl_copy_term_terms(void *t0, void *t1);
int rt_pl_nb_setval_term(void *key, void *val);
int rt_pl_nb_getval_term(void *key, int kres, long ires, const char *sres);
int rt_pl_aggregate_all_term(void *tmpl, void *goal, int kres, long ires, const char *sres);
int rt_pl_atom_chars_codes(int as_codes, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_atom_chars_codes_term(int as_codes, int k0, long i0, const char *s0, void *t1);
int rt_pl_findall(void *fs_ptr);
int rt_pl_catch(void *zc_ptr);
int rt_pl_throw(void *alpha_ptr);
int rt_pl_type_test(const char *fn, int k0, long i0, const char *s0);
int rt_pl_type_test_term(const char *fn, void *t0);
int rt_pl_sort_msort(int do_msort, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_sort_msort_term(int do_msort, void *t0, int k1, long i1, const char *s1);
int rt_pl_term_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
int rt_pl_arith_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
void *rt_pl_compound_build_n(const char *functor_name, int arity, void *args_ptr);
int rt_pl_term_cmp_terms(const char *op, void *t0, void *t1);
long size_value(DESCR_t v, int *failed);
#endif
