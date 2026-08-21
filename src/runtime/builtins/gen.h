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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int binop_is_concat(long op) { return op == BINOP_CONCAT || op == BINOP_CONCAT_FRACDIGIT; }
#ifndef EXPR_T_DEFINED
#define EXPR_T_DEFINED
typedef struct tree_t tree_t;
#endif
#endif
