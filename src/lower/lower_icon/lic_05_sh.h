#ifndef LOWER_LOWER_ICON_LIC_05_SH_H
#define LOWER_LOWER_ICON_LIC_05_SH_H
#include "lower/lower_icon/lic_head.h"
#include "ir/cset_block.h"
#include "ir/stage2.h"
__attribute__((visibility("hidden"))) int icn_const_step(const tree_t * s, int64_t * bits, int * isr);
__attribute__((visibility("hidden"))) IR_t * lic_lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_function_gen(icx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int icn_field_hint(const tree_t * prog, const char * fn);
__attribute__((visibility("hidden"))) void icn_field_var_store_only(IR_t * lv);
__attribute__((visibility("hidden"))) int icn_op_hands_line(tree_e k);
#endif
