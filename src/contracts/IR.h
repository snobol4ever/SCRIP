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
typedef enum {
    IR_ACTIVATE,
    IR_ASSIGN,
    IR_ASSIGN_VAR,
    IR_BINOP,
    IR_BINOP_TEST,
    IR_CALL,
    IR_CALL_BUILTIN,
    IR_CALL_PROC_STAGED,
    IR_CALL_VALUE,
    IR_COFAIL,
    IR_CONJUNCTION,
    IR_CORET,
    IR_CREATE,
    IR_DEREF,
    IR_DISJUNCTION,
    IR_INITIAL,
    IR_FAIL,
    IR_FIELD_GET,
    IR_FIELD_VAR,
    IR_NULLTEST_VAR,
    IR_GOTO,
    IR_INDIRECT_GOTO,
    IR_ITERATE,
    IR_KEYWORD_ICON,
    IR_KEYWORD_ICON_GEN,
    IR_KEYWORD_SNOBOL4,
    IR_KEYWORD_ASSIGN,
    IR_LIT_CHARSET,
    IR_LIT_INTEGER,
    IR_LIT_REAL,
    IR_LIT_STRING,
    IR_MAKE_LIST,
    IR_MOVE_LABEL,
    IR_PROC_GEN,
    IR_PROC_VALUE,
    IR_RANDOM,
    IR_RETURN,
    IR_LIMIT,
    IR_REPALT,
    IR_REV_ASSIGN,
    IR_REV_ASSIGN_VAR,
    IR_SCAN,
    IR_SCAN_ENTER,
    IR_SCAN_ANY,
    IR_SCAN_BAL,
    IR_SCAN_FIND,
    IR_SCAN_MANY,
    IR_SCAN_MATCH,
    IR_SCAN_MOVE,
    IR_SCAN_POS,
    IR_SCAN_TAB,
    IR_SCAN_UPTO,
    IR_SUBSCRIPT,
    IR_SUCCEED,
    IR_SUSPEND,
    IR_SWAP,
    IR_SWAP_VAR,
    IR_TO,
    IR_TO_BY,
    IR_UNOP,
    IR_UNOP_TEST,
    IR_VAR,
    IR_VAR_REF,
    /* ---- SNOBOL4 pattern family (SN4-PAT) ---------------------------------------------------
     * Re-added onto the post-GZ#5 spine. Amputated wholesale by 8de0fb46 (GZ#5 ENUM-AMPUTATION);
     * design recovered from parent 41b53078. Two families + one sealed-blob ref, mirroring SPITBOL:
     *   IR_MATCH_*   = MATCHERS: the inline needle. One node per pattern element, wired by γ (success)
     *                  / ω (failure) ports; IR_MATCH_ALTERNATE builds the backtrack tree, IR_MATCH_SEQUENCE
     *                  threads concatenation, IR_MATCH_ASSIGN_IMM/_COND do `$`/`.`. Used when a
     *                  pattern is matched directly (`SUBJECT PAT [= REPL]`) — lower_pat_node emits these.
     *   IR_PATTERN_* = STITCH boxes ONLY (D7 pivot d7ba0fd9 → RT build/stitch 52fce031): the per-element
     *                  builders were ABANDONED — FZ-3/FZ-4 constant folding freezes every invariant
     *                  subpattern to IR_REF_INVARIANT, so only VARIANT parts get stitched (CAT/ALT) around
     *                  sealed blobs; + CAPTURE passthrough, DEFER, and IR_DTP_ASSIGN for stored `.`/`$`.
     *   IR_REF_INVARIANT = a compile-time constant-folded pattern sealed into a RO blob (FZ-3,
     *                  18133720): an all-constant (VARIANT-free) subpattern is built ONCE, not per-match.
     * Re-seating is incremental (see GOAL-IR-IMMUTABLE-EMIT.md SN4-PAT ladder): enum first (here),
     * then templates back into the Makefile + emitter dispatch, then lower_snobol4.c, matcher by matcher.
     * Inert until lower_snobol4.c emits them, so this addition keeps Icon + the current build green. */
    IR_MATCH,               /* base/abstract matcher tag */
    IR_MATCH_LIT,
    IR_MATCH_ANY,
    IR_MATCH_NOTANY,
    IR_MATCH_SPAN,
    IR_MATCH_SPAN_VAR,
    IR_MATCH_BREAK,
    IR_MATCH_BREAKX,
    IR_MATCH_LEN,
    IR_MATCH_POS,
    IR_MATCH_TAB,
    IR_MATCH_RTAB,
    IR_MATCH_ARB,
    IR_MATCH_ARBNO,
    IR_MATCH_REM,
    IR_MATCH_BAL,
    IR_MATCH_FENCE,
    IR_MATCH_ABORT,
    IR_MATCH_SEQUENCE,
    IR_MATCH_ALTERNATE,
    IR_MATCH_ASSIGN_IMM,
    IR_MATCH_ASSIGN_COND,
    IR_MATCH_ASSIGN_SAVE,   /* SN4-PAT-3h: phase-0 cursor SAVE for multi-element capture (its own δ-slot) */
    IR_MATCH_ATP,
    IR_MATCH_CALLOUT,
    IR_MATCH_DEFER,
    IR_MATCH_HEAD,
    IR_MATCH_RETRY,
    IR_MATCH_ADVANCE,
    IR_REF_INVARIANT,       /* sealed constant-folded pattern blob (FZ-3) */
    IR_PATTERN_CAT,      /* STITCH-CAT box (B6 409f62a9/a59f38b8): rt_pattern_stitch_cat */
    IR_PATTERN_ALT,      /* STITCH-ALT box (B3 7a12aedd): rt_pattern_stitch_alt */
    IR_PATTERN_CAPTURE,  /* passthrough since FZ-4 6141434 (Raku passthrough kept) */
    IR_PATTERN_DEFER,    /* *EXPR deferred build */
    IR_DTP_ASSIGN,       /* stored-pattern `.`/`$` capture (B3, DTP frag — see src/include/dtp.h) */
    /* ---- end SNOBOL4 pattern family -------------------------------------------------------- */
    IR_OP_COUNT
} IR_e;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_scan_kind(IR_e t) { (void) t; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_call_kind(IR_e t) {
    return t == IR_CALL_PROC_STAGED || t == IR_CALL_BUILTIN || t == IR_CALL_VALUE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_e ir_norm_call_kind(IR_e t) { return ir_is_call_kind(t) ? IR_CALL : t; }
typedef enum { BINOP_CAT_ARITH = 0, BINOP_CAT_RELOP = 1, BINOP_CAT_CONCAT = 2 } binop_cat_t;
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
#define IR_LIT(nd)  (*(nd))
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
    struct IR_vslot_t { const char * name; int off; } * vslots;
    int            n_vslots;
    int            resume_slot;
    IR_t         * body_root;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
};
IR_graph_t * IR_alloc(int max_nodes);
int ir_varslot_of(const IR_graph_t * g, const char * name);
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
void ir_tmp_slot_assign(IR_graph_t * g);
void ir_jcon_slot_assign(IR_graph_t * g);
void         bb_reset(IR_graph_t * cfg);
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
void         bb_print_v(const IR_graph_t * cfg, FILE * fp, int verbose);
const char * bb_op_name(IR_e k);
#endif
