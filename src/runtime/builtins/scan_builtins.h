#ifndef SCAN_BUILTINS_H
#define SCAN_BUILTINS_H
#include "../ast/ast.h"
#include "core.h"
int scan_try_call_builtin(tree_t *call, DESCR_t *args, int nargs, DESCR_t *out);
#endif
