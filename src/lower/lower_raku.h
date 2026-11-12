#ifndef LOWER_RAKU_H
#define LOWER_RAKU_H
#include "ir/stage2.h"
stage2_t *lower_raku_stage2(const tree_t *prog);
int lower_raku_refuse(const tree_t *prog, const char *file);
void lower_raku_source(tree_t *prog, const char *file);
const char *lower_raku_eval_stage2(const tree_t *prog);
#endif
