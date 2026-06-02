#ifndef RAKU_BUILTINS_H
#define RAKU_BUILTINS_H
#include "../ast/ast.h"
#include "core.h"
int script_try_call_builtin(tree_t *call, DESCR_t *out);
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
int script_try_mutating_builtin_by_name(const char *fn, const char *vname, DESCR_t *args, int nargs, DESCR_t *out);
#endif
