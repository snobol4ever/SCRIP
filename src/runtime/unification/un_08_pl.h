#ifndef RUNTIME_UNIFICATION_UN_08_PL_H
#define RUNTIME_UNIFICATION_UN_08_PL_H
#include "runtime/unification/un_head.h"
__attribute__((visibility("hidden"))) void ** pl_db_cell_addr(void *root, int64_t k, int create);
void * rt_pl_db_get(void *root, int64_t k);
__attribute__((visibility("hidden"))) int plc_gvar_absent(const pl_cell_t *b);
int rt_pl_nb_set(void *root, int64_t k, void *val);
int rt_pl_b_set(void *root, int64_t k, void *val, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) pl_db_reg_t * pl_db_registry(void *root, int create);
__attribute__((visibility("hidden"))) pl_db_key_t * pl_db_reg_find(pl_db_reg_t *r, const char *key);
__attribute__((visibility("hidden"))) pl_db_mod_t * pl_db_mod_at(void *root, int idx);
__attribute__((visibility("hidden"))) void pl_db_note_live(void *root, pl_db_t *db, int i);
int rt_pl_db_key_cell(void *root, const char *key);
int rt_pl_db_cell_for(void *root, const char *name, int arity);
__attribute__((visibility("hidden"))) int pl_db_key_current(void *root, pl_db_key_t *e);
__attribute__((visibility("hidden"))) pl_db_t * pl_db_cell_peek(void *root, int k);
__attribute__((visibility("hidden"))) void pl_db_dynamize(void *root, pl_db_t *db);
__attribute__((visibility("hidden"))) const char *pl_db_head_name(void *pair_cell, int *ar);
__attribute__((visibility("hidden"))) int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile, int tx);
__attribute__((visibility("hidden"))) void pl_db_dynamize(void *root, pl_db_t *db);
__attribute__((visibility("hidden"))) int pl_db_data_enter(void *root, pl_db_t *db, int i);
__attribute__((visibility("hidden"))) void pl_db_note_erase(void *root, pl_db_t *db, int i, int gen);
#endif
