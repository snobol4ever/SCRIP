#ifndef LOWER_SNOCONE_H
#define LOWER_SNOCONE_H
#include "ir/ast.h"
tree_t *lower_snocone_tree(tree_t *pruned);
tree_t *lower_snocone_tree_at(tree_t *pruned, const char *path);
tree_t *snocone_resolve_imports(tree_t *prog, const char *path);
#endif
