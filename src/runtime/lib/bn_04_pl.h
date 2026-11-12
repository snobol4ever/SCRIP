#ifndef RUNTIME_LIB_BN_04_PL_H
#define RUNTIME_LIB_BN_04_PL_H
#include "runtime/lib/bn_head.h"
int icn_builtin_arity(const char *name);
const char *icn_builtin_nth(int k);
__attribute__((visibility("hidden"))) void plw_bind(DESCR_t *cell, DESCR_t word, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int plw_unify_cells(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int plw_unify_cell_val(DESCR_t *dst, DESCR_t val, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int plu_leaf(DESCR_t *A, DESCR_t *B, void *ctx);
__attribute__((visibility("hidden"))) int rt_pl_attv_atts_cell(DESCR_t *av, DESCR_t *aa, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int rt_pl_crv_enter_cell(DESCR_t *am, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_db_body_ill_typed(DESCR_t b);
__attribute__((visibility("hidden"))) char * pl_tree_name(const char *s);
void * rt_pl_choice_new(const char *key);
#endif
