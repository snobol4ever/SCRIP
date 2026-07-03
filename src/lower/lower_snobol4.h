/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef LOWER_SNOBOL4_H
#define LOWER_SNOBOL4_H
#include <stdio.h>
struct tree_t;
int tree_to_sno(const struct tree_t *ast, FILE *out);
#endif
