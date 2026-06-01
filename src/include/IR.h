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
#define IR_LANG_SNO  1
#define IR_LANG_SCO  2
#define IR_LANG_REB  3
#define IR_LANG_ICN  4
#define IR_LANG_PL   5
#define IR_LANG_RKU  6
typedef enum {
    IR_LIT_I,
    IR_LIT_S,
    IR_LIT_F,
    IR_LIT_NUL,
    IR_VAR,
    IR_ASSIGN,
    IR_AUGOP,
    IR_BINOP,
    IR_UNOP,
    IR_CALL,
    IR_SEQ,
    IR_FAIL,
    IR_SUCCEED,
    IR_GOTO,
    IR_RETURN,
    IR_IF,
    IR_CONJ,
    IR_TO_BY,
    IR_EVERY,
    IR_WHILE,
    IR_UNTIL,
    IR_REPEAT,
    IR_ALT,
    IR_SIZE,
    IR_CASE,
    IR_LIMIT,
    IR_SUSPEND,
    IR_PROC,
    IR_SCAN,
    IR_NONNULL,
    IR_INTERROGATE,
    IR_NOT,
    IR_PAT_LIT,
    IR_PAT_ANY,
    IR_PAT_SPAN,
    IR_PAT_BREAK,
    IR_PAT_ARB,
    IR_PAT_ARBNO,
    IR_PAT_CAT,
    IR_PAT_ALT,
    IR_PAT_ASSIGN_IMM,
    IR_PAT_ASSIGN_COND,
    IR_PAT_LEN,
    IR_PAT_NOTANY,
    IR_PAT_POS,
    IR_PAT_TAB,
    IR_PAT_REM,
    IR_PAT_FENCE,
    IR_PAT_ABORT,
    IR_PAT_CALLOUT,
    IR_PAT_DEFER,
    IR_CHOICE,
    IR_UNIFY,
    IR_CUT,
    IR_GOAL,
    IR_BUILTIN,
    IR_LOGICVAR,
    IR_ATOM,
    IR_STRUCT,
    IR_ARITH,
    IR_DISJ,
    IR_GCONJ,
    IR_ITE,
    IR_CATCH,
    IR_TO,
    IR_UPTO,
    IR_ITERATE,
    IR_GEN_ALT,
    IR_GEN_BINOP,
    IR_TO_NESTED,
    IR_PROC_GEN,
    IR_BREAK,
    IR_NEXT,
    IR_IDENTICAL,
    IR_NULL_TEST,
    IR_RANDOM,
    IR_NEG,
    IR_POS,
    IR_CSET_COMPL,
    IR_CSET_UNION,
    IR_CSET_DIFF,
    IR_CSET_INTER,
    IR_GEN_SCAN,
    IR_KEYWORD,
    IR_BINOP_GEN,
    IR_IDX,
    IR_SECTION,
    IR_LIST_BANG,
    IR_RECORD_DEF,
    IR_FIELD_GET,
    IR_FIELD_SET,
    IR_IDX_SET,
    IR_KEY_GEN,
    IR_SWAP,
    IR_SEQ_EXPR,
    IR_INITIAL,
    IR_LCONCAT,
    IR_FIND_GEN,
    IR_SEQ_GEN,
    IR_NFA_CHAR,
    IR_NFA_ANY,
    IR_NFA_CLASS,
    IR_NFA_SPLIT,
    IR_NFA_EPS,
    IR_NFA_BOL,
    IR_NFA_EOL,
    IR_NFA_CAP_OPEN,
    IR_NFA_CAP_CLOSE,
    IR_NFA_ACCEPT,
    IR_PAT_ATP,
    IR_DO_WHILE,
    IR_PAT_BAL,
    IR_GATHER,
    IR_MAP,
    IR_GREP,
    IR_SNO_PROG,
    IR_SUBJECT,
    IR_REF_INVARIANT,
    IR_PAT_MATCH,
    IR_OP_COUNT
} IR_e;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct IR_t IR_t;
struct IR_t {
    IR_e      t;
    IR_t       * α;
    IR_t       * β;
    IR_t       * γ;
    IR_t       * ω;
    const char * sval;
    int64_t      ival;
    double       dval;
    DESCR_t      value;
    int64_t      counter;
    int          state;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    IR_t      *node;
    IR_t     **operands;
    int        n;
} bb_operand_aux_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct IR_graph_t {
    IR_t    * entry;
    IR_t   ** all;
    int            n;
    int            max;
    int            lang;
    int            nslots;
    IR_t         * body_root;
    bb_operand_aux_t *operand_aux;
    int              operand_aux_n;
    int              operand_aux_max;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
} IR_graph_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes, int lang);
static inline void ag_ring_push(IR_graph_t * cfg, DESCR_t v) {
    if (!cfg) return;
    cfg->ring_head = (cfg->ring_head + 1) % AG_RING;
    cfg->ring[cfg->ring_head] = v;
    if (cfg->ring_depth < AG_RING) cfg->ring_depth++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t ag_ring_peek(const IR_graph_t * cfg, int k) {
    if (!cfg || k < 0 || k >= cfg->ring_depth) return FAILDESCR;
    int idx = (cfg->ring_head - k + AG_RING) % AG_RING;
    return cfg->ring[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_clear(IR_graph_t * cfg) {
    if (!cfg) return;
    cfg->ring_head  = -1;
    cfg->ring_depth = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t       * IR_node_alloc(IR_graph_t * cfg, IR_e t);
int          bb_operand_aux_set(IR_graph_t * cfg, IR_t * nd, IR_t * const * src, int n);
IR_t * const * bb_operand_aux_get(const IR_graph_t * cfg, const IR_t * nd, int * out_n);
void         bb_reset(IR_graph_t * cfg);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { IR_t * root; int succ_idx; int fail_idx; int is_terminal; } sno_stmt_t;
typedef struct { sno_stmt_t * stmts; int n; int entry_idx; } sno_prog_t;
typedef struct { IR_t ** kids; int nkids; IR_graph_t * inner; int * pos_stack; int cap; int saved_delta; } bb_arbno_state_t;
typedef struct { IR_t ** goals; int ngoals; } bb_conj_state_t;
typedef struct { IR_t * cond; IR_t * then_; IR_t * else_; } bb_ite_state_t;
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
typedef struct { IR_graph_t * gcfg; IR_t * tmpl; IR_t * result; } bb_findall_state_t;
typedef struct { IR_t ** kids; int nkids; } bb_pat_kids_state_t;
static inline int bb_pat_nkids(const IR_t * nd) {
    if (!nd) return 0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    return zk ? zk->nkids : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * bb_pat_kid(const IR_t * nd, int i) {
    if (!nd) return (IR_t *)0;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)(intptr_t)nd->counter;
    if (!zk || i < 0 || i >= zk->nkids) return (IR_t *)0;
    return zk->kids[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(IR_graph_t * cfg);
void              bb_restore_state(IR_graph_t * cfg, bb_node_state_t * snap);
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
const char * bb_op_name(IR_e k);
#endif
