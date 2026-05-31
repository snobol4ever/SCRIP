#ifndef LOWER_CTX_H
#define LOWER_CTX_H
#include "../../frontend/snobol4/scrip_cc.h"
#include "../ast/ast.h"
#include "../../runtime/interp/gen_runtime.h"
char *kw_canonicalize(const char *raw);
void expression_scope_walk(Scope *sc, tree_t *e);
#include "ast_clone.h"
#define T0(t) ((t)->n > 0 ? (t)->c[0] : NULL)
#define T1(t) ((t)->n > 1 ? (t)->c[1] : NULL)
#define T2(t) ((t)->n > 2 ? (t)->c[2] : NULL)
#endif
