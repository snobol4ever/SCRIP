#ifndef LOWER_LOWER_ICON_LIC_02_SH_H
#define LOWER_LOWER_ICON_LIC_02_SH_H
#include "lower/lower_icon/lic_head.h"
__attribute__((visibility("hidden"))) const char * icn_synth_local(icx_t * cx);
__attribute__((visibility("hidden"))) tree_t * icn_variable_lit_tree(const char * nm);
__attribute__((visibility("hidden"))) tree_t * pm_node(tree_e k, tree_t * a, tree_t * b, tree_t * c);
__attribute__((visibility("hidden"))) tree_t * pm_var(const char * nm);
__attribute__((visibility("hidden"))) tree_t * pm_int(int64_t v);
__attribute__((visibility("hidden"))) tree_t * pm_call(const char * fn, tree_t * a0, tree_t * a1);
__attribute__((visibility("hidden"))) tree_t * rx_translate(const tree_t * t);
__attribute__((visibility("hidden"))) tree_t * th_form(icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) tree_t * th_listcomp(icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) tree_t * pm_patfn(icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * lower_pmatch(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_pmatch_repl(icx_t * cx, const tree_t * m, const tree_t * repl, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) const tree_t * pm_fn_form(icx_t * cx, const tree_t * t);
#endif
