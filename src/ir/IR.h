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
#  define STRVAL(s_)   __extension__({ char *_sv_ = (char *)(s_); (DESCR_t){ .v = DT_S, .slen = _sv_ ? (uint32_t)__builtin_strlen(_sv_) : 0u, .s = _sv_ }; })
#endif
typedef enum {
    IR_ACTIVATE,
    IR_ASSIGN,
    IR_ASSIGN_VAR,
    IR_BINOP,
    IR_BINOP_RELOP_VAL,
    IR_BINOP_TEST,
    IR_BOUND,
    IR_CALL,
    IR_CALL_BUILTIN,
    IR_CALL_BUILTIN_GEN,
    IR_CALL_ICON,
    IR_CALL_SNOBOL4,
    IR_CALL_PROC_STAGED,
    IR_CALL_VALUE,
    IR_CMP_TEST,
    IR_COERCE_INTEGER,
    IR_COERCE_NUMERIC,
    IR_COERCE_REAL,
    IR_COERCE_STRING,
    IR_COFAIL,
    IR_CONJUNCTION,
    IR_CORET,
    IR_CREATE,
    IR_CUT,
    IR_DEFINE,
    IR_DEREF,
    IR_DIFFER,
    IR_DISJUNCTION,
    IR_DTP_ASSIGN,
    IR_FAIL,
    IR_FIELD_GET,
    IR_FIELD_VAR,
    IR_GALT,
    IR_GCC,
    IR_GLIT,
    IR_GOTO,
    IR_GOTO_DEFERRED,
    IR_GSUBRULE,
    IR_IDENT,
    IR_INDIRECT_GOTO,
    IR_INITIAL,
    IR_ITERATE,
    IR_KW_ASSIGN,
    IR_KW_ASSIGN_SNOBOL4,
    IR_KW_ICON,
    IR_KW_ICON_GEN,
    IR_KW_SNOBOL4,
    IR_LIMIT,
    IR_LIT_CHARSET,
    IR_LIT_INTEGER,
    IR_LIT_NAME,
    IR_LIT_REAL,
    IR_LIT_STRING,
    IR_MAKE_LIST,
    IR_MATCH,
    IR_MATCH_ABORT,
    IR_MATCH_ALTERNATE,
    IR_MATCH_ANY,
    IR_MATCH_ARB,
    IR_MATCH_ARBNO,
    IR_MATCH_ASSIGN_COND,
    IR_MATCH_ASSIGN_IMM,
    IR_MATCH_ASSIGN_SAVE,
    IR_MATCH_ATP,
    IR_MATCH_BAL,
    IR_MATCH_BEGIN,
    IR_MATCH_BREAK,
    IR_MATCH_BREAKX,
    IR_MATCH_CALLOUT,
    IR_MATCH_DEFER,
    IR_MATCH_END,
    IR_MATCH_FENCE0,
    IR_MATCH_FENCE1,
    IR_MATCH_LAMBDA,
    IR_MATCH_LEN,
    IR_MATCH_LIT,
    IR_MATCH_NOTANY,
    IR_MATCH_POS,
    IR_MATCH_REM,
    IR_MATCH_REPLACE,
    IR_MATCH_RETRY,
    IR_MATCH_RPOS,
    IR_MATCH_RTAB,
    IR_MATCH_SPAN,
    IR_MATCH_SPAN_VAR,
    IR_MATCH_TAB,
    IR_MATCH_VALUE,
    IR_MOVE_LABEL,
    IR_NULLTEST_VAR,
    IR_PATTERN_ALT,
    IR_PATTERN_CAPTURE,
    IR_PATTERN_CAT,
    IR_PATTERN_DEFER,
    IR_PROC_GEN,
    IR_PROC_VALUE,
    IR_RANDOM,
    IR_REF_INVARIANT,
    IR_REPALT,
    IR_RETURN,
    IR_REV_ASSIGN,
    IR_REV_ASSIGN_VAR,
    IR_REV_SWAP,
    IR_SCAN,
    IR_SCAN_ALTERNATE,
    IR_SCAN_ANY,
    IR_SCAN_BAL,
    IR_SCAN_ENTER,
    IR_SCAN_FIND,
    IR_SCAN_MANY,
    IR_SCAN_MATCH,
    IR_SCAN_MOVE,
    IR_SCAN_POS,
    IR_SCAN_SEQUENCE,
    IR_SCAN_TAB,
    IR_SCAN_UPTO,
    IR_STATEMENT,
    IR_STATEMENT_BEGIN,
    IR_STATEMENT_END,
    IR_SUBSCRIPT,
    IR_SUCCEED,
    IR_SUSPEND,
    IR_SWAP,
    IR_SWAP_VAR,
    IR_TO,
    IR_TO_BY,
    IR_UNMARK,
    IR_UNOP,
    IR_UNOP_TEST,
    IR_VAR,
    IR_VAR_REF,
    IR_VAR_FRAME,
    IR_ASSIGN_FRAME,
    IR_OP_COUNT
} IR_e;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_matcher(int t) {
    switch (t) { case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: case IR_MATCH_SPAN_VAR: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_LEN:
    case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_ARB: case IR_MATCH_ARBNO: case IR_MATCH_REM: case IR_MATCH_BAL: case IR_MATCH_FENCE0:
    case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ALTERNATE: case IR_MATCH_ASSIGN_IMM: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ATP:
    case IR_MATCH_CALLOUT: case IR_MATCH_DEFER: case IR_MATCH_LAMBDA: case IR_MATCH_VALUE: case IR_MATCH_BEGIN: case IR_MATCH_END: case IR_MATCH_REPLACE: case IR_MATCH_RETRY: return 1;
    default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_matcher_element(int t) {
    switch (t) { case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: case IR_MATCH_SPAN_VAR: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_LEN:
    case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_ARB: case IR_MATCH_ARBNO: case IR_MATCH_REM: case IR_MATCH_BAL: case IR_MATCH_FENCE0:
    case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ALTERNATE: case IR_MATCH_ASSIGN_IMM: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_SAVE: return 1;
    default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_scan_kind(IR_e t) { (void) t; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_call_kind(IR_e t) {
    return t == IR_CALL_PROC_STAGED || t == IR_CALL_BUILTIN || t == IR_CALL_BUILTIN_GEN || t == IR_CALL_ICON || t == IR_CALL_SNOBOL4 || t == IR_CALL_VALUE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_e ir_norm_call_kind(IR_e t) { return ir_is_call_kind(t) ? IR_CALL : t; }
typedef enum { BINOP_CAT_ARITH = 0, BINOP_CAT_RELOP = 1, BINOP_CAT_CONCAT = 2, BINOP_CAT_XREP = 3 } binop_cat_t;
typedef struct IR_t IR_t;
typedef struct IR_graph_t IR_graph_t;
typedef struct { IR_t * node; char sz[4]; } IR_ref_t;
struct IR_t {
    IR_e         op;
    IR_ref_t     γ;
    IR_ref_t     ω;
    IR_t       ** operands;
    int           n_operands;
    int           in_scan;
    int           seal;
    int           pat_static;
    union { const char * sval; int64_t ival; double dval; };
};
#define IR_LIT(nd)  (*(nd))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_define_ch8_role(const IR_t * nd) { long long v = (long long)IR_LIT(nd).ival; return (v >= 1 && v <= 4) ? (int)v : 0; }
static inline int ir_define_sr_citizen(const IR_t * nd) { return ir_define_ch8_role(nd) ? 1 : ((nd->γ.node && ir_norm_call_kind(nd->γ.node->op) == IR_CALL) ? 1 : 0); }
typedef struct {
    const char * sval;
    int64_t      ival;
    double       dval;
} IR_lit_t;
struct IR_graph_t {
    IR_t    * entry;
    IR_t   ** all;
    int            n;
    int            max;
    int            nslots;
    int            nvalue_slots;
    int            jcon_value_region;
    int            nparams;
    const char  ** pnames;
    int            nlocals;
    const char  ** lnames;
    struct IR_vslot_t { const char * name; int off; } * vslots;
    int            n_vslots;
    int            resume_slot;
    int            resumable_callable;
    int            decl_level;
    const char   * l3_ancestor_name;
    int            deterministic;
    int            zeta_mark_slot;
    IR_t         * body_root;
    IR_t        ** balias_node;
    const char  ** balias_name;
    int            n_balias;
    IR_t        ** dentry_node;
    IR_t        ** dentry_entry;
    const char  ** dentry_name;
    int            n_dentry;
    IR_t        ** alt_entry;
    IR_t        ** alt_ret;
    IR_t        ** alt_redo;
    IR_t         * alt_fail;
    int            n_alts;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
    int            zframe_graph;
    int            icn_cells_graph;
    int            zframe_pinned_base;
    int            root_graph;
    int            runtime_fragment_graph;
    int            is_variadic;
    int            rest_kind;
    IR_t         * ab_nodes[32];
    int            ab_n;
};
IR_graph_t * IR_alloc(int max_nodes);
int ir_varslot_of(const IR_graph_t * g, const char * name);
int graph_has_local(const IR_graph_t * g, const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * ir_call_arg(const IR_t * nd, int j) {
    if (!nd || j < 0) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * ir_pair_arg(const IR_t * nd, int j) {
    if (!nd || j < 0 || j > 1) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
IR_t       * IR_node_alloc(IR_graph_t * cfg, IR_e t);
int          ir_operand_push(IR_t * nd, IR_t * child);
int  ir_node_produces_value(IR_e op);
void         bb_reset(IR_graph_t * cfg);
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
void         bb_print_v(const IR_graph_t * cfg, FILE * fp, int verbose);
const char * bb_op_name(IR_e k);
#endif
