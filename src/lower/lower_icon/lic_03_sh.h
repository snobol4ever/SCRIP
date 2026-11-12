#ifndef LOWER_LOWER_ICON_LIC_03_SH_H
#define LOWER_LOWER_ICON_LIC_03_SH_H
#include "lower/lower_icon/lic_head.h"
#include "ir/ir_node.h"
__attribute__((visibility("hidden"))) IR_t * icn_line_hook(icx_t * cx, int line, IR_t * next);
__attribute__((visibility("hidden"))) IR_t * icn_trace_stmt_wrap(icx_t * cx, long line, IR_t * stmt_entry, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * icn_trace_named_prep(icx_t * cx, const char * hook, const char * name, IR_t * γ, IR_t * ω, IR_t ** call_out);
__attribute__((visibility("hidden"))) IR_t * icn_line_mark(icx_t * cx, int line, IR_t * next);
__attribute__((visibility("hidden"))) const char * icn_static_mangled(const icx_t * cx, const char * nm);
__attribute__((visibility("hidden"))) int icn_builtin_reassigned(const char * nm);
__attribute__((visibility("hidden"))) void icn_mark_γ_fail_conduit(IR_t * nd);
__attribute__((visibility("hidden"))) int icn_γ_is_fail_conduit(const IR_t * nd);
__attribute__((visibility("hidden"))) IR_t * lic_build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * icn_line_mark(icx_t * cx, int line, IR_t * next);
__attribute__((visibility("hidden"))) const tree_t * lic_stmt_subj(const tree_t * s);
__attribute__((visibility("hidden"))) int icn_is_icon_function(const char * nm);
__attribute__((visibility("hidden"))) int icn_never_pattern(const tree_t * a);
__attribute__((visibility("hidden"))) const char * icn_cset_canon(const char * s, int len, int * out_len);
__attribute__((visibility("hidden"))) void icn_attach_lit_len(icx_t * cx, IR_t * nd, int len);
__attribute__((visibility("hidden"))) int icn_callable_proc_index(const char * fn);
__attribute__((visibility("hidden"))) IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int icn_arg_is_scan_fn(const tree_t * a);
__attribute__((visibility("hidden"))) void icn_note_record_name(const char * nm);
__attribute__((visibility("hidden"))) IR_t * lc_key(icx_t * cx, const tree_t * t, const char * kw, IR_t * γ, IR_t * ω, IR_t ** res);
#endif
