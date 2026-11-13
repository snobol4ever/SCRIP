#ifndef RUNTIME_UNIFICATION_UN_09_PL_H
#define RUNTIME_UNIFICATION_UN_09_PL_H
#include "runtime/unification/un_head.h"
__attribute__((visibility("hidden"))) int64_t pl_ix_head_key(const pl_db_t *db, const void *head);
__attribute__((visibility("hidden"))) pl_ix_ent_t * pl_ix_find(pl_ix_t *x, int64_t key);
__attribute__((visibility("hidden"))) int pl_ix_step(const pl_db_t *db, int64_t keyed, int i);
__attribute__((visibility("hidden"))) void pl_ix_note(pl_db_t *db, int i, int prepend);
__attribute__((visibility("hidden"))) void pl_db_frag_record(pl_db_t *db, int i, const void *fn);
__attribute__((visibility("hidden"))) void pl_db_reclaim(pl_db_t *db);
__attribute__((visibility("hidden"))) int pl_db_trim(pl_db_t *db, int closing);
__attribute__((visibility("hidden"))) int pl_db_data_fact(const pl_db_t *db, int i);
__attribute__((visibility("hidden"))) int pl_db_define_data(void *root, const char *nm, int arity, const char *skey);
__attribute__((visibility("hidden"))) void pl_db_reclaim_report(pl_db_t *db);
#endif
