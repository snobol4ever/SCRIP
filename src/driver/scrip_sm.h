#ifndef SCRIP_SM_H
#define SCRIP_SM_H
#include "stage2.h"
#include "ast.h"
stage2_t *sm_preamble(const tree_t *ast_prog);
void stage2_free_sm_bb(stage2_t *s2);
void stage2_free_bb_after_emit(stage2_t *s2);
#endif
