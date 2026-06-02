#ifndef LOWER_H
#define LOWER_H
#include "SM.h"
#include "stage2.h"
#include "../../ast/ast.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "../ast/ast.h"
#include "../../runtime/builtins/gen_runtime.h"
#include "ast_clone.h"
#include "IR.h"
#include "../processor/bb_box.h"
#include "../runtime/builtins/gen.h"
/*====================================================================================================================*/
char *kw_canonicalize(const char *raw);
void expression_scope_walk(Scope *sc, tree_t *e);
#define T0(t) ((t)->n > 0 ? (t)->c[0] : NULL)
#define T1(t) ((t)->n > 1 ? (t)->c[1] : NULL)
#define T2(t) ((t)->n > 2 ? (t)->c[2] : NULL)
/*====================================================================================================================*/
stage2_t *lower(const tree_t *prog);
/*====================================================================================================================*/
struct tree_t;
IR_graph_t *lower_pl_predicate(struct tree_t *choice);
IR_t *lower_pl_threaded(IR_graph_t *bbg, const struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
/*====================================================================================================================*/
IR_graph_t * IR_lower_pat(const tree_t * pat_tree);
/*====================================================================================================================*/
struct tree_t;
typedef struct { bb_node_t gen[2]; int which; } alt_dcg_t;
typedef struct { bb_node_t gen; int64_t max; int64_t count; } lim_dcg_t;
typedef struct { bb_node_t left; bb_node_t right; BinopKind op; int is_relop; DESCR_t left_val; DESCR_t right_val; int phase; } binop_dcg_t;
IR_graph_t *lower_upto(const char *cset, const char *hay);
IR_graph_t *lower_to(int64_t lo, int64_t hi);
IR_graph_t *lower_to_nested(to_nested_state_t *z);
IR_graph_t *lower_to_by(int64_t lo, int64_t hi, int64_t step);
IR_graph_t *lower_iterate(const char *str, int64_t len);
IR_graph_t *lower_alternate(bb_node_t left, bb_node_t right);
IR_graph_t *lower_limit(bb_node_t gen, int64_t max);
IR_graph_t *lower_binop(bb_node_t left, bb_node_t right, BinopKind op, int is_relop);
DESCR_t     binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
struct GeneratorState;
IR_graph_t *lower_proc_gen(struct GeneratorState *gs);
IR_graph_t *lower_proc_body(struct tree_t *proc);
IR_graph_t *lower_expr_top(struct tree_t *e);
int  kind_is_resumable(IR_e t);
struct IR_t *lower_expr_threaded(struct IR_graph_t *bbg, struct tree_t *e,
                                     struct IR_t *γ_in, struct IR_t *ω_in,
                                     struct IR_t **α_out, struct IR_t **β_out);
struct IR_t *lower_expr_threaded_b(struct IR_graph_t *bbg, struct tree_t *e,
                                       struct IR_t *γ_in, struct IR_t *ω_in,
                                       struct IR_t **α_out, struct IR_t **β_out, int bounded);
struct IR_t *gen_leaf(struct IR_t *bb, struct IR_t *γ_in, struct IR_t *ω_in,
                      struct IR_t **α_out, struct IR_t **β_out, int bounded);
IR_t *lower_expr_node(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Intlit(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Reallit(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Stringlit(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Csetlit(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Global(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Binop(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Binop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Lconcat_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Sectionop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Idx_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Idx_set_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_If(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_If_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_ToBy(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_ToBy_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Every(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Every_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Compound(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_ProcBody(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Call(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Field(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Sectionop(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Alt(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Alt_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Conjunction(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Conjunction_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
IR_t *lower_new_Not(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_While(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Until(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Repeat(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Limitation(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Scan(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Case(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Return(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Suspend(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Break(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Next(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Initial(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Unop(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_AugOp(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Create(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Mutual(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Key(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Invocable(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Link(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_RepAlt(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_CoexpList(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Var(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Keyword(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Assign(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Swap(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_SeqExpr(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Decl(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Idx(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Identical(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_ProcFail(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_CsetBinop(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Record(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Makelist(IR_graph_t *bbg, struct tree_t *e);
#endif
