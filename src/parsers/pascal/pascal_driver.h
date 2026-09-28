/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../ir/ast.h"
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
int pascal_sem_check(const tree_t *root, const char *filename);
#endif
