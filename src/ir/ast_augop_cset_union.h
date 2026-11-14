#ifndef AST_AUGOP_CSET_UNION_H
#define AST_AUGOP_CSET_UNION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    AUGOP_ADD = 1, AUGOP_SUB = 2, AUGOP_MUL = 3, AUGOP_DIV = 4, AUGOP_MOD = 5, AUGOP_POW = 6, AUGOP_CONCAT = 7, AUGOP_CSET_UNION = 8, AUGOP_CSET_DIFF = 9, AUGOP_CSET_INTER = 10, AUGOP_SCAN = 11,
        AUGOP_EQ = 12, AUGOP_SEQ = 13, AUGOP_LT = 14, AUGOP_LE = 15, AUGOP_GT = 16, AUGOP_GE = 17, AUGOP_NE = 18, AUGOP_SLT = 19, AUGOP_SLE = 20, AUGOP_SGT = 21, AUGOP_SGE = 22, AUGOP_SNE = 23,
        AUGOP_LCONCAT = 24, AUGOP_ACTIVATE = 25, AUGOP_IDENTICAL = 26, AUGOP_NIDENTICAL = 27, AUGOP_CONJ = 28,
} AugOp_e;
#ifdef __cplusplus
}
#endif
#endif
