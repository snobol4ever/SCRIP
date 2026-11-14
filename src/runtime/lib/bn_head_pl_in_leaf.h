#ifndef BN_HEAD_PL_IN_LEAF_H
#define BN_HEAD_PL_IN_LEAF_H
#include <glob.h>
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_pas.h"
#include "runtime/core/g_runtime_bn.h"
#include "runtime/rt/rtx_try.h"
#include "runtime/core/array.h"
#include "runtime/core/datatype.h"
#include "runtime/core/table.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags_dt_platom.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define PL_SYNERR(vt) ((vt).err ? (vt).err : "cannot_start_term")
#define PL_READ_TERM_LEAF(nm, at) PL_CX_LEAF_HEAD(nm, 3) { ok = pl_read_term_from_text(args, cx, at); } PL_CX_LEAF_TAIL
#define PL_TEXTOP_read_term_opts 1
#define PL_TEXTOP_read 1
#define PL_TEXTOP_get_char 1
#define PL_TEXTOP_peek_char 1
#define PL_TEXTOP_get_code 1
#define PL_TEXTOP_peek_code 1
#define PL_TEXTOP_unget_char 1
#define PL_TEXTOP_unget_code 1
#define PL_TEXTOP_get_byte (-1)
#define PL_TEXTOP_peek_byte (-1)
#define PL_TEXTOP_unget_byte (-1)
#define PL_IN_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm##_s, ar) { extern int fh_current_input(void); extern void fh_set_input(int); int idx = pl_stream_idx_ball(args[0], 0, PL_TEXTOP_##nm, cx); ok = 0; if ( \
    idx >= 0) { int sv = fh_current_input(); fh_set_input(idx); DESCR_t r = rt_pl_dop_##nm##_c(args + 1, ar - 1, cx); fh_set_input(sv); ok = (r.v == (DTYPE_t)DT_I); cx->ball = pl_ball_sora(cx->ball, \
    args[0]); } } PL_CX_LEAF_TAIL
#define PL_OUT_CX_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm##_s, ar) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx_ball(args[0], 1, PL_TEXTOP_##nm, cx); ok = 0; \
    if (idx >= 0) { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = rt_pl_dop_##nm##_c(args + 1, ar - 1, cx); fh_set_output(sv); ok = pl_out_ioerr(idx, args[0], cx) && r.v == (DTYPE_t) \
    DT_I; } } PL_CX_LEAF_TAIL
#define PL_OUT_BALL_LEAF(nm, body1, ar) PL_CX_LEAF_HEAD(nm##_sb, ar) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx_ball(args[0], 1, 1, cx); ok = 0; if \
    (idx >= 0) { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = body1(args + 1, ar - 1); fh_set_output(sv); ok = pl_out_ioerr(idx, args[0], cx) && r.v == (DTYPE_t)DT_I; } } \
    PL_CX_LEAF_TAIL
#define PL_TEXTOP_put_byte (-1)
#define PL_TEXTOP_put_char_c 1
#define PL_TEXTOP_write_term 1
#define PL_TEXTOP_print 1
#define PL_TEXTOP_display 1
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#endif
