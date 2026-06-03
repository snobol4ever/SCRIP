#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../contracts/ast.h"
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
#endif
