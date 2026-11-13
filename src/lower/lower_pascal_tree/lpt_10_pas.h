#ifndef LOWER_LOWER_PASCAL_TREE_LPT_10_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_10_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
__attribute__((visibility("hidden"))) tree_t *pas_pun_view(tree_t *sel, tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *pas_pun_view_to(const pty_t *P, tree_t *rhs);
__attribute__((visibility("hidden"))) void pas_forin_types(tree_t *root, tree_t *t, tree_t *blk);
__attribute__((visibility("hidden"))) void pas_untyped_formals(tree_t *root);
__attribute__((visibility("hidden"))) tree_t *pas_untyped_view(tree_t *addr);
__attribute__((visibility("hidden"))) void pas_lvalue_casts(tree_t *root, tree_t *t);
__attribute__((visibility("hidden"))) void pas_call_casts(tree_t *root, tree_t *t);
__attribute__((visibility("hidden"))) void pas_absolute_vars(tree_t *t);
#endif
