/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_RESOLVE_H
#define PL_RESOLVE_H
#include "scrip_cc.h"
extern int g_resolve_cut_flag;
tree_t *resolve_pred_table_lookup_global(const char *key);
#endif
int resolve_throw_existence_error_procedure(const char *name, int arity);
