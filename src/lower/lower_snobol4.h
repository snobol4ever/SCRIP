/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef LOWER_SNOBOL4_H
#define LOWER_SNOBOL4_H
#include <stdio.h>
#include "ir/ast.h"
struct tree_t;
int tree_to_sno(const struct tree_t *ast, FILE *out);
int sno_predef_pattern_name(const char *s);
#include "ir/stage2.h"
stage2_t *lower_sno_stage2(const tree_t *prog);
#endif
