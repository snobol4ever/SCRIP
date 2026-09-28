/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../ir/ast.h"
#define PAS_PARAM_BYREF_MARK 0x42595245
static inline int pas_param_is_byref(const tree_t *pl, int k) { return pl && k >= 0 && k < pl->n && pl->c[k] && pl->c[k]->slen == PAS_PARAM_BYREF_MARK; }
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
int pascal_sem_check(const tree_t *root, const char *filename);
#endif
