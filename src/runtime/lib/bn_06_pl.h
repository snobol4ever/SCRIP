#ifndef RUNTIME_LIB_BN_06_PL_H
#define RUNTIME_LIB_BN_06_PL_H
#include "runtime/lib/bn_head_pl_vtab.h"
#include "runtime/rt_pl_reader.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) const char *pl_list_text_heap(DESCR_t v);
__attribute__((visibility("hidden"))) int pl_parse_term_text(const char *txt, DESCR_t *out, pl_vtab_t *vt, PlProgram **pg_out);
DESCR_t rt_pl_dop_read_c(DESCR_t *, int, pl_tr_ctx_t *);
__attribute__((visibility("hidden"))) int pl_read_term_options_cell(DESCR_t opts, pl_vtab_t *vt, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) void *pl_read_opts_ball(DESCR_t opts);
DESCR_t rt_pl_dop_read_term_opts_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_put_byte_c(DESCR_t *, int, pl_tr_ctx_t *);
#endif
