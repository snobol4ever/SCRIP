/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef SCRIP_SM_H
#define SCRIP_SM_H
#include "ir/stage2.h"
#include "ir/stage2_lower_seg.h"
#include "ir/ast.h"
stage2_t *sm_preamble(const tree_t *ast_prog, const lower_seg_t *segs, int nsegs);
void stage2_free_sm_bb(stage2_t *s2);
void stage2_free_bb_after_emit(stage2_t *s2);
#endif
