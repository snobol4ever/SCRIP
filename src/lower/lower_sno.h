#ifndef LOWER_SNO_H
#define LOWER_SNO_H
#include <stdio.h>
struct tree_t;
int tree_to_sno(const struct tree_t *ast, FILE *out);
#endif
