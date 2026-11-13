#ifndef LOWER_LOWER_PASCAL_TREE_LPT_12_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_12_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
__attribute__((visibility("hidden"))) PNodeList *pas_defaults_extend(const char *routine, PNodeList *args);
__attribute__((visibility("hidden"))) tree_t *pas_default_get(const char *routine, int idx);
__attribute__((visibility("hidden"))) long long pas_decl_part_order(long long prev, long long cur);
__attribute__((visibility("hidden"))) const char *pas_ov_psig(const tree_t *n);
__attribute__((visibility("hidden"))) void pas_ov_tag(const char *psig);
__attribute__((visibility("hidden"))) const char *pas_overload_name(const char *src, const char *psig);
__attribute__((visibility("hidden"))) const char *pas_overload_pick(const char *src, PNodeList *args, int *np_out);
__attribute__((visibility("hidden"))) const pty_t *pas_formal_type(const char *name, int idx);
__attribute__((visibility("hidden"))) int pascal_sem_check(const tree_t *root, const char *filename);
__attribute__((visibility("hidden"))) int pas_tree_has_event(const tree_t *t, const char *ev);
__attribute__((visibility("hidden"))) void pas_iso_empty_strings(const tree_t *t);
__attribute__((visibility("hidden"))) tree_t *pas_sign_binds_factor(tree_t *t);
__attribute__((visibility("hidden"))) void pas_apply_event(const char *ev);
__attribute__((visibility("hidden"))) int pas_result_flips_dialect(const tree_t *root);
__attribute__((visibility("hidden"))) void pas_defaults_prepass(tree_t *t);
__attribute__((visibility("hidden"))) void pas_param_prepass(tree_t *t);
#endif
