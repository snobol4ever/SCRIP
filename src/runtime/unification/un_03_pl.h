#ifndef RUNTIME_UNIFICATION_UN_03_PL_H
#define RUNTIME_UNIFICATION_UN_03_PL_H
#include "runtime/unification/un_head.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
void *rt_pl_compound_cell(const char *functor_name, int arity, void *arg_words);
__attribute__((visibility("hidden"))) pl_cell_t plc_atom_id_cell(int id);
__attribute__((visibility("hidden"))) void plc_put_float(plc_vmap *m, pl_cell_t *d);
__attribute__((visibility("hidden"))) void plc_put_int(plc_vmap *m, long v);
__attribute__((visibility("hidden"))) int plc_xsb_codes(plc_vmap *m, pl_cell_t *d);
__attribute__((visibility("hidden"))) pl_cell_t plc_fz_var(plc_fz_t *z, long k);
__attribute__((visibility("hidden"))) int plc_atom_id_of(pl_cell_t *d);
__attribute__((visibility("hidden"))) int plc_is_atomlike(pl_cell_t *d);
__attribute__((visibility("hidden"))) int rt_pl_cell_compare_leaf(pl_cell_t *a, pl_cell_t *b, void *ctx);
__attribute__((visibility("hidden"))) int plc_cmp_bounded(pl_cell_t *ca, pl_cell_t *cb, int use_map, int *ok);
__attribute__((visibility("hidden"))) int plc_copy_run(void *wv);
__attribute__((visibility("hidden"))) int plc_copy_share_run(void *wv);
void *rt_pl_ball_kind2(const char *kind, const char *arg0_atom, DESCR_t culprit);
void *rt_pl_ball_kind1(const char *kind, const char *arg0_atom);
void *rt_pl_ball_instantiation(void);
__attribute__((visibility("hidden"))) pl_cell_t pl_trie_key_term(pl_trie_node_t *n);
#endif
