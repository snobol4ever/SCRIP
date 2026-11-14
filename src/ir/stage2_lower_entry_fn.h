#ifndef STAGE2_LOWER_ENTRY_FN_H
#define STAGE2_LOWER_ENTRY_FN_H
#include <stdint.h>
#include "ir/ast.h"
#include "ir/stage2.h"
typedef stage2_t *(*lower_entry_fn)(const tree_t *prog);
#endif
