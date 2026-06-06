/*====================================================================================================================*/
/*====================================================================================================================*/
#ifndef LOWER_INTERNAL_H
#define LOWER_INTERNAL_H
#include "lower.h"
/*--------------------------------------------------------------------------------------------------------------------*/
typedef enum { ROLE_VALUE = 0, ROLE_PATTERN = 1, ROLE_GOAL = 2 } lower_role_e;
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
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower2_icn(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_value(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e);
IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * nalloc(lcx_t cx, IR_e kind);
void   set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in);
IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β);
IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int    tm(const tree_t * e, tree_e kind, int nargs, ...);
int    tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...);
#endif
