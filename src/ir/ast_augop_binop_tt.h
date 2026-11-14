#ifndef AST_AUGOP_BINOP_TT_H
#define AST_AUGOP_BINOP_TT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast_augop_cset_union.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline tree_e augop_binop_tt(int a) {
    switch (a) {
        case AUGOP_ADD:
        return TT_ADD;
        case AUGOP_SUB:
        return TT_SUB;
        case AUGOP_MUL:
        return TT_MUL;
        case AUGOP_DIV:
        return TT_DIV;
        case AUGOP_MOD:
        return TT_MOD;
        case AUGOP_POW:
        return TT_POW;
        case AUGOP_CONCAT:
        return TT_CAT;
        case AUGOP_LCONCAT:
        return TT_LCONCAT;
        case AUGOP_EQ:
        return TT_EQ;
        case AUGOP_LT:
        return TT_LT;
        case AUGOP_LE:
        return TT_LE;
        case AUGOP_GT:
        return TT_GT;
        case AUGOP_GE:
        return TT_GE;
        case AUGOP_NE:
        return TT_NE;
        case AUGOP_SEQ:
        return TT_LEQ;
        case AUGOP_SLT:
        return TT_LLT;
        case AUGOP_SLE:
        return TT_LLE;
        case AUGOP_SGT:
        return TT_LGT;
        case AUGOP_SGE:
        return TT_LGE;
        case AUGOP_SNE:
        return TT_LNE;
        case AUGOP_CSET_UNION:
        return TT_CSET_UNION;
        case AUGOP_CSET_DIFF:
        return TT_CSET_DIFF;
        case AUGOP_CSET_INTER:
        return TT_CSET_INTER;
        case AUGOP_ACTIVATE:
        return TT_ACTIVATE;
        case AUGOP_IDENTICAL:
        return TT_IDENTICAL;
        case AUGOP_NIDENTICAL:
        return TT_NIDENTICAL;
        case AUGOP_CONJ:
        return TT_CONJ;
        default:
        return (tree_e) 0;
    }
}
#ifdef __cplusplus
}
#endif
#endif
