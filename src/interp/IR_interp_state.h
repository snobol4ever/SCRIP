#ifndef SCRIP_IR_INTERP_STATE_H
#define SCRIP_IR_INTERP_STATE_H
/*--------------------------------------------------------------------------------------------------------------------*/
#include "IR.h"
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    DESCR_t value; int64_t counter; int state;
    void   *resolve_cs;
    int     ch_cur;
    int     ch_mark;
    void   *ch_saved_env;
    void   *ch_last_body;
    void   *ch_last_act;
    void   *ch_cp;
    void   *ch_cut_barrier;
    void  **ch_body_snaps;
    int     ch_nbodies;
} bb_node_state_t;
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { IR_t * root; int succ_idx; int fail_idx; int is_terminal; } stmt_t;
typedef struct { stmt_t * stmts; int n; int entry_idx; } prog_t;
typedef struct { IR_t ** kids; int nkids; IR_graph_t * inner; int * pos_stack; int cap; int saved_delta; } bb_arbno_state_t;
typedef struct { IR_t ** goals; int ngoals; } bb_conj_state_t;
typedef struct { IR_t * cond; IR_t * then_; IR_t * else_; IR_t * then_root; IR_t * else_root; IR_t * cond_root; void * cp_mark; int committed; long seen_seq; } bb_ite_state_t;
typedef struct { IR_graph_t * goal_g; IR_t * catcher; IR_graph_t * rec_g; } bb_catch_state_t;
typedef struct { IR_graph_t ** bodies; int nbodies; int cur; int mark; void * saved_env;
                 IR_graph_t * last_body; void * last_act;
                 void * cp;
                 void * cut_barrier;
                 long * idx_key;
                 int idx_ok;
               } bb_choice_state_t;
#define RESOLVE_IDX_VAR    0L
#define RESOLVE_IDX_NOKEY  (-1L)
#define RESOLVE_IDX_CLS_ATOM (1L << 60)
#define RESOLVE_IDX_CLS_INT  (2L << 60)
#define RESOLVE_IDX_CLS_FLT  (3L << 60)
#define RESOLVE_IDX_CLS_CMP  (4L << 60)
#define RESOLVE_IDX_PAYLOAD_MASK ((1L << 60) - 1L)
#define RESOLVE_IDX_ATOM(id)        (RESOLVE_IDX_CLS_ATOM | ((long)(id) & RESOLVE_IDX_PAYLOAD_MASK))
#define RESOLVE_IDX_INT(v)          (RESOLVE_IDX_CLS_INT  | ((long)(v)  & RESOLVE_IDX_PAYLOAD_MASK))
#define RESOLVE_IDX_FLT             (RESOLVE_IDX_CLS_FLT)
#define RESOLVE_IDX_CMP(fn,ar)      (RESOLVE_IDX_CLS_CMP  | ((((long)(fn) << 16) | ((long)(ar) & 0xFFFF)) & RESOLVE_IDX_PAYLOAD_MASK))
typedef struct { IR_t ** args; int nargs; const char * callee; int arity; void * cs; } bb_goal_state_t;
typedef struct { int nclauses; int arity; int mark_slot; IR_t * args[2]; IR_t * consts[4][2]; } pl_gz_choice_state_t;
typedef struct { void * graph_key; int base; int arity; int nlocals; int mark_slot; IR_t * body_head; IR_t * frame_node; void * lblA; void * lblB; int nchild;
                 int nclauses; IR_t * clause_head[4]; } pl_gz_callee_t;
typedef struct { pl_gz_callee_t * callee; int nargs; IR_t * args[2]; int child_slot; } pl_gz_call_state_t;
typedef struct { IR_t * cond_head; IR_t * then_head; IR_t * else_head; int gate_slot; } pl_gz_ite_state_t;
typedef struct { IR_graph_t * gcfg; IR_t * tmpl; IR_t * result; IR_t * goal_node; } bb_findall_state_t;
typedef struct { IR_t ** kids; int nkids; } bb_pat_kids_state_t;
static inline int bb_pat_nkids(const IR_t * nd) {
    if (!nd) return 0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    return zk ? zk->nkids : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * bb_pat_kid(const IR_t * nd, int i) {
    if (!nd) return (IR_t *)0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    if (!zk || i < 0 || i >= zk->nkids) return (IR_t *)0;
    return zk->kids[i];
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(IR_graph_t * cfg);
void              bb_restore_state(IR_graph_t * cfg, bb_node_state_t * snap);
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
