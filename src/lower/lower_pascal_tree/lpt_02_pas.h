#ifndef LOWER_LOWER_PASCAL_TREE_LPT_02_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_02_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
__attribute__((visibility("hidden"))) const pty_t *pas_rvt(const char *vn);
__attribute__((visibility("hidden"))) int pas_rvf(const char *vn, long idx, struct pty_fld *f);
__attribute__((visibility("hidden"))) const char *pas_fld_rectype(const struct pty_fld *f);
__attribute__((visibility("hidden"))) const char *pas_ptrexpr_target(tree_t *e);
__attribute__((visibility("hidden"))) const char *pas_with_sel_rtype(tree_t *sel);
__attribute__((visibility("hidden"))) const char *pas_rectype_field_typename(const char *rn, long idx);
__attribute__((visibility("hidden"))) int pas_rectype_nf(const char *rn);
__attribute__((visibility("hidden"))) int pas_is_aggregate_designator(tree_t *e);
__attribute__((visibility("hidden"))) int pas_rectype_total_size(const char *rn, long long *out);
__attribute__((visibility("hidden"))) int pas_rectype_field_offset(const char *rn, int idx, long long *out);
__attribute__((visibility("hidden"))) struct pas_vt *pas_vt_new(int state);
__attribute__((visibility("hidden"))) int pas_rectype_slot(const char *rn, int fi);
__attribute__((visibility("hidden"))) int pas_rectype_has_file(const char *rn);
__attribute__((visibility("hidden"))) tree_t *pas_field_region(tree_t *base, const char *rt, int fi, tree_t *e);
__attribute__((visibility("hidden"))) const char *pas_rectype_field_enum_by_index(const char *rn, long idx);
__attribute__((visibility("hidden"))) tree_t *pas_tree_clone(tree_t *e);
__attribute__((visibility("hidden"))) int pas_enumnames_idx(const char *tn);
__attribute__((visibility("hidden"))) const char *pas_trace_enum_names_of_var(const char *vn);
__attribute__((visibility("hidden"))) const char *pas_trace_store_name(const tree_t *lhs);
__attribute__((visibility("hidden"))) tree_t *pas_tree_clone(tree_t *e);
__attribute__((visibility("hidden"))) int pas_ty_dyn_depth(tree_t *v, const pty_t **leaf);
__attribute__((visibility("hidden"))) tree_t *pas_ty_dyn_proto(const pty_t *q);
__attribute__((visibility("hidden"))) int pas_pend_is_rectype(const char *rn);
__attribute__((visibility("hidden"))) int pas_recvar_slot(const char *vn, int fi);
__attribute__((visibility("hidden"))) const char *pas_ptrexpr_target(tree_t *e);
__attribute__((visibility("hidden"))) int pas_with_field_index(const char *rtype, const char *fname);
__attribute__((visibility("hidden"))) int pas_with_recvar_field(const char *vname, const char *fname);
__attribute__((visibility("hidden"))) void pas_with_push_once(tree_t *sel);
__attribute__((visibility("hidden"))) tree_t *pas_with_finish(long long n, tree_t *body);
__attribute__((visibility("hidden"))) int pas_rectype_is_packed(const char *rn);
__attribute__((visibility("hidden"))) int pas_actual_is_packed_component(tree_t *a);
__attribute__((visibility("hidden"))) tree_t *pas_vt_wrap_read(tree_t *sel);
__attribute__((visibility("hidden"))) tree_t *pas_vt_strip_undef(tree_t *a);
__attribute__((visibility("hidden"))) tree_t *pas_vt_wrap_stmt(tree_t *sel, tree_t *rhs, tree_t *st);
__attribute__((visibility("hidden"))) const char *pas_store_tn(tree_t *sel);
__attribute__((visibility("hidden"))) void pas_value_formal_file_check(PNodeList *ids, const char *t);
__attribute__((visibility("hidden"))) pval E_selector_raw(const tree_t *n);
#endif
