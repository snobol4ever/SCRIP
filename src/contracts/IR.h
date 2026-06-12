#pragma once
#ifndef SCRIP_IR_H
#define SCRIP_IR_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
#define IR_LANG_PAS  7
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
    IR_PAT_BREAKX,
    IR_PAT_RTAB,
    IR_PAT_SPAN_VAR,
    IR_ASSIGN_LIT_S,
    IR_ASSIGN_LIT_I,
    IR_ASSIGN_VAR,
    IR_ASSIGN_CONCAT,
    IR_ASSIGN_CALL,
    IR_ASSIGN_DESCR,
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
    IR_QUERY_FRAME,
    IR_DET_WRITE,
    IR_DET_NL,
    IR_DET_IS,
    IR_DET_CMP,
    IR_DET_TYPE_TEST,
    IR_DET_FUNCTOR,
    IR_DET_ARG,
    IR_DET_UNIV,
    IR_DET_FORMAT,
    IR_DET_SUCC_PLUS,
    IR_DET_ATOM_OP,
    IR_DET_CHAR_TYPE,
    IR_DET_SORT,
    IR_DET_NUMBERVARS,
    IR_DET_TERM_STRING,
    IR_DET_COPY_TERM,
    IR_CELL_UNIFY,
    IR_CELL_CHOICE,
    IR_CELL_CALL,
    IR_CALLEE_FRAME,
    IR_CELL_CUT,
    IR_ITE_COMMIT,
    IR_ITE_GATE,
    IR_CELL_ITE,
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
    IR_PROG,
    IR_SUBJECT,
    IR_REF_INVARIANT,
    IR_PAT_MATCH,
    IR_BINOP_RELOP,
    IR_BINOP_ARITH,
    IR_BINOP_GVAR_ARITH,
    IR_BINOP_GVAR_RELOP,
    IR_BINOP_GVAR_ARITH_SLOT,
    IR_BINOP_CONCAT,
    IR_SCAN_ANY,
    IR_SCAN_MANY,
    IR_SCAN_MATCH,
    IR_SCAN_UPTO,
    IR_SCAN_FIND,
    IR_SCAN_BAL,
    IR_SCAN_TAB,
    IR_SCAN_MOVE,
    IR_SCAN_POS,
    IR_VAR_FRAME,
    IR_ASSIGN_FRAME,
    IR_VAR_FRAME_REF,
    IR_ASSIGN_FRAME_REF,
    IR_PAT_MATCH_HEAD,
    IR_PAT_MATCH_RETRY,
    IR_PAT_MATCH_ADVANCE,
    IR_PATTERN_LIT,
    IR_PATTERN_ANY,
    IR_PATTERN_NOTANY,
    IR_PATTERN_SPAN,
    IR_PATTERN_BREAK,
    IR_PATTERN_BREAKX,
    IR_PATTERN_LEN,
    IR_PATTERN_POS,
    IR_PATTERN_RPOS,
    IR_PATTERN_TAB,
    IR_PATTERN_RTAB,
    IR_PATTERN_ARB,
    IR_PATTERN_REM,
    IR_PATTERN_BAL,
    IR_PATTERN_ABORT,
    IR_PATTERN_FENCE,
    IR_PATTERN_FAIL,
    IR_PATTERN_SUCCEED,
    IR_PATTERN_ARBNO,
    IR_PATTERN_FENCE_P,
    IR_PATTERN_CAT,
    IR_PATTERN_ALT,
    IR_PATTERN_CAPTURE,
    IR_PATTERN_DEFER,
    IR_DTP_ASSIGN,
    IR_OP_COUNT
} IR_e;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct IR_t IR_t;
typedef struct IR_graph_t IR_graph_t;
typedef struct { IR_t * node; char sz[4]; } IR_ref_t;
struct IR_t {
    IR_e      op;
    IR_ref_t   γ;
    IR_ref_t   ω;
    IR_t      ** operands;
    int          n_operands;
    int          idx;
    IR_graph_t * own;
};
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    IR_t      *node;
    IR_t     **operands;
    int        n;
} bb_operand_aux_t;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    const char * sval;
    int64_t      ival;
    double       dval;
} IR_lit_t;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    DESCR_t      value;
    int64_t      counter;
    int          state;
} IR_exec_t;
/*--------------------------------------------------------------------------------------------------------------------*/
struct IR_graph_t {
    IR_t    * entry;
    IR_t   ** all;
    int            n;
    int            max;
    int            lang;
    int            nslots;
    IR_t         * body_root;
    IR_lit_t     * lit;
    IR_exec_t    * exec;
    bb_operand_aux_t *operand_aux;
    int              operand_aux_n;
    int              operand_aux_max;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
};
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes, int lang);
/*--------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * IR_sidecar_own(IR_t * nd) {
    if (!nd || !nd->own || nd->idx < 0 || nd->idx >= nd->own->n || nd->own->all[nd->idx] != nd) {
        fprintf(stderr, "IR sidecar ownership violation: nd=%p own=%p idx=%d\n", (void *)nd, nd ? (void *)nd->own : NULL, nd ? nd->idx : -1);
        abort();
    }
    return nd;
}
#define IR_LIT(nd)  ((IR_sidecar_own((IR_t *)(nd)))->own->lit[(nd)->idx])
#define IR_EXEC(nd) ((IR_sidecar_own((IR_t *)(nd)))->own->exec[(nd)->idx])
/*--------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * ir_call_arg(const IR_t * nd, int j) {
    if (!nd || j < 0) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
static inline IR_t * ir_pair_arg(const IR_t * nd, int j) {
    if (!nd || j < 0 || j > 1) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_push(IR_graph_t * cfg, DESCR_t v) {
    if (!cfg) return;
    cfg->ring_head = (cfg->ring_head + 1) % AG_RING;
    cfg->ring[cfg->ring_head] = v;
    if (cfg->ring_depth < AG_RING) cfg->ring_depth++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t ag_ring_peek(const IR_graph_t * cfg, int k) {
    if (!cfg || k < 0 || k >= cfg->ring_depth) return FAILDESCR;
    int idx = (cfg->ring_head - k + AG_RING) % AG_RING;
    return cfg->ring[idx];
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_clear(IR_graph_t * cfg) {
    if (!cfg) return;
    cfg->ring_head  = -1;
    cfg->ring_depth = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t       * IR_node_alloc(IR_graph_t * cfg, IR_e t);
int          ir_operand_push(IR_t * nd, IR_t * child);
int          bb_operand_aux_set(IR_graph_t * cfg, IR_t * nd, IR_t * const * src, int n);
IR_t * const * bb_operand_aux_get(const IR_graph_t * cfg, const IR_t * nd, int * out_n);
void         bb_reset(IR_graph_t * cfg);
/*--------------------------------------------------------------------------------------------------------------------*/
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
const char * bb_op_name(IR_e k);
#endif
