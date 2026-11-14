#ifndef RUNTIME_LIB_BN_05_PL_H
#define RUNTIME_LIB_BN_05_PL_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) int plw_bind_vv(DESCR_t *A, DESCR_t *B, pl_tr_ctx_t *cx, const char *floor_);
__attribute__((visibility("hidden"))) int plw_unify_vals(DESCR_t va, DESCR_t vb, pl_tr_ctx_t *cx);
DESCR_t rt_pl_deref_val(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rt_pl_goal_key(DESCR_t goal, int extra, DESCR_t **args_out, int *ar_out);
__attribute__((visibility("hidden"))) const char *pl_atom_str(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t pl_mk_atom(const char *s);
__attribute__((visibility("hidden"))) DESCR_t pl_nil(void);
__attribute__((visibility("hidden"))) DESCR_t pl_cons(DESCR_t head, DESCR_t tail);
__attribute__((visibility("hidden"))) DESCR_t pl_list_from_arr(DESCR_t *elems, int n);
__attribute__((visibility("hidden"))) int pl_val_unbound(DESCR_t d);
__attribute__((visibility("hidden"))) DESCR_t pl_ok(void);
__attribute__((visibility("hidden"))) void pl_atoms_ready(void);
__attribute__((visibility("hidden"))) int pl_is_cons(DESCR_t d);
__attribute__((visibility("hidden"))) DESCR_t pl_spine_end(DESCR_t cur, long *n_out, int *cyclic);
__attribute__((visibility("hidden"))) int pl_cell_text(DESCR_t v, char *buf, size_t n, const char **out);
DESCR_t dop_pl_writeq(DESCR_t *args, int nargs);
DESCR_t dop_pl_write_canonical(DESCR_t *args, int nargs);
DESCR_t dop_pl_flush_output(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) int pl_stream_idx_ball(DESCR_t s, int out, int textop, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) DESCR_t pl_unif_global(DESCR_t *c, const char *floor_, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_read_term_text(char **bp, size_t *np);
__attribute__((visibility("hidden"))) int pl_out_ioerr(int idx, DESCR_t s, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) DESCR_t pl_flag_term_value(const pl_flag_t *fl);
__attribute__((visibility("hidden"))) const char *pl_gnu_path_arg(DESCR_t a, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_gnu_os_fail(pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) void pl_gnu_ctx(pl_tr_ctx_t *cx, const char *nm, int ar);
__attribute__((visibility("hidden"))) DESCR_t pl_ct_a(DESCR_t t, int i);
__attribute__((visibility("hidden"))) int pl_os_errv(DESCR_t r, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_os_okv(DESCR_t r, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) void * pl_db_cell_of(DESCR_t *args, void *root);
__attribute__((visibility("hidden"))) int pl_anum_is_text(DESCR_t d);
__attribute__((visibility("hidden"))) int pl_cell_is_clref(DESCR_t t);
#endif
