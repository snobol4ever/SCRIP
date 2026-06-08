/*====================================================================================================================*/
/*====================================================================================================================*/
#ifndef LOWER_SNOBOL4_INTERNAL_H
#define LOWER_SNOBOL4_INTERNAL_H
#include "lower.h"
#include <string.h>
/*--------------------------------------------------------------------------------------------------------------------*/
typedef enum { ROLE_VALUE = 0, ROLE_PATTERN = 1 } lower_role_e;
typedef struct { int count; } pl_vars_t;
typedef struct {
    IR_graph_t * bbg;
    lower_role_e role;
    int          bounded;
    int          lang;
    IR_t       * loop_ω;
    IR_t       * loop_next;
    pl_vars_t  * pl_vars;
} lcx_t;
static inline IR_t * iref(IR_t * n, IR_ref_t * α, IR_ref_t * β, IR_t * na, IR_t * nb) {
    if (α) { α->node = na; memcpy(α->sz, "α", 3); }
    if (β) { β->node = nb; memcpy(β->sz, "β", 3); }
    return n;
}
IR_t * lower_sno(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
IR_t * v_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_pattern(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
#endif
