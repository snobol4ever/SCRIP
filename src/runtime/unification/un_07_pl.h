#ifndef RUNTIME_UNIFICATION_UN_07_PL_H
#define RUNTIME_UNIFICATION_UN_07_PL_H
#include "runtime/unification/un_head.h"
void rt_pl_ball_report(void *ball);
__attribute__((visibility("hidden"))) int pl_db_mod_find(pl_db_reg_t *r, int atom, int create);
__attribute__((visibility("hidden"))) int pl_db_mod_of_key(pl_db_reg_t *r, const char *key, int create);
__attribute__((visibility("hidden"))) void pl_db_kmod_set(pl_db_reg_t *r, int k, int mod);
__attribute__((visibility("hidden"))) void pl_db_key_set_ncl(void *root, const char *name, int64_t arity, int64_t v);
__attribute__((visibility("hidden"))) int pl_db_define_absent(void *root, const char *key, int arity);
#endif
