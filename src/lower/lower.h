#ifndef LOWER_H
#define LOWER_H
#include "SM.h"
#include "stage2.h"
#include "ast.h"
#include "../../parser/snobol4/scrip_cc.h"
#include "ast.h"
#include "../../runtime/builtins/gen_runtime.h"
#include "ast_clone.h"
#include "IR.h"
#include "bb_box.h"
#include "../runtime/builtins/gen.h"
/*====================================================================================================================*/
extern int g_icn_globals_nv;
extern int g_icn_postfix_resume;
/*====================================================================================================================*/
#define T0(t) ((t)->n > 0 ? (t)->c[0] : NULL)
#define T1(t) ((t)->n > 1 ? (t)->c[1] : NULL)
#define T2(t) ((t)->n > 2 ? (t)->c[2] : NULL)
/*====================================================================================================================*/
stage2_t *lower_stage2(const tree_t *prog);
/*====================================================================================================================*/
typedef struct { bb_node_t gen[2]; int which; } alt_dcg_t;
typedef struct { bb_node_t left; bb_node_t right; BinopKind op; int is_relop; DESCR_t left_val; DESCR_t right_val; int phase; } binop_dcg_t;
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
struct GeneratorState;
IR_graph_t *lower_proc_gen(struct GeneratorState *gs);
#endif
