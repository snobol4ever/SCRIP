#include "IR.h"
#include "box_state.h"
#include "bb_program.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static const char * kind_names[IR_OP_COUNT] = {
    [IR_LIT_I] = "IR_LIT_I",
    [IR_LIT_S] = "IR_LIT_S",
    [IR_LIT_F] = "IR_LIT_F",
    [IR_LIT_NUL] = "IR_LIT_NUL",
    [IR_VAR] = "IR_VAR",
    [IR_ASSIGN] = "IR_ASSIGN",
    [IR_PROG] = "IR_PROG",
    [IR_AUGOP] = "IR_AUGOP",
    [IR_BINOP] = "IR_BINOP",
    [IR_BINOP_RELOP] = "IR_BINOP_RELOP",
    [IR_BINOP_ARITH] = "IR_BINOP_ARITH",
    [IR_BINOP_GVAR_ARITH] = "IR_BINOP_GVAR_ARITH",
    [IR_UNOP_GVAR_SLOT] = "IR_UNOP_GVAR_SLOT",
    [IR_BINOP_CONCAT] = "IR_BINOP_CONCAT",
    [IR_UNOP] = "IR_UNOP",
    [IR_CALL] = "IR_CALL",
    [IR_CALL_DEFINE] = "IR_CALL_DEFINE",
    [IR_CALL_PROC_STAGED] = "IR_CALL_PROC_STAGED",
    [IR_CALL_USERPROC] = "IR_CALL_USERPROC",
    [IR_CALL_BYNAME] = "IR_CALL_BYNAME",
    [IR_CALL_BUILTIN] = "IR_CALL_BUILTIN",
    [IR_CALL_GVAR_USERPROC] = "IR_CALL_GVAR_USERPROC",
    [IR_SEQ] = "IR_SEQ",
    [IR_FAIL] = "IR_FAIL",
    [IR_SUCCEED] = "IR_SUCCEED",
    [IR_GOTO] = "IR_GOTO",
    [IR_RETURN] = "IR_RETURN",
    [IR_IF] = "IR_IF",
    [IR_CONJ] = "IR_CONJ",
    [IR_TO_BY] = "IR_TO_BY",
    [IR_EVERY] = "IR_EVERY",
    [IR_WHILE] = "IR_WHILE",
    [IR_UNTIL] = "IR_UNTIL",
    [IR_REPEAT] = "IR_REPEAT",
    [IR_ALT] = "IR_ALT",
    [IR_SIZE] = "IR_SIZE",
    [IR_CASE] = "IR_CASE",
    [IR_CASE_ARM] = "IR_CASE_ARM",
    [IR_LIMIT] = "IR_LIMIT",
    [IR_SUSPEND] = "IR_SUSPEND",
    [IR_PROC] = "IR_PROC",
    [IR_SCAN] = "IR_SCAN",
    [IR_NONNULL] = "IR_NONNULL",
    [IR_INTERROGATE] = "IR_INTERROGATE",
    [IR_NOT] = "IR_NOT",
    [IR_MATCH_LIT] = "IR_MATCH_LIT",
    [IR_MATCH_ANY] = "IR_MATCH_ANY",
    [IR_MATCH_SPAN] = "IR_MATCH_SPAN",
    [IR_MATCH_BREAK] = "IR_MATCH_BREAK",
    [IR_MATCH_BREAKX]      = "IR_MATCH_BREAKX",
    [IR_MATCH_ARB] = "IR_MATCH_ARB",
    [IR_MATCH_ARBNO] = "IR_MATCH_ARBNO",
    [IR_MATCH_CAT] = "IR_MATCH_CAT",
    [IR_MATCH_ALT] = "IR_MATCH_ALT",
    [IR_MATCH_ASSIGN_IMM] = "IR_MATCH_ASSIGN_IMM",
    [IR_MATCH_ASSIGN_COND] = "IR_MATCH_ASSIGN_COND",
    [IR_MATCH_LEN] = "IR_MATCH_LEN",
    [IR_MATCH_NOTANY] = "IR_MATCH_NOTANY",
    [IR_MATCH_POS] = "IR_MATCH_POS",
    [IR_MATCH_TAB] = "IR_MATCH_TAB",
    [IR_MATCH_RTAB]        = "IR_MATCH_RTAB",
    [IR_MATCH_SPAN_VAR]    = "IR_MATCH_SPAN_VAR",
    [IR_ASSIGN_LIT_S]    = "IR_ASSIGN_LIT_S",
    [IR_ASSIGN_LIT_I]    = "IR_ASSIGN_LIT_I",
    [IR_ASSIGN_VAR]      = "IR_ASSIGN_VAR",
    [IR_ASSIGN_CONCAT]   = "IR_ASSIGN_CONCAT",
    [IR_ASSIGN_CALL]     = "IR_ASSIGN_CALL",
    [IR_ASSIGN_DESCR]    = "IR_ASSIGN_DESCR",
    [IR_MATCH_REM] = "IR_MATCH_REM",
    [IR_MATCH_FENCE] = "IR_MATCH_FENCE",
    [IR_MATCH_ABORT] = "IR_MATCH_ABORT",
    [IR_MATCH_CALLOUT] = "IR_MATCH_CALLOUT",
    [IR_MATCH_DEFER] = "IR_MATCH_DEFER",
    [IR_CHOICE] = "IR_CHOICE",
    [IR_UNIFY] = "IR_UNIFY",
    [IR_CUT] = "IR_CUT",
    [IR_GOAL] = "IR_GOAL",
    [IR_BUILTIN] = "IR_BUILTIN",
    [IR_LOGICVAR] = "IR_LOGICVAR",
    [IR_ATOM] = "IR_ATOM",
    [IR_STRUCT] = "IR_STRUCT",
    [IR_ARITH] = "IR_ARITH",
    [IR_DISJ] = "IR_DISJ",
    [IR_GCONJ] = "IR_GCONJ",
    [IR_ITE] = "IR_ITE",
    [IR_CATCH] = "IR_CATCH",
    [IR_QUERY_FRAME] = "IR_QUERY_FRAME",
    [IR_DET_WRITE] = "IR_DET_WRITE",
    [IR_DET_NL] = "IR_DET_NL",
    [IR_DET_IS] = "IR_DET_IS",
    [IR_DET_CMP] = "IR_DET_CMP",
    [IR_DET_TYPE_TEST] = "IR_DET_TYPE_TEST",
    [IR_DET_FUNCTOR] = "IR_DET_FUNCTOR",
    [IR_DET_ARG] = "IR_DET_ARG",
    [IR_DET_UNIV] = "IR_DET_UNIV",
    [IR_DET_FORMAT] = "IR_DET_FORMAT",
    [IR_DET_SUCC_PLUS] = "IR_DET_SUCC_PLUS",
    [IR_DET_ATOM_OP]   = "IR_DET_ATOM_OP",
    [IR_DET_CHAR_TYPE] = "IR_DET_CHAR_TYPE",
    [IR_DET_SORT] = "IR_DET_SORT",
    [IR_DET_NUMBERVARS] = "IR_DET_NUMBERVARS",
    [IR_DET_TERM_STRING] = "IR_DET_TERM_STRING",
    [IR_DET_COPY_TERM] = "IR_DET_COPY_TERM",
    [IR_DET_NB_SETVAL] = "IR_DET_NB_SETVAL",
    [IR_DET_NB_GETVAL] = "IR_DET_NB_GETVAL",
    [IR_CELL_CHOICE] = "IR_CELL_CHOICE",
    [IR_CELL_CALL] = "IR_CELL_CALL",
    [IR_CALLEE_FRAME] = "IR_CALLEE_FRAME",
    [IR_CELL_CUT] = "IR_CELL_CUT",
    [IR_ITE_COMMIT] = "IR_ITE_COMMIT",
    [IR_ITE_GATE] = "IR_ITE_GATE",
    [IR_CELL_ITE] = "IR_CELL_ITE",
    [IR_CELL_FINDALL] = "IR_CELL_FINDALL",
    [IR_DET_THROW] = "IR_DET_THROW",
    [IR_CELL_CATCH] = "IR_CELL_CATCH",
    [IR_DET_RETRACT] = "IR_DET_RETRACT",
    [IR_DET_ABOLISH] = "IR_DET_ABOLISH",
    [IR_GOTO_DYN] = "IR_GOTO_DYN",
    [IR_INDIRECT_ASSIGN_LIT_S] = "IR_INDIRECT_ASSIGN_LIT_S",
    [IR_INDIRECT_ASSIGN_VAR] = "IR_INDIRECT_ASSIGN_VAR",
    [IR_INDIRECT_ASSIGN_DESCR] = "IR_INDIRECT_ASSIGN_DESCR",
    [IR_TMP] = "IR_TMP",
    [IR_DET_ASSERTZ] = "IR_DET_ASSERTZ",
    [IR_CELL_DYNITER] = "IR_CELL_DYNITER",
    [IR_TO] = "IR_TO",
    [IR_UPTO] = "IR_UPTO",
    [IR_ITERATE] = "IR_ITERATE",
    [IR_GEN_ALT] = "IR_GEN_ALT",
    [IR_TO_NESTED] = "IR_TO_NESTED",
    [IR_PROC_GEN] = "IR_PROC_GEN",
    [IR_BREAK] = "IR_BREAK",
    [IR_NEXT] = "IR_NEXT",
    [IR_IDENTICAL] = "IR_IDENTICAL",
    [IR_NULL_TEST] = "IR_NULL_TEST",
    [IR_RANDOM] = "IR_RANDOM",
    [IR_NEG] = "IR_NEG",
    [IR_POS] = "IR_POS",
    [IR_CSET_COMPL] = "IR_CSET_COMPL",
    [IR_CSET_UNION] = "IR_CSET_UNION",
    [IR_CSET_DIFF] = "IR_CSET_DIFF",
    [IR_CSET_INTER] = "IR_CSET_INTER",
    [IR_GEN_SCAN] = "IR_GEN_SCAN",
    [IR_KEYWORD] = "IR_KEYWORD",
    [IR_IDX] = "IR_IDX",
    [IR_SECTION] = "IR_SECTION",
    [IR_LIST_BANG] = "IR_LIST_BANG",
    [IR_RECORD_DEF] = "IR_RECORD_DEF",
    [IR_FIELD_GET] = "IR_FIELD_GET",
    [IR_FIELD_SET] = "IR_FIELD_SET",
    [IR_IDX_SET] = "IR_IDX_SET",
    [IR_KEY_GEN] = "IR_KEY_GEN",
    [IR_SWAP] = "IR_SWAP",
    [IR_RASGN] = "IR_RASGN",
    [IR_SEQ_EXPR] = "IR_SEQ_EXPR",
    [IR_INITIAL] = "IR_INITIAL",
    [IR_LCONCAT] = "IR_LCONCAT",
    [IR_FIND_GEN] = "IR_FIND_GEN",
    [IR_SEQ_GEN] = "IR_SEQ_GEN",
    [IR_MATCH_ATP] = "IR_MATCH_ATP",
    [IR_GATHER] = "IR_GATHER",
    [IR_MAP] = "IR_MAP",
    [IR_GREP] = "IR_GREP",
    [IR_SCAN_ANY] = "IR_SCAN_ANY",
    [IR_SCAN_MANY] = "IR_SCAN_MANY",
    [IR_SCAN_MATCH] = "IR_SCAN_MATCH",
    [IR_SCAN_UPTO] = "IR_SCAN_UPTO",
    [IR_SCAN_FIND] = "IR_SCAN_FIND",
    [IR_SCAN_BAL] = "IR_SCAN_BAL",
    [IR_SCAN_TAB] = "IR_SCAN_TAB",
    [IR_SCAN_MOVE] = "IR_SCAN_MOVE",
    [IR_SCAN_POS] = "IR_SCAN_POS",
    [IR_VAR_FRAME] = "IR_VAR_FRAME",
    [IR_ASSIGN_FRAME] = "IR_ASSIGN_FRAME",
    [IR_VAR_FRAME_REF] = "IR_VAR_FRAME_REF",
    [IR_ASSIGN_FRAME_REF] = "IR_ASSIGN_FRAME_REF",
    [IR_MATCH] = "IR_MATCH",
    [IR_MATCH_HEAD] = "IR_MATCH_HEAD",
    [IR_MATCH_RETRY] = "IR_MATCH_RETRY",
    [IR_MATCH_ADVANCE] = "IR_MATCH_ADVANCE",
    [IR_PATTERN_LIT] = "IR_PATTERN_LIT",
    [IR_PATTERN_ANY] = "IR_PATTERN_ANY",
    [IR_PATTERN_NOTANY] = "IR_PATTERN_NOTANY",
    [IR_PATTERN_SPAN] = "IR_PATTERN_SPAN",
    [IR_PATTERN_BREAK] = "IR_PATTERN_BREAK",
    [IR_PATTERN_BREAKX] = "IR_PATTERN_BREAKX",
    [IR_PATTERN_LEN] = "IR_PATTERN_LEN",
    [IR_PATTERN_POS] = "IR_PATTERN_POS",
    [IR_PATTERN_RPOS] = "IR_PATTERN_RPOS",
    [IR_PATTERN_TAB] = "IR_PATTERN_TAB",
    [IR_PATTERN_RTAB] = "IR_PATTERN_RTAB",
    [IR_PATTERN_ARB] = "IR_PATTERN_ARB",
    [IR_PATTERN_REM] = "IR_PATTERN_REM",
    [IR_PATTERN_BAL] = "IR_PATTERN_BAL",
    [IR_PATTERN_ABORT] = "IR_PATTERN_ABORT",
    [IR_PATTERN_FENCE] = "IR_PATTERN_FENCE",
    [IR_PATTERN_FAIL] = "IR_PATTERN_FAIL",
    [IR_PATTERN_SUCCEED] = "IR_PATTERN_SUCCEED",
    [IR_PATTERN_ARBNO] = "IR_PATTERN_ARBNO",
    [IR_PATTERN_FENCE_P] = "IR_PATTERN_FENCE_P",
    [IR_PATTERN_CAT] = "IR_PATTERN_CAT",
    [IR_PATTERN_ALT] = "IR_PATTERN_ALT",
    [IR_PATTERN_CAPTURE] = "IR_PATTERN_CAPTURE",
    [IR_PATTERN_DEFER] = "IR_PATTERN_DEFER",
    [IR_DTP_ASSIGN] = "IR_DTP_ASSIGN",
};
/*--------------------------------------------------------------------------------------------------------------------*/
const char * bb_op_name(IR_e k) {
    if (k >= 0 && k < IR_OP_COUNT) return kind_names[k];
    return "IR_UNKNOWN";
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes, int lang) {
    IR_graph_t * bbg = calloc(1, sizeof(IR_graph_t));
    if (!bbg) return NULL;
    bbg->all  = calloc((size_t)max_nodes, sizeof(IR_t *));
    if (!bbg->all) { free(bbg); return NULL; }
    bbg->lit  = calloc((size_t)max_nodes, sizeof(IR_lit_t));
    bbg->exec = calloc((size_t)max_nodes, sizeof(IR_exec_t));
    if (!bbg->lit || !bbg->exec) { free(bbg->lit); free(bbg->exec); free(bbg->all); free(bbg); return NULL; }
    bbg->n    = 0;
    bbg->max  = max_nodes;
    bbg->lang = lang;
    bbg->entry = NULL;
    return bbg;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * IR_node_alloc(IR_graph_t * bbg, IR_e t) {
    IR_t * bb = calloc(1, sizeof(IR_t));
    if (!bb) return NULL;
    bb->op       = t;
    bb->γ.node = NULL;
    bb->ω.node = NULL;
    bb->lhs      = -1;
    if (bbg->n >= bbg->max) { free(bb); return NULL; }
    bb->idx = bbg->n;
    bb->own = bbg;
    bbg->all[bbg->n++] = bb;
    bbg->exec[bb->idx].value = FAILDESCR;
    return bb;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_operand_push(IR_t * nd, IR_t * child) {
    if (!nd) return 0;
    IR_t ** p = realloc(nd->operands, (size_t)(nd->n_operands + 1) * sizeof(IR_t *));
    if (!p) return 0;
    nd->operands = p;
    nd->operands[nd->n_operands++] = child;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_operand_aux_set(IR_graph_t * bbg, IR_t * bb, IR_t * const * src, int n) {
    (void) bbg;
    if (!bb) return -1;
    if (bb->operands) { free(bb->operands); bb->operands = NULL; bb->n_operands = 0; }
    if (n <= 0) return 0;
    bb->operands = (IR_t **)calloc((size_t)n, sizeof(IR_t *));
    if (!bb->operands) return -1;
    if (src) for (int i = 0; i < n; i++) bb->operands[i] = src[i];
    bb->n_operands = n;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * const * bb_operand_aux_get(const IR_graph_t * bbg, const IR_t * bb, int * out_n) {
    (void) bbg;
    if (out_n) *out_n = 0;
    if (!bb) return NULL;
    if (out_n) *out_n = bb->n_operands;
    return bb->operands;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void IR_free(IR_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        free(bb);
    }
    free(bbg->lit);
    free(bbg->exec);
    free(bbg->all);
    free(bbg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void IR_free_dyn(void * g) { IR_free((IR_graph_t *)g); }
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_program_add(bb_program_t * p, IR_graph_t * bbg) {
    if (!bbg) return -1;
    if (p->cap == 0) {
        p->cap   = 16;
        p->count = 0;
        p->table = (IR_graph_t **)calloc((size_t)p->cap, sizeof(IR_graph_t *));
    }
    if (p->count >= p->cap) {
        p->cap  *= 2;
        p->table = (IR_graph_t **)realloc(p->table, (size_t)p->cap * sizeof(IR_graph_t *));
    }
    int idx = p->count++;
    p->table[idx] = bbg;
    return idx;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_program_free(bb_program_t * p) {
    if (!p) return;
    for (int i = 0; i < p->count; i++) { IR_free(p->table[i]); p->table[i] = NULL; }
    p->count = 0;
    p->cap   = 0;
    free(p->table);
    p->table = NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_index_of(const IR_graph_t * bbg, const IR_t * bb) {
    if (!bbg || !bb) return -1;
    for (int i = 0; i < bbg->n; i++) if (bbg->all[i] == bb) return i;
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void bb_emit_order_visit(const IR_graph_t *bbg, const IR_t *nd, char *vis, int *order, int *norder) {
    if (!nd) return;
    int ix = nd->idx;
    if (ix < 0 || ix >= bbg->n || bbg->all[ix] != nd || vis[ix]) return;
    vis[ix] = 1; order[(*norder)++] = ix;
    bb_emit_order_visit(bbg, nd->γ.node, vis, order, norder);
    bb_emit_order_visit(bbg, nd->ω.node, vis, order, norder);
    int na = 0; IR_t * const * ops = NULL;
    if (nd->n_operands > 0) { na = nd->n_operands; ops = nd->operands; } else { ops = bb_operand_aux_get((IR_graph_t *)bbg, (IR_t *)nd, &na); }
    if (ops) for (int j = 0; j < na; j++) if (ops[j]) bb_emit_order_visit(bbg, ops[j], vis, order, norder);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_node_produces_value(IR_e op) { return op == IR_LIT_I || op == IR_LIT_S || op == IR_LIT_F || op == IR_LIT_NUL || op == IR_VAR || op == IR_BINOP || op == IR_UNOP || op == IR_CALL || ir_is_call_kind(op) || op == IR_PROC_GEN; }
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_tmp_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int cursor = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && ir_node_produces_value(nd->op)) { nd->lhs = cursor; cursor += 16; } }
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (!nd || nd->op != IR_SEQ || IR_LIT(nd).dval != 1.0) continue; IR_graph_t * L = (IR_graph_t *)(intptr_t) IR_EXEC(nd).counter; IR_graph_t * R = (IR_graph_t *)(intptr_t) IR_LIT(nd).ival; if (L) ir_tmp_slot_assign(L); if (R) ir_tmp_slot_assign(R); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_tmp_slot_assign_flat(IR_graph_t * g) {
    if (!g) return;
    int n = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op != IR_VAR && ir_node_produces_value(nd->op)) { nd->lhs = n * 16; n++; } }
    g->nvalue_slots = n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int jcon_converted_producer(IR_e op) { return op == IR_LIT_I || op == IR_LIT_S || op == IR_LIT_F || op == IR_LIT_NUL; }
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_jcon_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int k = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && jcon_converted_producer(nd->op)) { nd->lhs = 16 + k * 16; k++; } }
    g->jcon_value_region = k * 16;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void bb_print_node_line(const IR_graph_t *bbg, FILE *fp, int seq, int i) {
    const IR_t * bb = (i >= 0 && i < bbg->n) ? bbg->all[i] : NULL;
    char sq[8]; if (seq >= 0) snprintf(sq, sizeof sq, "%d", seq); else snprintf(sq, sizeof sq, "-");
    if (!bb) { fprintf(fp, "%4s %4d:    .    .  %-22s []\n", sq, i, "(null)"); return; }
    char gp[8], wp[8];
    if (bb->γ.node) snprintf(gp, sizeof gp, "%d", bb->γ.node->idx); else snprintf(gp, sizeof gp, ".");
    if (bb->ω.node) snprintf(wp, sizeof wp, "%d", bb->ω.node->idx); else snprintf(wp, sizeof wp, ".");
    int na = 0; IR_t * const * ops = NULL;
    if (bb->n_operands > 0) { na = bb->n_operands; ops = bb->operands; } else { ops = bb_operand_aux_get((IR_graph_t *)bbg, (IR_t *)bb, &na); }
    char ob[160]; int op = 0; ob[0] = 0;
    if (bb->op != IR_SCAN && na > 0 && ops) for (int j = 0; j < na && op < 140; j++) op += snprintf(ob + op, sizeof ob - op, "%s%d", j ? " " : "", ops[j] ? ops[j]->idx : -1);
    const char * opn = bb_op_name(bb->op);
    fprintf(fp, "%4s %4d: %4s %4s  %-22s [%s]", sq, i, gp, wp, opn, ob);
    if (bb->lhs >= 0) fprintf(fp, " lhs=%d", bb->lhs);
    if (IR_EXEC(bb).stno != 0) fprintf(fp, " stno=%d", (int)IR_EXEC(bb).stno);
    switch (bb->op) {
        case IR_LIT_I: fprintf(fp, " ival=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_LIT_F: fprintf(fp, " dval=%g", IR_LIT(bb).dval); break;
        case IR_LIT_S: fprintf(fp, " sval=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_VAR: fprintf(fp, " var=\"%s\"%s", IR_LIT(bb).sval ? IR_LIT(bb).sval : "", IR_EXEC(bb).state == 1 ? " global" : ""); break;
        case IR_LOGICVAR: fprintf(fp, " slot=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_ATOM: fprintf(fp, " atom=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_BINOP: fprintf(fp, " binop=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_ARITH: fprintf(fp, " arith=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_GOAL: fprintf(fp, " callee=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_BUILTIN: fprintf(fp, " builtin=\"%s\"/%lld", IR_LIT(bb).sval ? IR_LIT(bb).sval : "", (long long)IR_LIT(bb).ival); break;
        case IR_SEQ: fprintf(fp, " parts(sub-graphs)"); break;
        case IR_FIELD_GET:
        case IR_FIELD_SET: fprintf(fp, " field=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        default:
            if (IR_LIT(bb).sval) fprintf(fp, " sval=\"%s\"", IR_LIT(bb).sval);
            else if (IR_LIT(bb).ival) fprintf(fp, " ival=%lld", (long long)IR_LIT(bb).ival);
            break;
    }
    fprintf(fp, "\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_print(const IR_graph_t * bbg, FILE * fp) {
    if (!bbg) { fprintf(fp, "(null IR_graph_t)\n"); return; }
    static const char * lang_names[] = { "?", "SNO", "SCO", "REB", "ICN", "PL", "RKU" };
    const char * lname = (bbg->lang >= 1 && bbg->lang <= 6) ? lang_names[bbg->lang] : "?";
    fprintf(fp, "IR_graph_t lang=%s n=%d entry=%d nslots=%d\n", lname, bbg->n, bb_index_of(bbg, bbg->entry), bbg->nslots);
    fprintf(fp, ";  seq node:    γ    ω  kind                   [operands]  payload   (linear emit order: γ-spine DFS from entry, then ω, then operands)\n");
    int nn = bbg->n;
    char * vis = (char *) calloc(nn > 0 ? nn : 1, 1);
    int * order = (int *) malloc((size_t)(nn > 0 ? nn : 1) * sizeof(int));
    int norder = 0;
    if (vis && order) {
        bb_emit_order_visit(bbg, bbg->entry, vis, order, &norder);
        for (int sq = 0; sq < norder; sq++) bb_print_node_line(bbg, fp, sq, order[sq]);
        int any_unreached = 0;
        for (int i = 0; i < nn; i++) if (!vis[i] && bbg->all[i]) { any_unreached = 1; break; }
        if (any_unreached) fprintf(fp, "; --- unreached (not on emit spine; shown for completeness) ---\n");
        if (any_unreached) for (int i = 0; i < nn; i++) if (!vis[i] && bbg->all[i]) bb_print_node_line(bbg, fp, -1, i);
    } else { for (int i = 0; i < nn; i++) bb_print_node_line(bbg, fp, i, i); }
    free(vis); free(order);
    for (int i = 0; i < nn; i++) {                              /* recurse only true sub-graph nodes (leaf-SEQ/scan) */
        const IR_t * bb = bbg->all[i];
        if (!bb || bb->op != IR_SEQ || IR_LIT(bb).dval != 1.0) continue;
        IR_graph_t * L = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
        IR_graph_t * R = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
        if (L) { fprintf(fp, "; SEQ[%d] part0:\n", i); bb_print(L, fp); }
        if (R) { fprintf(fp, "; SEQ[%d] part1:\n", i); bb_print(R, fp); }
    }
    for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        if (!bb || bb->op != IR_SCAN) continue;
        IR_graph_t * pg = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
        if (pg) bb_print(pg, fp);
        for (int j = 0; j < bb->n_operands; j++) if (bb->operands[j]) bb_print((IR_graph_t *)(void *) bb->operands[j], fp);
    }
    { static int xd2 = -1; if (xd2 < 0) { const char * e = getenv("SCRIP_DUMP_X"); xd2 = (e && e[0] == '1') ? 1 : 0; }
      if (xd2) for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op == IR_GEN_SCAN && IR_LIT(bb).dval == 1.0) {
            IR_graph_t * ssg = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
            IR_graph_t * bsg = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
            if (ssg) { fprintf(fp, "; X scansubj node=%d\n", i); bb_print(ssg, fp); }
            if (bsg) { fprintf(fp, "; X scanbody node=%d\n", i); bb_print(bsg, fp); }
            continue;
        }
        if (bb->op != IR_CALL && bb->op != IR_CALL_DEFINE) continue;
        if (IR_LIT(bb).dval != 2.0 && IR_LIT(bb).dval != 3.0 && IR_LIT(bb).dval != 5.0) continue;
        IR_graph_t ** blks = (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter;
        if (!blks) continue;
        for (int j = 0; j < (int) IR_LIT(bb).ival; j++) if (blks[j]) { fprintf(fp, "; X argblk call=%d arg=%d\n", i, j); bb_print(blks[j], fp); }
      } }
}
