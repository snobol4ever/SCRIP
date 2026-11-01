/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RAKU_DRIVER_H
#define RAKU_DRIVER_H
#include "ir/ast.h"
void raku_compile(const char *src, const char *filename, tree_t **out_ast);
#endif
