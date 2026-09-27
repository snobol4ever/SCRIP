#ifndef RK_SYNTAX_H
#define RK_SYNTAX_H
int rk_syntax_check(const char *src, int len, const char *path, char *err, int errlen);
int rk_syntax_file(const char *path);
int rk_dump_tree_file(const char *path);
#endif
