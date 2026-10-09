/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../ir/ast.h"
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
void *pascal_compile_parse(const char *source, const char *filename);
void  pascal_compile_finish(void *parsed, const char *filename, tree_t **out_ast);
#endif
