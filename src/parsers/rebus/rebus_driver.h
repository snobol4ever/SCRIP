#ifndef REBUS_DRIVER_H
#define REBUS_DRIVER_H
#include "ir/ast.h"
void rebus_compile(const char *src, const char *filename, tree_t **out_ast);
void *rebus_compile_parse(const char *src, const char *filename);
void rebus_compile_finish(void *parsed, const char *filename, tree_t **out_ast);
#endif
