#ifndef RUNTIME_LIB_BN_34_PL_H
#define RUNTIME_LIB_BN_34_PL_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) int pl_rb_room(char **bp, size_t *np, size_t need);
__attribute__((visibility("hidden"))) int pl_text_fffd(char **bp, size_t *np);
__attribute__((visibility("hidden"))) int pl_read_term_text_raw(char **bp, size_t *np);
__attribute__((visibility("hidden"))) const char *pl_gp_read_err(long off0);
__attribute__((visibility("hidden"))) int pl_gs_read(const char *op, DESCR_t res, pl_tr_ctx_t *cx);
#endif
