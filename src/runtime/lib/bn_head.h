#ifndef RUNTIME_LIB_BN_HEAD_H
#define RUNTIME_LIB_BN_HEAD_H
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
#include "runtime/keywords_g_error_eval_at.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "ir/gc_frame_map.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head_rk_redisp.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/lib/bn_head_bn_direct.h"
#ifdef __cplusplus
extern "C" {
#endif
RT_ENTRY_DECL(rt_pl_fresh_var_ref, leaf, 0);
RT_ENTRY_DECL(rk_is_truthy, leaf, 0);
_Static_assert(sizeof(rk_redisp_t) == RK_REDISP_REC_BYTES,
    "the redispatch record is RK_REDISP_REC_BYTES of the meth_call node's frame grant (frame_layout.c grants RK_REDISP_REC_SLOTS slots after the argv)");
RT_ENTRY_DECL(rk_method_open, open, 1);
RT_ENTRY_DECL(rt_apply_open, open, 1);
RT_ENTRY_DECL(rt_apply_land_γ, leaf, 0);
RT_ENTRY_DECL(rt_apply_land_ω, leaf, 0);
_Static_assert((RT_TWEAK_IC_N & (RT_TWEAK_IC_N - 1)) == 0,
    "THE CONSTRUCTOR HOOK CACHE (2026-09-25, R-1258): a type whose method-resolution chain carries no <type>__TWEAK procedure is remembered by its name pointer under a generation that moves wh"
    "enever a procedure or a type is added or the procedure table is reset, so the check that cost every record constructor a formatted name, a proc-table scan and a hash lookup runs once per type p"
    "er generation");
RT_ENTRY_DECL(rt_tweak_open, leaf, 0);
RT_ENTRY_DECL(rt_call_value_gen_h, gen, 0);
RT_ENTRY_DECL(rt_call_apply_gen_h, gen, 0);
RT_ENTRY_DECL(rt_call_value_spine_prep, open, 1);
RT_ENTRY_DECL(rt_call_value_spine_prep_blk, open, 1);
RT_ENTRY_DECL(rt_call_apply_spine_prep, open, 1);
RT_ENTRY_DECL(rt_call_apply_spine_prep_blk, open, 1);
RT_ENTRY_DECL(rt_call_value_resume_h, gen, 0);
RT_ENTRY_DECL(rt_pl_unify_const_cold, leaf, 0);
RT_ENTRY_DECL(dop_write, leaf, 0);
RT_ENTRY_DECL(dop_nl, leaf, 0);
RT_ENTRY_DECL(dop_pl_ground, leaf, 0);
RT_ENTRY_DECL(dop_pl_is_list, leaf, 0);
RT_ENTRY_DECL(dop_pl_between_special, leaf, 0);
RT_ENTRY_DECL(dop_pl_acyclic_term, leaf, 0);
RT_ENTRY_DECL(dop_pl_writeq, leaf, 0);
RT_ENTRY_DECL(dop_pl_write_canonical, leaf, 0);
RT_ENTRY_DECL(dop_pl_writeln, leaf, 0);
RT_ENTRY_DECL(dop_pl_tab, leaf, 0);
RT_ENTRY_DECL(dop_pl_put_char, leaf, 0);
RT_ENTRY_DECL(dop_pl_put_code, leaf, 0);
RT_ENTRY_DECL(dop_pl_flush_output, leaf, 0);
RT_ENTRY_DECL(dop_pl_set_input, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_sub_atom_n, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_atom_concat_n, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_big, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_findall_new, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_findall_add, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_bagof_group_n, leaf, 0);
RT_ENTRY_DECL(dop_pl_at_end_of_stream, leaf, 0);
RT_ENTRY_DECL(rt_sno_patck_d, leaf, 0);
_Static_assert(__builtin_offsetof(pl_flag_t, nm) == 0 && __builtin_offsetof(pl_flag_t, val) == 8 && __builtin_offsetof(pl_flag_t, is_term) == 72 && __builtin_offsetof(pl_flag_t, term) == 80 &&
    sizeof(pl_flag_t) == 104, "rt_gc_flags_roots_run (rtx_gc.s) walks the Prolog flags at these offsets, 104 bytes apart");
RT_ENTRY_DECL(dop_pl_told, leaf, 0);
RT_ENTRY_DECL(dop_pl_seen, leaf, 0);
_Static_assert(offsetof(ARBLK_t, lo) == 0 && offsetof(ARBLK_t, hi) == 4 && offsetof(ARBLK_t, data) == 32 && offsetof(ARBLK_t, ewidth) == 56 && sizeof(DESCR_t) == 16,
    "bb_call_pas_elem.cpp reads the block bounds, the data vector and the element width at these offsets");
RT_ENTRY_DECL(pas_arr_make, leaf, 0);
RT_ENTRY_DECL(pas_arr_make_raw, leaf, 0);
RT_ENTRY_DECL(pas_arr_get, leaf, 0);
RT_ENTRY_DECL(pas_arr_set, leaf, 0);
RT_ENTRY_DECL(c_rt_rk_arr_set, leaf, 0);
RT_ENTRY_DECL(c_rt_rk_arr_at, leaf, 0);
RT_ENTRY_DECL(rt_rk_undef, leaf, 0);
RT_ENTRY_DECL(pas_slice, leaf, 0);
RT_ENTRY_DECL(rt_call_pas_bi, leaf, 0);
RT_ENTRY_DECL(rt_call_arr, leaf, 0);
RT_ENTRY_DECL(rt_call_arr_bl, leaf, 0);
static inline unsigned rt_ctor_ic_h(const char *fn) __attribute__((always_inline));
static inline int rt_ctor_ic_find(const char *fn, long gen) __attribute__((always_inline));
_Static_assert((RT_CTOR_IC_N & (RT_CTOR_IC_N - 1)) == 0,
    "THE RECORD-CONSTRUCTOR INLINE CACHE (2026-09-25, R-1258): a call site whose name the full by-name path resolved to a DATA type and constructed positionally is remembered by its na"
    "me pointer under a generation that moves whenever a procedure, a type, an EVAL'd procedure or the dtax cache changes, so the next call at that site constructs directly -- the same dat_c"
    "onstruct, the same TWEAK hook, without the 5,000 instructions of checks that precede the positional arm");
RT_ENTRY_DECL(rt_call_arr_bl_strict, leaf, 0);
_Static_assert((RT_FIELD_IC_N & (RT_FIELD_IC_N - 1)) == 0,
    "THE FIELD-ACCESSOR INLINE CACHE (2026-09-25, R-1258): a call site's field name pointer and the instance's type pointer key a direct-mapped slot holding the field index; a hit is c"
    "onfirmed by strcmp against the type's own field name, so a reused string address can never answer with another field's index, and the value is the instance's own cell, exactly what dat_"
    "field_get returns");
static inline int rt_field_index_cached_i(const char *fn, const DATBLK_t *idb) __attribute__((always_inline));
RT_ENTRY_DECL(rt_call_fld_sn4, leaf, 0);
RT_ENTRY_DECL(rt_call_arr_bl_try, open, 1);
RT_ENTRY_DECL(rt_call_arr_bl_sn4_try, open, 1);
RT_ENTRY_DECL(rt_call_callee_try_sn4, open, 1);
_Static_assert(G_ERROR_EVAL_STAGE == (-9223372036854775807L - 1), "rtx_misc.s rt_call_bid_sn4 passes g_error by doubling it: only 0 and LONG_MIN double to zero");
RT_ENTRY_DECL(c_rt_call_bid_sn4, leaf, 0);
RT_ENTRY_DECL(rt_call_arr_gen, gen, 0);
RT_ENTRY_DECL(rt_make_list, leaf, 0);
RT_ENTRY_DECL(rt_main_args_stage, leaf, 0);
RT_ENTRY_DECL(rt_main_args_stage_argv, leaf, 0);
RT_ENTRY_DECL(rt_main_progname_stage, leaf, 0);
RT_ENTRY_DECL(rt_main_args_bind, leaf, 0);
RT_ENTRY_DECL(c_rt_str_coerce, leaf, 0);
RT_ENTRY_DECL(c_rt_jct_relop, leaf, 0);
_Static_assert(sizeof(bn_direct_t) == 16, "rtx_misc.s indexes g_bn_direct with a 16-byte stride: fn at +0, op at +8 (2026-09-25, R-1255)");
RT_ENTRY_DECL(c_rt_sno_name_dl, leaf, 0);
RT_ENTRY_DECL(rt_sno_mkexpr_d, leaf, 0);
RT_ENTRY_DECL(rt_sno_xlv_d, leaf, 0);
RT_ENTRY_DECL(rk_box_step, leaf, 0);
RT_ENTRY_DECL(rk_box_back, leaf, 0);
RT_ENTRY_DECL(rk_box_match_build, leaf, 0);
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head_rt_ctor_ic_h.h"
#include "runtime/lib/bn_head_rt_ctor_ic_find.h"
#include "runtime/lib/bn_head_rt_field_index_cached_i.h"
#endif
