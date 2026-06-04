#include "IR.h"
#include "IR_interp_state.h"
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
    [IR_BINOP_CONCAT] = "IR_BINOP_CONCAT",
    [IR_UNOP] = "IR_UNOP",
    [IR_CALL] = "IR_CALL",
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
    [IR_LIMIT] = "IR_LIMIT",
    [IR_SUSPEND] = "IR_SUSPEND",
    [IR_PROC] = "IR_PROC",
    [IR_SCAN] = "IR_SCAN",
    [IR_NONNULL] = "IR_NONNULL",
    [IR_INTERROGATE] = "IR_INTERROGATE",
    [IR_NOT] = "IR_NOT",
    [IR_PAT_LIT] = "IR_PAT_LIT",
    [IR_PAT_ANY] = "IR_PAT_ANY",
    [IR_PAT_SPAN] = "IR_PAT_SPAN",
    [IR_PAT_BREAK] = "IR_PAT_BREAK",
    [IR_PAT_ARB] = "IR_PAT_ARB",
    [IR_PAT_ARBNO] = "IR_PAT_ARBNO",
    [IR_PAT_CAT] = "IR_PAT_CAT",
    [IR_PAT_ALT] = "IR_PAT_ALT",
    [IR_PAT_ASSIGN_IMM] = "IR_PAT_ASSIGN_IMM",
    [IR_PAT_ASSIGN_COND] = "IR_PAT_ASSIGN_COND",
    [IR_PAT_LEN] = "IR_PAT_LEN",
    [IR_PAT_NOTANY] = "IR_PAT_NOTANY",
    [IR_PAT_POS] = "IR_PAT_POS",
    [IR_PAT_TAB] = "IR_PAT_TAB",
    [IR_PAT_REM] = "IR_PAT_REM",
    [IR_PAT_FENCE] = "IR_PAT_FENCE",
    [IR_PAT_ABORT] = "IR_PAT_ABORT",
    [IR_PAT_CALLOUT] = "IR_PAT_CALLOUT",
    [IR_PAT_DEFER] = "IR_PAT_DEFER",
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
    [IR_TO] = "IR_TO",
    [IR_UPTO] = "IR_UPTO",
    [IR_ITERATE] = "IR_ITERATE",
    [IR_GEN_ALT] = "IR_GEN_ALT",
    [IR_GEN_BINOP] = "IR_GEN_BINOP",
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
    [IR_BINOP_GEN] = "IR_BINOP_GEN",
    [IR_IDX] = "IR_IDX",
    [IR_SECTION] = "IR_SECTION",
    [IR_LIST_BANG] = "IR_LIST_BANG",
    [IR_RECORD_DEF] = "IR_RECORD_DEF",
    [IR_FIELD_GET] = "IR_FIELD_GET",
    [IR_FIELD_SET] = "IR_FIELD_SET",
    [IR_IDX_SET] = "IR_IDX_SET",
    [IR_KEY_GEN] = "IR_KEY_GEN",
    [IR_SWAP] = "IR_SWAP",
    [IR_SEQ_EXPR] = "IR_SEQ_EXPR",
    [IR_INITIAL] = "IR_INITIAL",
    [IR_LCONCAT] = "IR_LCONCAT",
    [IR_FIND_GEN] = "IR_FIND_GEN",
    [IR_SEQ_GEN] = "IR_SEQ_GEN",
    [IR_NFA_CHAR] = "IR_NFA_CHAR",
    [IR_NFA_ANY] = "IR_NFA_ANY",
    [IR_NFA_CLASS] = "IR_NFA_CLASS",
    [IR_NFA_SPLIT] = "IR_NFA_SPLIT",
    [IR_NFA_EPS] = "IR_NFA_EPS",
    [IR_NFA_BOL] = "IR_NFA_BOL",
    [IR_NFA_EOL] = "IR_NFA_EOL",
    [IR_NFA_CAP_OPEN] = "IR_NFA_CAP_OPEN",
    [IR_NFA_CAP_CLOSE] = "IR_NFA_CAP_CLOSE",
    [IR_NFA_ACCEPT] = "IR_NFA_ACCEPT",
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
    [IR_PAT_MATCH] = "IR_PAT_MATCH",
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
    bb->t       = t;
    bb->α       = NULL;
    bb->β       = NULL;
    bb->γ       = NULL;
    bb->ω       = NULL;
    bb->value   = FAILDESCR;
    bb->counter = 0;
    bb->state   = 0;
    if (bbg->n >= bbg->max) { free(bb); return NULL; }
    bbg->all[bbg->n++] = bb;
    return bb;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_operand_aux_set(IR_graph_t * bbg, IR_t * bb, IR_t * const * src, int n) {
    if (!bbg || !bb) return -1;
    int slot = -1;
    for (int i = 0; i < bbg->operand_aux_n; i++) {
        if (bbg->operand_aux[i].node == bb) { slot = i; break; }
    }
    if (slot < 0) {
        if (bbg->operand_aux_n >= bbg->operand_aux_max) {
            int new_max = bbg->operand_aux_max ? bbg->operand_aux_max * 2 : 16;
            bb_operand_aux_t *grown = (bb_operand_aux_t *)realloc(bbg->operand_aux, (size_t)new_max * sizeof(bb_operand_aux_t));
            if (!grown) return -1;
            bbg->operand_aux = grown;
            bbg->operand_aux_max = new_max;
        }
        slot = bbg->operand_aux_n++;
        bbg->operand_aux[slot].node = bb;
        bbg->operand_aux[slot].operands = NULL;
        bbg->operand_aux[slot].n = 0;
    }
    if (bbg->operand_aux[slot].operands) {
        free(bbg->operand_aux[slot].operands);
        bbg->operand_aux[slot].operands = NULL;
        bbg->operand_aux[slot].n = 0;
    }
    if (n <= 0) return 0;
    bbg->operand_aux[slot].operands = (IR_t **)calloc((size_t)n, sizeof(IR_t *));
    if (!bbg->operand_aux[slot].operands) return -1;
    if (src) for (int i = 0; i < n; i++) bbg->operand_aux[slot].operands[i] = src[i];
    bbg->operand_aux[slot].n = n;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * const * bb_operand_aux_get(const IR_graph_t * bbg, const IR_t * bb, int * out_n) {
    if (out_n) *out_n = 0;
    if (!bbg || !bb) return NULL;
    for (int i = 0; i < bbg->operand_aux_n; i++) {
        if (bbg->operand_aux[i].node == bb) {
            if (out_n) *out_n = bbg->operand_aux[i].n;
            return bbg->operand_aux[i].operands;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_reset(IR_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        bb->value   = FAILDESCR;
        if (bb->t != IR_PAT_ARBNO && bb->t != IR_SCAN && bb->t != IR_GEN_SCAN && bb->t != IR_GOTO && bb->t != IR_GATHER && bb->t != IR_MAP && bb->t != IR_GREP && bb->t != IR_PROG && bb->t != IR_SUSPEND && !(bb->t == IR_SEQ && bb->dval == 1.0) && !(bb->t == IR_CALL && (bb->dval == 2.0 || bb->dval == 3.0))) bb->counter = 0;
        bb->state   = 0;
    }
    ag_ring_clear(bbg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(IR_graph_t * bbg) {
    if (!bbg || bbg->n <= 0) return NULL;
    bb_node_state_t * snap = (bb_node_state_t *)malloc((size_t)bbg->n * sizeof(bb_node_state_t));
    if (!snap) return NULL;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) { memset(&snap[i], 0, sizeof snap[i]); snap[i].value = FAILDESCR; continue; }
        snap[i].value   = bb->value;
        snap[i].counter = bb->counter;
        snap[i].state   = bb->state;
        snap[i].resolve_cs = NULL; snap[i].ch_cur = 0; snap[i].ch_mark = 0; snap[i].ch_saved_env = NULL;
        snap[i].ch_last_body = NULL; snap[i].ch_last_act = NULL;
        snap[i].ch_cp = NULL; snap[i].ch_cut_barrier = NULL;
        snap[i].ch_body_snaps = NULL; snap[i].ch_nbodies = 0;
        if (bb->t == IR_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)bb->ival;
            if (zc) snap[i].resolve_cs = zc->cs;
        } else if (bb->t == IR_CHOICE) {
            bb_choice_state_t * zc = (bb_choice_state_t *)(intptr_t)bb->ival;
            if (zc) { snap[i].ch_cur = zc->cur; snap[i].ch_mark = zc->mark; snap[i].ch_saved_env = zc->saved_env;
                      snap[i].ch_last_body = zc->last_body; snap[i].ch_last_act = zc->last_act;
                      snap[i].ch_cp = zc->cp; snap[i].ch_cut_barrier = zc->cut_barrier;
                      if (zc->nbodies > 0 && zc->bodies) {
                          snap[i].ch_nbodies = zc->nbodies;
                          snap[i].ch_body_snaps = (void **)calloc((size_t)zc->nbodies, sizeof(void *));
                          if (snap[i].ch_body_snaps)
                              for (int b = 0; b < zc->nbodies; b++)
                                  snap[i].ch_body_snaps[b] = (void *)bb_snapshot_state(zc->bodies[b]);
                      } }
        }
    }
    return snap;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_restore_state(IR_graph_t * bbg, bb_node_state_t * snap) {
    if (!bbg || !snap) { free(snap); return; }
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        bb->value   = snap[i].value;
        bb->counter = snap[i].counter;
        bb->state   = snap[i].state;
        if (bb->t == IR_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)bb->ival;
            if (zc) zc->cs = snap[i].resolve_cs;
        } else if (bb->t == IR_CHOICE) {
            bb_choice_state_t * zc = (bb_choice_state_t *)(intptr_t)bb->ival;
            if (zc) { zc->cur = snap[i].ch_cur; zc->mark = snap[i].ch_mark; zc->saved_env = snap[i].ch_saved_env;
                      zc->last_body = (IR_graph_t *)snap[i].ch_last_body; zc->last_act = snap[i].ch_last_act;
                      zc->cp = snap[i].ch_cp; zc->cut_barrier = snap[i].ch_cut_barrier;
                      if (snap[i].ch_body_snaps) {
                          for (int b = 0; b < snap[i].ch_nbodies; b++)
                              if (b < zc->nbodies && zc->bodies)
                                  bb_restore_state(zc->bodies[b], (bb_node_state_t *)snap[i].ch_body_snaps[b]);
                              else
                                  free(snap[i].ch_body_snaps[b]);
                          free(snap[i].ch_body_snaps); snap[i].ch_body_snaps = NULL;
                      } }
        }
    }
    free(snap);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void IR_free(IR_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        free(bb);
    }
    free(bbg->all);
    free(bbg);
}
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
static void print_port(FILE * fp, const IR_graph_t * bbg, const char * label, const IR_t * bb) {
    int idx = bb_index_of(bbg, bb);
    if (!bb) fprintf(fp, " %s=.", label);
    else if (idx >= 0) fprintf(fp, " %s=%d", label, idx);
    else fprintf(fp, " %s=set", label);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_print(const IR_graph_t * bbg, FILE * fp) {
    if (!bbg) { fprintf(fp, "(null IR_graph_t)\n"); return; }
    static const char * lang_names[] = { "?", "SNO", "SCO", "REB", "ICN", "PL", "RKU" };
    const char * lname = (bbg->lang >= 1 && bbg->lang <= 6) ? lang_names[bbg->lang] : "?";
    fprintf(fp, "IR_graph_t lang=%s n=%d entry=%d\n", lname, bbg->n, bb_index_of(bbg, bbg->entry));
    for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        if (!bb) continue;
        fprintf(fp, "  [%d] %s", i, bb_op_name(bb->t));
        print_port(fp, bbg, "α", bb->α);
        print_port(fp, bbg, "β", bb->β);
        print_port(fp, bbg, "γ", bb->γ);
        print_port(fp, bbg, "ω", bb->ω);
        switch (bb->t) {
            case IR_LIT_I: fprintf(fp, " ival=%lld", (long long)bb->ival); break;
            case IR_LIT_F: fprintf(fp, " dval=%g",   bb->dval);             break;
            case IR_LIT_S: fprintf(fp, " sval=\"%s\"", bb->sval ? bb->sval : ""); break;
            case IR_VAR:   fprintf(fp, " var=\"%s\"%s",  bb->sval ? bb->sval : "", bb->state == 1 ? " scope=global" : ""); break;
            case IR_FIELD_GET:
            case IR_FIELD_SET: fprintf(fp, " field=\"%s\"", bb->sval ? bb->sval : ""); break;
            default: break;
        }
        fprintf(fp, "\n");
    }
}
