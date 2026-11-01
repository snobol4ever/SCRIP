#ifndef SCRIP_IR_NODE_H
#define SCRIP_IR_NODE_H
#include <stdint.h>
#include "ir/descr.h"
#include "ir/ir_fwd.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    IR_ACTIVATE = 1, IR_ASSIGN, IR_ASSIGN_VAR, IR_BINOP, IR_BINOP_RELOP_VAL, IR_BINOP_TEST, IR_BOUND, IR_CALL, IR_CALL_BUILTIN, IR_CALL_BUILTIN_GEN, IR_CALL_ICON, IR_CALL_SNOBOL4, IR_CALL_PROC_STAGED,
        IR_CALL_VALUE, IR_CMP_TEST, IR_COERCE_INTEGER, IR_COERCE_NUMERIC, IR_COERCE_REAL, IR_COERCE_STRING, IR_COFAIL, IR_CONJUNCTION, IR_CORET, IR_CREATE, IR_CUT, IR_DEFINE_BIND, IR_DEFINE_ADOPT,
        IR_DEFINE_SHIM, IR_DEFINE_RT_SHIM, IR_DEFINE_RETURN, IR_DEFINE_FRETURN, IR_DEFINE_CALL, IR_DEREF, IR_DIFFER, IR_DISJUNCTION, IR_DTP_ASSIGN, IR_EXCISED, IR_FAIL, IR_FIELD_GET, IR_FIELD_VAR,
        IR_GOTO, IR_GOTO_DEFERRED, IR_IDENT, IR_GATE, IR_INITIAL, IR_ITERATE, IR_KW_ASSIGN, IR_KW_ASSIGN_SNOBOL4, IR_KW_ICON, IR_KW_ICON_GEN, IR_KW_SNOBOL4, IR_LIMIT, IR_LIT_CHARSET, IR_LIT_INTEGER,
        IR_LIT_NAME, IR_LIT_REAL, IR_LIT_STRING, IR_MAKE_LIST, IR_MATCH, IR_MATCH_ABORT, IR_MATCH_ALTERNATE, IR_MATCH_ANY, IR_MATCH_ARB, IR_MATCH_ARBNO, IR_MATCH_ASSIGN_COND, IR_MATCH_ASSIGN_IMM,
        IR_MATCH_ASSIGN_SAVE, IR_MATCH_ATP, IR_MATCH_BAL, IR_MATCH_CASMARK, IR_MATCH_BEGIN, IR_MATCH_BREAK, IR_MATCH_BREAKX, IR_MATCH_CALLOUT, IR_MATCH_DEFER, IR_MATCH_END, IR_MATCH_FENCE0,
        IR_MATCH_FENCE1, IR_MATCH_LAMBDA, IR_MATCH_LEN, IR_MATCH_LIT, IR_MATCH_NOTANY, IR_MATCH_POS, IR_MATCH_REM, IR_MATCH_REPLACE, IR_MATCH_RETRY, IR_MATCH_RKRUN, IR_MATCH_RKCAPS, IR_MATCH_RKCAPE,
        IR_MATCH_RKLAND, IR_MATCH_RPOS, IR_MATCH_RTAB, IR_MATCH_SPAN, IR_MATCH_SPAN_VAR, IR_MATCH_TAB, IR_MATCH_VALUE, IR_GATE_ARM, IR_NULLTEST_VAR, IR_PATTERN_ALT, IR_PATTERN_CAPTURE, IR_PATTERN_CAT,
        IR_PATTERN_DEFER, IR_PROC_GEN, IR_PROC_VALUE, IR_RANDOM, IR_REF_INVARIANT, IR_REPALT, IR_RETURN, IR_REV_ASSIGN, IR_REV_ASSIGN_VAR, IR_REV_SWAP, IR_SCAN, IR_SCAN_ALTERNATE, IR_SCAN_ANY,
        IR_SCAN_BAL, IR_SCAN_ENTER, IR_SCAN_FIND, IR_SCAN_MANY, IR_SCAN_MATCH, IR_SCAN_MOVE, IR_SCAN_POS, IR_SCAN_TAB, IR_SCAN_UPTO, IR_STATEMENT, IR_STATEMENT_BEGIN, IR_STATEMENT_END, IR_STMT_MARK,
        IR_LINE_MARK, IR_SUBSCRIPT, IR_SUCCEED, IR_SUSPEND, IR_SWAP, IR_SWAP_VAR, IR_TO, IR_TO_BY, IR_UNMARK, IR_UNOP, IR_UNOP_TEST, IR_VAR, IR_VAR_REF, IR_VAR_FRAME, IR_ASSIGN_FRAME, IR_LIMIT_GATE,
        IR_LIT_ATOM, IR_UNIFY_CONST, IR_UNIFY_STRUCT, IR_UNIFY_FIRST, IR_UNIFY_VALUE, IR_SETEXIT_TEST, IR_CLEANUP, IR_CALL_DIRECT, IR_CALL_PL_LEAF, IR_CALL_PAS_BI, IR_CALL_CALLEE_REC,
        IR_CALL_FIELD_REC, IR_CALL_BYNAME, IR_CALL_TRY, IR_CALL_METHOD, IR_CALL_REDISP, IR_CALL_APPLY, IR_CALL_EVAL, IR_CALL_GEN, IR_LIT_DSTAR, IR_LIT_THUNK, IR_TABLE_HDR, IR_CALL_PL_BLOCK,
        IR_OP_COUNT
} IR_e;
#define TABLE_HDR_CELLS 15
typedef enum { CLEANUP_OPEN = 0, CLEANUP_TEST = 1, CLEANUP_POP = 2, CLEANUP_RESUME = 3, CLEANUP_PROBE = 4, CLEANUP_DRAIN = 5, CLEANUP_BANK = 6, CLEANUP_WAKE = 7, CLEANUP_DET = 8 } cleanup_kind_e;
typedef enum { SNO_FENCE_LIT_BARE = 0, SNO_FENCE_LIT_ARG = 1, SNO_FENCE_LIT_ARG_IN_ARBNO = 2, SNO_FENCE_LIT_FLUSH = 3 } sno_fence_lit_e;
DESCR_SASSERT(IR_OP_COUNT > 0,
    "THE OP-IN-A-BYTE GUARD THAT STOOD HERE IS RETIRED AND THIS ASSERT KEEPS ITS PLACE SO THE HISTORY IS "
    "NOT LOST. It read IR_OP_COUNT <= 255 because descr.h carried a uint8_t mint_op holding the minting "
    "BB/IR op, so an op past 255 would have stamped as a DIFFERENT op with nothing saying so. Lon removed "
    "mint_op on 2026-09-18, in-chat, on the ground that the op is DERIVABLE from the node id -- scrip "
    "--dump-bb maps each id to its kind and statement -- so storing both was storing one fact twice. The "
    "byte went to src_node, which is now 24 bits. NOTHING CONSTRAINS IR_OP_COUNT TO A BYTE ANY MORE; if "
    "you are adding ops, add them. The descr_tags.inc block above this file's enum carries the rest of "
    "the numbering history, including the 123 per-runtime-routine ids that were a SECOND, ORTHOGONAL " "dimension and are still homeless.");
