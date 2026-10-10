#ifndef RK_REGEX_H
#define RK_REGEX_H
#include "../../ir/ast.h"
tree_t *rk_regex_parse(const char *flags, const char *s, int len);
#endif
