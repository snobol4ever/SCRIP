#ifndef LOWER_LOWER_SNOBOL4_LSN_09_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_09_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) IR_t * sno_mkpat_emit(scx_t * cx, const tree_t * pat, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int sno_pat_dfree(const tree_t * t, int spine, int depth);
__attribute__((visibility("hidden"))) void sno_prologue_add(const char * nm);
__attribute__((visibility("hidden"))) int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp);
__attribute__((visibility("hidden"))) void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i);
__attribute__((visibility("hidden"))) IR_t * sno_lower_match_body(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land);
#endif
