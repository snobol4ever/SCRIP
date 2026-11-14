#ifndef STAGE2_SM_PROG_API_H
#define STAGE2_SM_PROG_API_H
#include <stdint.h>
#include "ir/stage2.h"
extern stage2_t g_stage2;
void stage2_reset(void);
void ir_delete_all(stage2_t *s2);
int stage2_label_grow(stage2_t *s2);
int stage2_proc_grow (stage2_t *s2);
#ifdef __cplusplus
extern "C" {
#endif
int bb_proc_find(const char * fname);
#ifdef __cplusplus
}
#endif
#endif
