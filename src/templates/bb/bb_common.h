/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include <string>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include <stdio.h>
extern void rt_write_atom(const char *s);
extern void rt_pl_frame_sync_env(void *frame, int nslots);
extern void rt_pl_gz_init(void *frame, int nslots);
extern void rt_write_var(int slot);
extern void rt_write_term_ptr(void *t);
extern void rt_writeq_term_ptr(void *t);
extern void rt_write_canonical_term_ptr(void *t);
extern int  rt_is(int dst_slot, const char *op, int lk, long li, int rk, long ri);
extern int  rt_is_lint(long lval, const char *op, int lk, long li, int rk, long ri);
extern int  rt_is_cell(void *dst_cell, const char *op, int lk, void *larg, double ld, int rk, void *rarg, double rd);
extern int  rt_is_cell_lit(long lval, const char *op, int lk, void *larg, double ld, int rk, void *rarg, double rd);
extern int  rt_arith_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_term_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_type_test(const char *fn, int k0, long i0, const char *s0);
extern void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
extern int  rt_functor_term(void *t0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
extern int  rt_arg_term(int k0, long i0, const char *s0, void *t1, int k2, long i2, const char *s2);
extern int  rt_univ_term(void *t0, int k1, long i1, const char *s1);
extern int  rt_univ_term_list(int k0, long i0, const char *s0, void *t1);
extern int  rt_functor(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
extern int  rt_arg(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
extern int  rt_univ(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_number_string_pair(int num_first, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_format(int arity, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_format_term(int arity, int k0, long i0, const char *s0, void *args_term_ptr);
extern int  rt_term_to_atom_term(void *t0, int k1, long i1, const char *s1);
extern int  rt_atomic_list_concat_term(void *list, int arity, int ksep, long isep, const char *ssep, int kres, long ires, const char *sres);
extern int  rt_sort_msort(int do_msort, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_sort_msort_term(int do_msort, void *t0, int k1, long i1, const char *s1);
extern int  rt_copy_term_term(void *t0, int k1, long i1, const char *s1);
extern int  rt_copy_term_terms(void *t0, void *t1);
extern int  rt_nb_setval_term(void *key, void *val);
extern int  rt_nb_getval_term(void *key, int kres, long ires, const char *sres);
extern int  rt_aggregate_all_term(void *tmpl, void *goal, int kres, long ires, const char *sres);
extern int  rt_aggregate_all_meta(void *tmpl, void *goal, void *result);
extern int  rt_atom_concat(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
extern int  rt_atom_chars_codes(int as_codes, int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_atom_chars_codes_term(int as_codes, int k0, long i0, const char *s0, void *t1);
extern int  rt_char_type(int k0, long i0, const char *s0, const char *ty, int is_compound, int ki, long ii, const char *si);
extern int  rt_numbervars_term(void *t0, long start, int k2, long i2, const char *s2);
extern int  rt_succ(int k0, long i0, const char *s0, int k1, long i1, const char *s1);
extern int  rt_plus(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
extern int  rt_throw_term(void *ball_term);
}
#include "x86_asm.h"
std::string emit_build_compound_term(const IR_t *nd);
int bb_op_floaty(const char *fn);
