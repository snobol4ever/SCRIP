/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef LOWER_H
#define LOWER_H
#include "SM.h"
#include "stage2.h"
#include "ast.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "ast.h"
#include "../../runtime/builtins/gen_runtime.h"
#include "IR.h"
#include "bb_box.h"
#include "../runtime/builtins/gen.h"
extern int g_postfix_resume;
#define T0(t) ((t)->n > 0 ? (t)->c[0] : NULL)
#define T1(t) ((t)->n > 1 ? (t)->c[1] : NULL)
#define T2(t) ((t)->n > 2 ? (t)->c[2] : NULL)
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
typedef struct { void * data; int n; int cap; int esz; } lc_vec;
void   lc_vec_init(lc_vec * v, int esz);
void * lc_vec_push(lc_vec * v, const void * elem);
void * lc_vec_at(const lc_vec * v, int i);
#define LC_AT(v, T, i) (((T *)(v)->data)[i])
int lc_binop_code(tree_e tt);
int lc_is_binop(tree_e tt);
typedef IR_t * (*lc_lower_fn)(void * cx, const tree_t * a, IR_t * F);
IR_graph_t * lc_arg_block(IR_graph_t ** gslot, lc_lower_fn fn, void * cx, const tree_t * a);
typedef IR_graph_t * (*lc_argblk_fn)(void * cx, const tree_t * a);
void lc_call_argblks(IR_t * call, double dv, int nargs, lc_argblk_fn mk, void * cx, const tree_t * const * args);
stage2_t *lower_sno_stage2(const tree_t *prog);
stage2_t *lower_icon_stage2(const tree_t *prog);
stage2_t *lower_pl_stage2(const tree_t *prog);
stage2_t *lower_raku_stage2(const tree_t *prog);
stage2_t *lower_pascal_stage2(const tree_t *prog);
typedef struct { bb_node_t gen[2]; int which; } alt_dcg_t;
typedef struct { bb_node_t left; bb_node_t right; BinopKind op; int is_relop; DESCR_t left_val; DESCR_t right_val; int phase; } binop_dcg_t;
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
struct GeneratorState;
IR_graph_t *lower_proc_gen(struct GeneratorState *gs);
#endif
