/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "rebus.h"
#include "../../parsers/snobol4/scrip_cc.h"
CODE_t *rebus_lower(tree_t *prog);
void rebus_compile(const char *src, const char *filename, tree_t **out_ast);
void *rebus_compile_parse(const char *src, const char *filename);
void  rebus_compile_finish(void *parsed, const char *filename, tree_t **out_ast);