typedef enum { BINOP_CAT_ARITH = 0, BINOP_CAT_RELOP = 1, BINOP_CAT_CONCAT = 2, BINOP_CAT_XREP = 3 } binop_cat_t;
static inline int ir_is_scan_kind(IR_e t) { return 0; }
static inline int ir_is_call_road_kind(IR_e t) { return (t >= IR_CALL_DIRECT && t <= IR_CALL_GEN) || t == IR_CALL_PL_BLOCK; }
int ir_is_call_kind(IR_e t);
static inline IR_e ir_norm_call_kind(IR_e t) { return ir_is_call_kind(t) ? IR_CALL : t; }
static inline int ir_is_define_kind(IR_e t) { return t >= IR_DEFINE_BIND && t <= IR_DEFINE_CALL; }
int ir_is_matcher(int t);
int ir_is_matcher_element(int t);
int ir_decides_own_failure_kind(IR_e t);
const char * bb_op_name(IR_e k);
typedef struct { IR_t * node; char sz[4]; } IR_ref_t;
struct IR_t {
    IR_e op;
    IR_e op0;
    IR_ref_t γ;
    IR_ref_t ω;
    IR_t ** operands;
    int n_operands;
    int seal;
    int pat_static;
    int strict;
    int elem_access;
    union { const char * sval; int64_t ival; double dval; };
};
static inline IR_e ir_kind0(const IR_t * n) { return n->op0 ? n->op0 : n->op; }
static inline int ir_is_lit_text_kind(IR_e op) { return op == IR_LIT_STRING || op == IR_LIT_DSTAR || op == IR_LIT_THUNK; }
#define IR_LIT(nd) (*(nd))
#define IR_SEAL_CALL_DET_LEAF 2
#define IR_SEAL_SUBSCRIPT_VALUE 5
#define IR_STRICT_DIVERR_MASK (3 << 27)
#define IR_STRICT_DIVERR_ISO7185 (1 << 27)
#define IR_STRICT_DIVERR_RTERR (2 << 27)
#define IR_STRICT_RELOP_NUMIFY_BLANK (1 << 29)
#define IR_STRICT_RELOP_NUM_TEXT (1 << 26)
#define IR_SEAL_STMT_HOOK 8
#define IR_SEAL_LABEL_HOOK 9
#define IR_SEAL_TAIL_POP 10
#define IR_SEAL_KILL_VALUE 11
#define IR_SEAL_KILL_HOME 12
int ir_range_tag_has(const IR_t * nd, char a, char b, char c);
int ir_range_kind_is(const IR_t * nd, const char * k);
static inline int ir_range_trailed(const IR_t * nd) { return (nd && nd->op == IR_TO) ? ir_range_tag_has(nd, 't', 'r', 'l') : 0; }
static inline int ir_range_operands_must_be_integers(const IR_t * nd) { return (nd && (nd->op == IR_TO || nd->op == IR_TO_BY)) ? ir_range_tag_has(nd, 'i', 'n', 't') : 0; }
int ir_define_ch8_role(const IR_t * nd);
static inline int ir_define_sr_citizen(const IR_t * nd) { return ir_is_define_kind(nd->op) && nd->op != IR_DEFINE_BIND; }
int ir_floater_kind(const IR_t * n);
int ir_floater_member(const IR_t * n);
static inline int port_sz_beta(const char * sz) { return sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2; }
static inline int beta_is_stmt_land(const IR_t * tgt) { return tgt && tgt->op == IR_STATEMENT_BEGIN; }
int ir_glue_box(const IR_t * t, int phi);
const char * ir_define_bind_entry(const IR_t * nd);
const char * ir_define_bind_proto(const IR_t * nd);
int ir_define_is_bind(const IR_t * nd);
IR_t * ir_call_arg(const IR_t * nd, int j);
IR_t * ir_pair_arg(const IR_t * nd, int j);
IR_t * IR_node_alloc(IR_graph_t * cfg, IR_e t);
int ir_operand_push(IR_t * nd, IR_t * child);
int ir_node_produces_value(IR_e op);
#ifdef __cplusplus
}
#endif
#endif
