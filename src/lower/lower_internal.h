/*====================================================================================================================*/
/*====================================================================================================================*/
#ifndef LOWER_INTERNAL_H
#define LOWER_INTERNAL_H
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
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
IR_t * lower_program(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_icn(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_value_shared(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_sno(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
IR_t * lower_rku(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
IR_t * lower_pas(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
static inline IR_t * iref(IR_t * n, IR_ref_t * α, IR_ref_t * β, IR_t * na, IR_t * nb) {
    if (α) { α->node = na; memcpy(α->sz, "α", 3); }
    if (β) { β->node = nb; memcpy(β->sz, "β", 3); }
    return n;
}
lcx_t  bounded(lcx_t cx);
IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_det_call(lcx_t cx, const char * fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_mutate_writeback(lcx_t cx, const char * target, const char * pure_fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_for(lcx_t cx, const tree_t * range_t, const char * var, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_gather(lcx_t cx, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_map_grep(lcx_t cx, int is_grep, const tree_t * closure_t, const tree_t * src_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_raku_pop(lcx_t cx, const char * dst, const tree_t * arr, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * rku_fnc_junction(lcx_t cx, const char * flav, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * rku_fnc_calls(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out, int * handled);
IR_t * rku_say_print(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * rku_for_range(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_binop_bool(lcx_t cx, const tree_t * e, IR_t * bin, int b1, int b2, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_assign_fnc_lhs(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_goto_u(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_label_def(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * bb_label_landing(const char * name);
void bb_label_registry_reset(void);
void bb_label_registry_add(const char * name, IR_t * landing);
int    flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap);
IR_t * sno_conj(lcx_t cx, const tree_t * const * kids, int nk, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_e   sno_assign_kind(const tree_t * rhs_t);
IR_t * sno_fnc_call(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * v_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_pattern(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_pattern_build(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int sno_pattern_buildable(const tree_t * e);
IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e);
IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * nalloc(lcx_t cx, IR_e kind);
void   set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in);
IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β);
IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int    tt_is_relational(tree_e t);
int    tt_to_binop(tree_e t);
IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int    tm(const tree_t * e, tree_e kind, int nargs, ...);
int    tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...);
int    kind_is_resumable(IR_e t);
#endif
