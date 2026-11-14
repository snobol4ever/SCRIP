/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "parsers/snobol4/scrip_cc.h"
#include "ir/ast.h"
CODE_t *rebus_lower(tree_t *prog);
tree_t *lower_rebus_tree(tree_t *prog);
