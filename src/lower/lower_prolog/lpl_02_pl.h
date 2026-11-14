#ifndef LOWER_LOWER_PROLOG_LPL_02_PL_H
#define LOWER_LOWER_PROLOG_LPL_02_PL_H
#include "lower/lower_prolog/lpl_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int pl_swi_user_redefines(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_decl_mf_is(const char * name, int arity);
__attribute__((visibility("hidden"))) void pl_decl_meta_record(const tree_t * spec);
__attribute__((visibility("hidden"))) const tree_t * pl_decl_meta_of(const char * name, int arity);
__attribute__((visibility("hidden"))) int pl_xsb_sysmod(const char * m);
__attribute__((visibility("hidden"))) int pl_phrase_plain_nt(const tree_t * g);
__attribute__((visibility("hidden"))) const tree_t * pl_meta_goal(const tree_t * g, const tree_t * const * extra, int nextra);
__attribute__((visibility("hidden"))) void * pl_runtime_define_pred_x(const char * key, const tree_t * choice, int arity, IR_graph_t ** gout, int pkt_cell, int pkt_slot, int * chain_off_out, void * rt_root);
__attribute__((visibility("hidden"))) int pl_mkc_functor_is(const IR_t * f, const char * nm);
__attribute__((visibility("hidden"))) int pl_name_is_scrip_own(const char * nm, int ar);
__attribute__((visibility("hidden"))) void pl_dyn_spec_file(const tree_t * spec, const char * file);
__attribute__((visibility("hidden"))) void pl_db_decls_capture(void);
__attribute__((visibility("hidden"))) const tree_t * pl_dict_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) int pl_names_depth_limit(const tree_t * t);
#endif
