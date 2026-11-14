#ifndef LOWER_COMMON_H
#define LOWER_COMMON_H
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ir_fwd.h"
#include "ir/ir_node.h"
#include "ir/bb_box_bb_node.h"
#include "runtime/builtins/gen.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#define T0(t) ((t)->n > 0 ? (t)->c[0] : NULL)
#define T1(t) ((t)->n > 1 ? (t)->c[1] : NULL)
void lc_γ_to(IR_t *nd, IR_t *t);
void lc_ω_to(IR_t *nd, IR_t *t);
void lc_γ_to_β(IR_t *nd, IR_t *t);
void lc_γ_tag_β(IR_t *nd);
void lc_ω_to_β(IR_t *nd, IR_t *t);
void lc_γ_to_α(IR_t *nd, IR_t *t);
void lc_ω_to_α(IR_t *nd, IR_t *t);
IR_t *lc_build(IR_graph_t *g, IR_e op, IR_t *γ, IR_t *ω);
const tree_t *lc_stmt_subj(const tree_t *s);
int lp_s_int(const tree_t *s, const char *tag);
tree_t *lp_s_expr(const tree_t *s, const char *tag);
const char *lp_strdup(const char *s);
void bb_label_registry_reset(void);
void bb_label_registry_add(const char *name, IR_t *landing);
IR_t *bb_label_landing(const char *name);
int bb_label_registry_count(void);
const char *bb_label_registry_get(int i, IR_t **landing);
void bb_src_note(const IR_t *nd, const char *src, int line);
const char *bb_src_of(const IR_t *nd);
int bb_line_of(const IR_t *nd);
void bb_src_reset(void);
int lc_binop_code(tree_e tt);
int lc_is_binop(tree_e tt);
typedef IR_t * (*lc_lower_fn)(void * cx, const tree_t * a, IR_t * F);
IR_graph_t * lc_arg_block(IR_graph_t ** gslot, lc_lower_fn fn, void * cx, const tree_t * a);
typedef struct { bb_node_t left; bb_node_t right; BinopKind op; int is_relop; DESCR_t left_val; DESCR_t right_val; int phase; } binop_dcg_t;
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
int is_global(const char * name);
#endif
