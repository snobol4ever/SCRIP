#ifndef LOWER_GRAPH_H
#define LOWER_GRAPH_H
#include "BB.h"
#include "../processor/bb_box.h"
#include "../runtime/interp/gen.h"
struct tree_t;
typedef struct { bb_node_t gen[2]; int which; } alt_dcg_t;
typedef struct { bb_node_t gen; int64_t max; int64_t count; } lim_dcg_t;
typedef struct { bb_node_t left; bb_node_t right; BinopKind op; int is_relop; DESCR_t left_val; DESCR_t right_val; int phase; } binop_dcg_t;
BB_graph_t *lower_upto(const char *cset, const char *hay);
BB_graph_t *lower_to(int64_t lo, int64_t hi);
BB_graph_t *lower_to_nested(to_nested_state_t *z);
BB_graph_t *lower_to_by(int64_t lo, int64_t hi, int64_t step);
BB_graph_t *lower_iterate(const char *str, int64_t len);
BB_graph_t *lower_alternate(bb_node_t left, bb_node_t right);
BB_graph_t *lower_limit(bb_node_t gen, int64_t max);
BB_graph_t *lower_binop(bb_node_t left, bb_node_t right, BinopKind op, int is_relop);
DESCR_t     binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
struct GeneratorState;
BB_graph_t *lower_proc_gen(struct GeneratorState *gs);
BB_graph_t *lower_proc_body(struct tree_t *proc);
BB_graph_t *lower_expr_top(struct tree_t *e);
int  kind_is_resumable(BB_op_t t);
struct BB_t *lower_expr_threaded(struct BB_graph_t *bbg, struct tree_t *e,
                                     struct BB_t *γ_in, struct BB_t *ω_in,
                                     struct BB_t **α_out, struct BB_t **β_out);
struct BB_t *lower_expr_threaded_b(struct BB_graph_t *bbg, struct tree_t *e,
                                       struct BB_t *γ_in, struct BB_t *ω_in,
                                       struct BB_t **α_out, struct BB_t **β_out, int bounded);
struct BB_t *gen_leaf(struct BB_t *bb, struct BB_t *γ_in, struct BB_t *ω_in,
                      struct BB_t **α_out, struct BB_t **β_out, int bounded);
BB_t *lower_expr_node(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Intlit(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Reallit(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Stringlit(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Csetlit(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Global(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Binop(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Binop_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Lconcat_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Sectionop_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Idx_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Idx_set_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_If(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_If_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_ToBy(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_ToBy_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Every(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Every_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Compound(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_ProcBody(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Call(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Field(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Sectionop(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Alt(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Alt_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Conjunction(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Conjunction_ag(BB_graph_t *bbg, struct tree_t *e, BB_t *γ_in, BB_t *ω_in, BB_t **α_out, BB_t **β_out);
BB_t *lower_new_Not(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_While(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Until(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Repeat(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Limitation(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Scan(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Case(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Return(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Suspend(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Break(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Next(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Initial(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Unop(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_AugOp(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Create(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Mutual(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Key(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Invocable(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Link(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_RepAlt(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_CoexpList(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Var(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Keyword(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Assign(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Swap(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_SeqExpr(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Decl(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Idx(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Identical(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_ProcFail(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_CsetBinop(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Record(BB_graph_t *bbg, struct tree_t *e);
BB_t *lower_new_Makelist(BB_graph_t *bbg, struct tree_t *e);
#endif
