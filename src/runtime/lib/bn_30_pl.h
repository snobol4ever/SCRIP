#ifndef RUNTIME_LIB_BN_30_PL_H
#define RUNTIME_LIB_BN_30_PL_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) const char *pl_atom_str_n(DESCR_t v, size_t *n);
__attribute__((visibility("hidden"))) int pl_is_nil(DESCR_t v);
__attribute__((visibility("hidden"))) int pl_cell_text_n(DESCR_t v, char *buf, size_t n, const char **out, size_t *len);
const char * pl_skip_layout(const char *s);
__attribute__((visibility("hidden"))) int pl_parse_number(const char *s, DESCR_t *out);
__attribute__((visibility("hidden"))) int pl_iso_unbound(DESCR_t d);
__attribute__((visibility("hidden"))) int pl_anum_is_compound(DESCR_t d);
void * rt_pl_dop_goal_guard_c(DESCR_t *args, int nargs);
void * rt_pl_dop_list_guard_c(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) void * pl_anum_check(const char *nm, DESCR_t *a, int n);
#endif
