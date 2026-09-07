/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../ir/ast.h"
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
int pas_is_nrec_idx(const tree_t *e);
int pascal_sem_check(const tree_t *root, const char *filename);
#endif
