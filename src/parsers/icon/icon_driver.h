/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ICON_DRIVER_H
#define ICON_DRIVER_H
#include "ast.h"
void icon_compile(const char *source, const char *filename, tree_t **out_ast);
void icn_prune_unreachable_procs(tree_t * prog);
#endif
