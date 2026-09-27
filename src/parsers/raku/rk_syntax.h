#ifndef RK_SYNTAX_H
#define RK_SYNTAX_H
int rk_syntax_check(const char *src, int len, const char *path, char **err);
int rk_syntax_file(const char *path);
#include "ast.h"
tree_t *rk_parse_tree(const char *src, int len, const char *path, char **errmsg);
#endif
