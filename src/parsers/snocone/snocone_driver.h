/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "ast.h"
void snocone_compile(const char *source, const char *filename, tree_t **out_ast);
void *snocone_compile_parse(const char *source, const char *filename);
void  snocone_compile_finish(void *parsed, const char *filename, tree_t **out_ast);
