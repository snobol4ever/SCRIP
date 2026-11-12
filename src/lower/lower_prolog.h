#ifndef LOWER_PROLOG_H
#define LOWER_PROLOG_H
#include "ir/stage2.h"
stage2_t *lower_pl_stage2(const tree_t *prog);
#ifdef __cplusplus
extern "C" int emit_pl_scc_armed(void);
#else
int emit_pl_scc_armed(void);
#endif
#endif
