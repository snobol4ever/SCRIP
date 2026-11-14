#ifndef RUNTIME_LIB_BN_40_SH_H
#define RUNTIME_LIB_BN_40_SH_H
#include "runtime/lib/bn_head.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
long rt_record_image_id(void *inst);
__attribute__((visibility("hidden"))) DESCR_t *icn_variable_frame_cell(DESCR_t nm, char *stat, size_t statsz);
__attribute__((visibility("hidden"))) int icn_rchar(int with_echo);
DESCR_t rt_iand_d(DESCR_t *args, int nargs);
DESCR_t rt_ior_d(DESCR_t *args, int nargs);
DESCR_t rt_ixor_d(DESCR_t *args, int nargs);
DESCR_t rt_ishift_d(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) int icn_unicon_name(DESCR_t a, DESCR_t * out);
#endif
