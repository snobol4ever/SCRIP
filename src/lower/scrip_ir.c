#include "BB.h"
#include "bb_program.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static const char * kind_names[BB_OP_COUNT] = {
    [BB_LIT_I] = "BB_LIT_I",
    [BB_LIT_S] = "BB_LIT_S",
    [BB_LIT_F] = "BB_LIT_F",
    [BB_LIT_NUL] = "BB_LIT_NUL",
    [BB_VAR] = "BB_VAR",
    [BB_ASSIGN] = "BB_ASSIGN",
    [BB_AUGOP] = "BB_AUGOP",
    [BB_BINOP] = "BB_BINOP",
    [BB_UNOP] = "BB_UNOP",
    [BB_CALL] = "BB_CALL",
    [BB_SEQ] = "BB_SEQ",
    [BB_FAIL] = "BB_FAIL",
    [BB_SUCCEED] = "BB_SUCCEED",
    [BB_GOTO] = "BB_GOTO",
    [BB_RETURN] = "BB_RETURN",
    [BB_IF] = "BB_IF",
    [BB_CONJ] = "BB_CONJ",
    [BB_TO_BY] = "BB_TO_BY",
    [BB_EVERY] = "BB_EVERY",
    [BB_WHILE] = "BB_WHILE",
    [BB_UNTIL] = "BB_UNTIL",
    [BB_REPEAT] = "BB_REPEAT",
    [BB_ALT] = "BB_ALT",
    [BB_SIZE] = "BB_SIZE",
    [BB_CASE] = "BB_CASE",
    [BB_LIMIT] = "BB_LIMIT",
    [BB_SUSPEND] = "BB_SUSPEND",
    [BB_PROC] = "BB_PROC",
    [BB_SCAN] = "BB_SCAN",
    [BB_NONNULL] = "BB_NONNULL",
    [BB_INTERROGATE] = "BB_INTERROGATE",
    [BB_NOT] = "BB_NOT",
    [BB_PAT_LIT] = "BB_PAT_LIT",
    [BB_PAT_ANY] = "BB_PAT_ANY",
    [BB_PAT_SPAN] = "BB_PAT_SPAN",
    [BB_PAT_BREAK] = "BB_PAT_BREAK",
    [BB_PAT_ARB] = "BB_PAT_ARB",
    [BB_PAT_ARBNO] = "BB_PAT_ARBNO",
    [BB_PAT_CAT] = "BB_PAT_CAT",
    [BB_PAT_ALT] = "BB_PAT_ALT",
    [BB_PAT_ASSIGN_IMM] = "BB_PAT_ASSIGN_IMM",
    [BB_PAT_ASSIGN_COND] = "BB_PAT_ASSIGN_COND",
    [BB_PAT_LEN] = "BB_PAT_LEN",
    [BB_PAT_NOTANY] = "BB_PAT_NOTANY",
    [BB_PAT_POS] = "BB_PAT_POS",
    [BB_PAT_TAB] = "BB_PAT_TAB",
    [BB_PAT_REM] = "BB_PAT_REM",
    [BB_PAT_FENCE] = "BB_PAT_FENCE",
    [BB_PAT_ABORT] = "BB_PAT_ABORT",
    [BB_PAT_CALLOUT] = "BB_PAT_CALLOUT",
    [BB_PAT_DEFER] = "BB_PAT_DEFER",
    [BB_CHOICE] = "BB_CHOICE",
    [BB_UNIFY] = "BB_UNIFY",
    [BB_CUT] = "BB_CUT",
    [BB_GOAL] = "BB_GOAL",
    [BB_BUILTIN] = "BB_BUILTIN",
    [BB_LOGICVAR] = "BB_LOGICVAR",
    [BB_ATOM] = "BB_ATOM",
    [BB_STRUCT] = "BB_STRUCT",
    [BB_ARITH] = "BB_ARITH",
    [BB_DISJ] = "BB_DISJ",
    [BB_GCONJ] = "BB_GCONJ",
    [BB_ITE] = "BB_ITE",
    [BB_CATCH] = "BB_CATCH",
    [BB_TO] = "BB_TO",
    [BB_UPTO] = "BB_UPTO",
    [BB_ITERATE] = "BB_ITERATE",
    [BB_GEN_ALT] = "BB_GEN_ALT",
    [BB_GEN_BINOP] = "BB_GEN_BINOP",
    [BB_TO_NESTED] = "BB_TO_NESTED",
    [BB_PROC_GEN] = "BB_PROC_GEN",
    [BB_BREAK] = "BB_BREAK",
    [BB_NEXT] = "BB_NEXT",
    [BB_IDENTICAL] = "BB_IDENTICAL",
    [BB_NULL_TEST] = "BB_NULL_TEST",
    [BB_RANDOM] = "BB_RANDOM",
    [BB_NEG] = "BB_NEG",
    [BB_POS] = "BB_POS",
    [BB_CSET_COMPL] = "BB_CSET_COMPL",
    [BB_CSET_UNION] = "BB_CSET_UNION",
    [BB_CSET_DIFF] = "BB_CSET_DIFF",
    [BB_CSET_INTER] = "BB_CSET_INTER",
    [BB_GEN_SCAN] = "BB_GEN_SCAN",
    [BB_KEYWORD] = "BB_KEYWORD",
    [BB_BINOP_GEN] = "BB_BINOP_GEN",
    [BB_IDX] = "BB_IDX",
    [BB_SECTION] = "BB_SECTION",
    [BB_LIST_BANG] = "BB_LIST_BANG",
    [BB_RECORD_DEF] = "BB_RECORD_DEF",
    [BB_FIELD_GET] = "BB_FIELD_GET",
    [BB_FIELD_SET] = "BB_FIELD_SET",
    [BB_IDX_SET] = "BB_IDX_SET",
    [BB_KEY_GEN] = "BB_KEY_GEN",
    [BB_SWAP] = "BB_SWAP",
    [BB_SEQ_EXPR] = "BB_SEQ_EXPR",
    [BB_INITIAL] = "BB_INITIAL",
    [BB_LCONCAT] = "BB_LCONCAT",
    [BB_FIND_GEN] = "BB_FIND_GEN",
    [BB_SEQ_GEN] = "BB_SEQ_GEN",
    [BB_NFA_CHAR] = "BB_NFA_CHAR",
    [BB_NFA_ANY] = "BB_NFA_ANY",
    [BB_NFA_CLASS] = "BB_NFA_CLASS",
    [BB_NFA_SPLIT] = "BB_NFA_SPLIT",
    [BB_NFA_EPS] = "BB_NFA_EPS",
    [BB_NFA_BOL] = "BB_NFA_BOL",
    [BB_NFA_EOL] = "BB_NFA_EOL",
    [BB_NFA_CAP_OPEN] = "BB_NFA_CAP_OPEN",
    [BB_NFA_CAP_CLOSE] = "BB_NFA_CAP_CLOSE",
    [BB_NFA_ACCEPT] = "BB_NFA_ACCEPT",
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * bb_op_name(BB_op_t k) {
    if (k >= 0 && k < BB_OP_COUNT) return kind_names[k];
    return "BB_UNKNOWN";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
BB_graph_t * BB_alloc(int max_nodes, int lang) {
    BB_graph_t * bbg = calloc(1, sizeof(BB_graph_t));
    if (!bbg) return NULL;
    bbg->all  = calloc((size_t)max_nodes, sizeof(BB_t *));
    if (!bbg->all) { free(bbg); return NULL; }
    bbg->n    = 0;
    bbg->max  = max_nodes;
    bbg->lang = lang;
    bbg->entry = NULL;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
BB_t * BB_node_alloc(BB_graph_t * bbg, BB_op_t t) {
    BB_t * bb = calloc(1, sizeof(BB_t));
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_operand_aux_set(BB_graph_t * bbg, BB_t * bb, BB_t * const * src, int n) {
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
    bbg->operand_aux[slot].operands = (BB_t **)calloc((size_t)n, sizeof(BB_t *));
    if (!bbg->operand_aux[slot].operands) return -1;
    if (src) for (int i = 0; i < n; i++) bbg->operand_aux[slot].operands[i] = src[i];
    bbg->operand_aux[slot].n = n;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
BB_t * const * bb_operand_aux_get(const BB_graph_t * bbg, const BB_t * bb, int * out_n) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_reset(BB_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        BB_t * bb = bbg->all[i];
        if (!bb) continue;
        bb->value   = FAILDESCR;
        if (bb->t != BB_PAT_ARBNO) bb->counter = 0;
        bb->state   = 0;
    }
    ag_ring_clear(bbg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(BB_graph_t * bbg) {
    if (!bbg || bbg->n <= 0) return NULL;
    bb_node_state_t * snap = (bb_node_state_t *)malloc((size_t)bbg->n * sizeof(bb_node_state_t));
    if (!snap) return NULL;
    for (int i = 0; i < bbg->n; i++) {
        BB_t * bb = bbg->all[i];
        if (!bb) { memset(&snap[i], 0, sizeof snap[i]); snap[i].value = FAILDESCR; continue; }
        snap[i].value   = bb->value;
        snap[i].counter = bb->counter;
        snap[i].state   = bb->state;
        snap[i].resolve_cs = NULL; snap[i].ch_cur = 0; snap[i].ch_mark = 0; snap[i].ch_saved_env = NULL;
        snap[i].ch_last_body = NULL; snap[i].ch_last_act = NULL;
        snap[i].ch_cp = NULL; snap[i].ch_cut_barrier = NULL;
        snap[i].ch_body_snaps = NULL; snap[i].ch_nbodies = 0;
        if (bb->t == BB_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)bb->ival;
            if (zc) snap[i].resolve_cs = zc->cs;
        } else if (bb->t == BB_CHOICE) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_restore_state(BB_graph_t * bbg, bb_node_state_t * snap) {
    if (!bbg || !snap) { free(snap); return; }
    for (int i = 0; i < bbg->n; i++) {
        BB_t * bb = bbg->all[i];
        if (!bb) continue;
        bb->value   = snap[i].value;
        bb->counter = snap[i].counter;
        bb->state   = snap[i].state;
        if (bb->t == BB_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)bb->ival;
            if (zc) zc->cs = snap[i].resolve_cs;
        } else if (bb->t == BB_CHOICE) {
            bb_choice_state_t * zc = (bb_choice_state_t *)(intptr_t)bb->ival;
            if (zc) { zc->cur = snap[i].ch_cur; zc->mark = snap[i].ch_mark; zc->saved_env = snap[i].ch_saved_env;
                      zc->last_body = (BB_graph_t *)snap[i].ch_last_body; zc->last_act = snap[i].ch_last_act;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void BB_free(BB_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        BB_t * bb = bbg->all[i];
        if (!bb) continue;
        free(bb);
    }
    free(bbg->all);
    free(bbg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_program_add(bb_program_t * p, BB_graph_t * bbg) {
    if (!bbg) return -1;
    if (p->cap == 0) {
        p->cap   = 16;
        p->count = 0;
        p->table = (BB_graph_t **)calloc((size_t)p->cap, sizeof(BB_graph_t *));
    }
    if (p->count >= p->cap) {
        p->cap  *= 2;
        p->table = (BB_graph_t **)realloc(p->table, (size_t)p->cap * sizeof(BB_graph_t *));
    }
    int idx = p->count++;
    p->table[idx] = bbg;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_program_free(bb_program_t * p) {
    if (!p) return;
    for (int i = 0; i < p->count; i++) { BB_free(p->table[i]); p->table[i] = NULL; }
    p->count = 0;
    p->cap   = 0;
    free(p->table);
    p->table = NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void print_port(FILE * fp, const char * label, const BB_t * bb) {
    fprintf(fp, " %s=%s", label, bb ? "set" : "NULL");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_print(const BB_graph_t * bbg, FILE * fp) {
    if (!bbg) { fprintf(fp, "(null BB_graph_t)\n"); return; }
    static const char * lang_names[] = { "?", "SNO", "SCO", "REB", "ICN", "PL", "RKU" };
    const char * lname = (bbg->lang >= 1 && bbg->lang <= 6) ? lang_names[bbg->lang] : "?";
    fprintf(fp, "BB_graph_t lang=%s n=%d entry=%s\n", lname, bbg->n, bbg->entry ? "set" : "NULL");
    for (int i = 0; i < bbg->n; i++) {
        const BB_t * bb = bbg->all[i];
        if (!bb) continue;
        fprintf(fp, "  [%d] %s", i, bb_op_name(bb->t));
        print_port(fp, "α", bb->α);
        print_port(fp, "β", bb->β);
        print_port(fp, "γ", bb->γ);
        print_port(fp, "ω", bb->ω);
        switch (bb->t) {
            case BB_LIT_I: fprintf(fp, " ival=%lld", (long long)bb->ival); break;
            case BB_LIT_F: fprintf(fp, " dval=%g",   bb->dval);             break;
            case BB_LIT_S: fprintf(fp, " sval=\"%s\"", bb->sval ? bb->sval : ""); break;
            case BB_VAR:   fprintf(fp, " var=\"%s\"",  bb->sval ? bb->sval : ""); break;
            case BB_FIELD_GET:
            case BB_FIELD_SET: fprintf(fp, " field=\"%s\"", bb->sval ? bb->sval : ""); break;
            default: break;
        }
        fprintf(fp, "\n");
    }
}
