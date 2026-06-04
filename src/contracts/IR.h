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
    IR_OP_COUNT
} IR_e;
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    IR_t      *node;
    IR_t     **operands;
    int        n;
} bb_operand_aux_t;
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes, int lang);
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
int          bb_operand_aux_set(IR_graph_t * cfg, IR_t * nd, IR_t * const * src, int n);
IR_t * const * bb_operand_aux_get(const IR_graph_t * cfg, const IR_t * nd, int * out_n);
void         bb_reset(IR_graph_t * cfg);
/*--------------------------------------------------------------------------------------------------------------------*/
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
const char * bb_op_name(IR_e k);
#endif
