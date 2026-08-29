/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "rebus.h"
#include "../../parsers/snobol4/scrip_cc.h"
CODE_t *rebus_lower(tree_t *prog);
void rebus_compile(const char *src, const char *filename, tree_t **out_ast);
