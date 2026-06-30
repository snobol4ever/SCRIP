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
    IR_ALT,
    IR_ASSIGN,
    IR_BINOP,
    IR_BINOP_GENERIC,
    IR_BINOP_RELOP,
    IR_CALL,
    IR_CALL_BUILTIN,
    IR_CALL_BYNAME,
    IR_CALL_GVAR_USERPROC,
    IR_CALL_PROC_STAGED,
    IR_CALL_USERPROC,
    IR_COFAIL,
    IR_CONJ,
    IR_CORET,
    IR_CREATE,
    IR_DEREF,
    IR_ENTER_INIT,
    IR_FAIL,
    IR_FIELD,
    IR_GOTO,
    IR_INDIRECT_GOTO,
    IR_KEYWORD,
    IR_LIT_CHARSET,
    IR_LIT_INTEGER,
    IR_LIT_REAL,
    IR_LIT_STRING,
    IR_MAKE_LIST,
    IR_MOVE,
    IR_NOT,
    IR_PROC_GEN,
    IR_RESUME_VALUE,
    IR_RETURN,
    IR_SCAN_SWAP,
    IR_SUBSCRIPT,
    IR_SUCCEED,
    IR_TERNOP,
    IR_TO,
    IR_UNOP,
    IR_UNOP_GENERIC,
    IR_UNOP_TEST,
    IR_UNREACHABLE,
    IR_VAR,
    IR_OP_COUNT
} IR_e;
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_scan_kind(IR_e t) { (void) t; return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_call_kind(IR_e t) { return t == IR_CALL_PROC_STAGED || t == IR_CALL_USERPROC || t == IR_CALL_BYNAME || t == IR_CALL_BUILTIN || t == IR_CALL_GVAR_USERPROC; }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline IR_e ir_norm_call_kind(IR_e t) { return ir_is_call_kind(t) ? IR_CALL : t; }
/*--------------------------------------------------------------------------------------------------------------------*/
typedef enum { BINOP_CAT_ARITH = 0, BINOP_CAT_RELOP = 1, BINOP_CAT_CONCAT = 2 } binop_cat_t;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct IR_t IR_t;
typedef struct IR_graph_t IR_graph_t;
typedef struct { IR_t * node; char sz[4]; } IR_ref_t;
struct IR_t {
    IR_e         op;
    IR_ref_t     γ;
    IR_ref_t     ω;
    IR_t       ** operands;
    int           n_operands;
    int           tmp;
    union { const char * sval; int64_t ival; double dval; };
};
/*--------------------------------------------------------------------------------------------------------------------*/
#define IR_LIT(nd)  (*(nd))
#define IR_EXEC(nd) (*(nd))
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    const char * sval;
    int64_t      ival;
    double       dval;
} IR_lit_t;
/*--------------------------------------------------------------------------------------------------------------------*/
struct IR_graph_t {
    IR_t    * entry;
    IR_t   ** all;
    int            n;
    int            max;
    int            lang;
    int            nslots;
    int            nvalue_slots;
    int            jcon_value_region;
    IR_t         * body_root;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
};
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes, int lang);
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
int  ir_node_produces_value(IR_e op);
void ir_tmp_slot_assign(IR_graph_t * g);
void ir_tmp_slot_assign_flat(IR_graph_t * g);
void ir_jcon_slot_assign(IR_graph_t * g);
void         bb_reset(IR_graph_t * cfg);
/*--------------------------------------------------------------------------------------------------------------------*/
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
const char * bb_op_name(IR_e k);
#endif
