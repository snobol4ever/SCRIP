/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GEN_H
#define GEN_H
#include <stdlib.h>
#include <string.h>
#include "bb_box.h"
#include "core.h"
typedef enum {
    BINOP_ADD, BINOP_SUB, BINOP_MUL, BINOP_DIV, BINOP_MOD,
    BINOP_LT, BINOP_LE, BINOP_GT, BINOP_GE, BINOP_EQ, BINOP_NE,
    BINOP_CONCAT,
    BINOP_SLT, BINOP_SLE, BINOP_SGT, BINOP_SGE, BINOP_SEQ, BINOP_SNE,
    BINOP_POW,
    BINOP_CUNION, BINOP_CDIFF, BINOP_CINTER,
    BINOP_EQV, BINOP_NEQV,
    BINOP_XREP,
    BINOP_CONCAT_FRACDIGIT,
} BinopKind;
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Two concatenation opcodes, because concatenation is TWO operations that differ in how a real operand becomes a string, and that difference is settled by the
 * source language -- which is knowable at LOWER and deliberately unknowable downstream.  BINOP_CONCAT renders reals in SPITBOL's trailing-point convention
 * ("10.", real_str) and keeps SPITBOL's type-preserving null-string identity; BINOP_CONCAT_FRACDIGIT renders them with a mandatory fraction digit ("10.0",
 * icon_real_str) and always yields a string, per canonical cater() in refs/icon-master/src/runtime/ocat.r (cnv:string on BOTH operands, abstract{return string}).
 * APPENDED AT END, never inserted: the enum carries live range tests (BINOP_LT..BINOP_NE, BINOP_SLT..BINOP_SNE) that renumbering would silently break.
 * Every downstream site that only asks "is this a concatenation?" -- category mapping, ZD arming, the write() fast path, driver graph analysis -- must ask
 * through binop_is_concat() rather than comparing to one code, so that adding a third convention later cannot silently skip a site.  The two sites that must
 * NOT use it are the ones that select the convention itself: the fold in lower_common.c and the runtime symbol chosen in bb_binop_concat_slot.cpp.
 * GATE: grep -rn '== BINOP_CONCAT\b' src/ must return 0. */
static inline int binop_is_concat(long op) { return op == BINOP_CONCAT || op == BINOP_CONCAT_FRACDIGIT; }
#ifndef EXPR_T_DEFINED
#define EXPR_T_DEFINED
typedef struct tree_t tree_t;
#endif
#endif
