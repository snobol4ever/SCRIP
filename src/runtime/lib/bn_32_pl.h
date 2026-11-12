#ifndef RUNTIME_LIB_BN_32_PL_H
#define RUNTIME_LIB_BN_32_PL_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) char *pl_atom_carve_cx(size_t n, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) DESCR_t pl_mk_atom_carved(char *b, size_t n, size_t k);
__attribute__((visibility("hidden"))) DESCR_t pl_os_cwd_atom(void);
__attribute__((visibility("hidden"))) int bnd_db_key_need(void *cell);
#endif
