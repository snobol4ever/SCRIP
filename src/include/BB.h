#pragma once
#ifndef SCRIP_IR_H
#define SCRIP_IR_H
#include <stdint.h>
#include <stdio.h>
#include "descr.h"
#ifndef NULVCL
#  define NULVCL       ((DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" })
#endif
#ifndef INTVAL
#  define INTVAL(i_)   ((DESCR_t){ .v = DT_I, .i = (int64_t)(i_) })
#endif
#ifndef REALVAL
#  define REALVAL(r_)  ((DESCR_t){ .v = DT_R, .r = (double)(r_) })
#endif
#ifndef STRVAL
#  define STRVAL(s_)   ((DESCR_t){ .v = DT_S, .slen = 0, .s = (s_) })
#endif
#define BB_LANG_SNO  1
#define BB_LANG_SCO  2
#define BB_LANG_REB  3
#define BB_LANG_ICN  4
#define BB_LANG_PL   5
#define BB_LANG_RKU  6
typedef enum {
    BB_LIT_I,
    BB_LIT_S,
    BB_LIT_F,
    BB_LIT_NUL,
    BB_VAR,
    BB_ASSIGN,
    BB_AUGOP,
    BB_BINOP,
    BB_UNOP,
    BB_CALL,
    BB_SEQ,
    BB_FAIL,
    BB_SUCCEED,
    BB_GOTO,
    BB_RETURN,
    BB_IF,
    BB_CONJ,
    BB_TO_BY,
    BB_EVERY,
    BB_WHILE,
    BB_UNTIL,
    BB_REPEAT,
    BB_ALT,
    BB_SIZE,
    BB_CASE,
    BB_LIMIT,
    BB_SUSPEND,
    BB_PROC,
    BB_SCAN,
    BB_NONNULL,
    BB_INTERROGATE,
    BB_NOT,
    BB_PAT_LIT,
    BB_PAT_ANY,
    BB_PAT_SPAN,
    BB_PAT_BREAK,
    BB_PAT_ARB,
    BB_PAT_ARBNO,
    BB_PAT_CAT,
    BB_PAT_ALT,
    BB_PAT_ASSIGN_IMM,
    BB_PAT_ASSIGN_COND,
    BB_PAT_LEN,
    BB_PAT_NOTANY,
    BB_PAT_POS,
    BB_PAT_TAB,
    BB_PAT_REM,
    BB_PAT_FENCE,
    BB_PAT_ABORT,
    BB_PAT_CALLOUT,
    BB_PAT_DEFER,
    BB_CHOICE,
    BB_UNIFY,
    BB_CUT,
    BB_GOAL,
    BB_BUILTIN,
    BB_LOGICVAR,
    BB_ATOM,
    BB_STRUCT,
    BB_ARITH,
    BB_DISJ,
    BB_GCONJ,
    BB_ITE,
    BB_CATCH,
    BB_TO,
    BB_UPTO,
    BB_ITERATE,
    BB_GEN_ALT,
    BB_GEN_BINOP,
    BB_TO_NESTED,
    BB_PROC_GEN,
    BB_BREAK,
    BB_NEXT,
    BB_IDENTICAL,
    BB_NULL_TEST,
    BB_RANDOM,
    BB_NEG,
    BB_POS,
    BB_CSET_COMPL,
    BB_CSET_UNION,
    BB_CSET_DIFF,
    BB_CSET_INTER,
    BB_GEN_SCAN,
    BB_KEYWORD,
    BB_BINOP_GEN,
    BB_IDX,
    BB_SECTION,
    BB_LIST_BANG,
    BB_RECORD_DEF,
    BB_FIELD_GET,
    BB_FIELD_SET,
    BB_IDX_SET,
    BB_KEY_GEN,
    BB_SWAP,
    BB_SEQ_EXPR,
    BB_INITIAL,
    BB_LCONCAT,
    BB_FIND_GEN,
    BB_SEQ_GEN,
    BB_NFA_CHAR,
    BB_NFA_ANY,
    BB_NFA_CLASS,
    BB_NFA_SPLIT,
    BB_NFA_EPS,
    BB_NFA_BOL,
    BB_NFA_EOL,
    BB_NFA_CAP_OPEN,
    BB_NFA_CAP_CLOSE,
    BB_NFA_ACCEPT,
    BB_PAT_ATP,
    BB_OP_COUNT
} BB_op_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct BB_t BB_t;
struct BB_t {
    BB_op_t      t;
    BB_t       * α;
    BB_t       * β;
    BB_t       * γ;
    BB_t       * ω;
    const char * sval;
    int64_t      ival;
    double       dval;
    DESCR_t      value;
    int64_t      counter;
    int          state;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    BB_t      *node;
    BB_t     **operands;
    int        n;
} bb_operand_aux_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct BB_graph_t {
    BB_t    * entry;
    BB_t   ** all;
    int            n;
    int            max;
    int            lang;
    bb_operand_aux_t *operand_aux;
    int              operand_aux_n;
    int              operand_aux_max;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
} BB_graph_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
BB_graph_t * BB_alloc(int max_nodes, int lang);
static inline void ag_ring_push(BB_graph_t * cfg, DESCR_t v) {
    if (!cfg) return;
    cfg->ring_head = (cfg->ring_head + 1) % AG_RING;
    cfg->ring[cfg->ring_head] = v;
    if (cfg->ring_depth < AG_RING) cfg->ring_depth++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t ag_ring_peek(const BB_graph_t * cfg, int k) {
    if (!cfg || k < 0 || k >= cfg->ring_depth) return FAILDESCR;
    int idx = (cfg->ring_head - k + AG_RING) % AG_RING;
    return cfg->ring[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_clear(BB_graph_t * cfg) {
    if (!cfg) return;
    cfg->ring_head  = -1;
    cfg->ring_depth = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
BB_t       * BB_node_alloc(BB_graph_t * cfg, BB_op_t t);
int          bb_operand_aux_set(BB_graph_t * cfg, BB_t * nd, BB_t * const * src, int n);
BB_t * const * bb_operand_aux_get(const BB_graph_t * cfg, const BB_t * nd, int * out_n);
void         bb_reset(BB_graph_t * cfg);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { BB_t ** kids; int nkids; BB_graph_t * inner; int * pos_stack; int cap; int saved_delta; } bb_arbno_state_t;
typedef struct { BB_t ** goals; int ngoals; } bb_conj_state_t;
typedef struct { BB_t * cond; BB_t * then_; BB_t * else_; } bb_ite_state_t;
typedef struct { BB_graph_t * goal_g; BB_t * catcher; BB_graph_t * rec_g; } bb_catch_state_t;
typedef struct { BB_graph_t ** bodies; int nbodies; int cur; int mark; void * saved_env;
                 BB_graph_t * last_body; void * last_act;
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
typedef struct { BB_t ** args; int nargs; const char * callee; int arity; void * cs; } bb_goal_state_t;
typedef struct { BB_graph_t * gcfg; BB_t * tmpl; BB_t * result; } bb_findall_state_t;
typedef struct { BB_t ** kids; int nkids; } bb_pat_kids_state_t;
static inline int bb_pat_nkids(const BB_t * nd) {
    if (!nd) return 0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    return zk ? zk->nkids : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline BB_t * bb_pat_kid(const BB_t * nd, int i) {
    if (!nd) return (BB_t *)0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    if (!zk || i < 0 || i >= zk->nkids) return (BB_t *)0;
    return zk->kids[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(BB_graph_t * cfg);
void              bb_restore_state(BB_graph_t * cfg, bb_node_state_t * snap);
void         BB_free(BB_graph_t * cfg);
void         bb_print(const BB_graph_t * cfg, FILE * fp);
const char * bb_op_name(BB_op_t k);
#endif
