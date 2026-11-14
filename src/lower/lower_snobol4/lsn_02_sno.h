#ifndef LOWER_LOWER_SNOBOL4_LSN_02_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_02_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int sno_fname_is_multiproto(const char * fname);
__attribute__((visibility("hidden"))) const char * sno_expr_collect(const tree_t * expr);
__attribute__((visibility("hidden"))) int sno_define_entry_computed(const tree_t * t, int argbase);
__attribute__((visibility("hidden"))) int sno_is_xname(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_expr_eq(const tree_t * a, const tree_t * b);
__attribute__((visibility("hidden"))) int sno_expr_is_thunk(const char * nm);
__attribute__((visibility("hidden"))) const char * sno_expr_collect_nm(const tree_t * expr);
__attribute__((visibility("hidden"))) const char * sno_expr_collect_wn(const tree_t * expr);
__attribute__((visibility("hidden"))) int sno_def_entry_absent(const tree_t * subj, int argbase);
__attribute__((visibility("hidden"))) void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d);
__attribute__((visibility("hidden"))) const char * sno_define_entry_opt(const tree_t * dsub, int argbase);
__attribute__((visibility("hidden"))) void sno_bind_attach_proto(IR_graph_t * g, IR_t * bind, const sno_def_t * d, IR_t * fail);
__attribute__((visibility("hidden"))) void sno_bind_attach_entry(IR_graph_t * g, IR_t * bind, const char * entry, IR_t * fail);
__attribute__((visibility("hidden"))) void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d);
#endif
