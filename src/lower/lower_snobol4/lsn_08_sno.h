#ifndef LOWER_LOWER_SNOBOL4_LSN_08_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_08_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
__attribute__((visibility("hidden"))) const char * sno_lead_name(const char * lead, const char * nm);
__attribute__((visibility("hidden"))) const char * sno_cursor_target(const tree_t * tgt);
__attribute__((visibility("hidden"))) const char * sno_lead_name(const char * lead, const char * nm);
__attribute__((visibility("hidden"))) const char * sno_capt_name(const tree_t * tgt);
__attribute__((visibility("hidden"))) void sno_ω_to(IR_t * nd, IR_t * t);
__attribute__((visibility("hidden"))) IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail);
__attribute__((visibility("hidden"))) void sno_cap_defer_reset(void);
#endif
