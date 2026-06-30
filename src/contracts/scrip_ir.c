#include "IR.h"
#include "emit.h"
#include "bb_program.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static const char * kind_names[IR_OP_COUNT] = {
    [IR_LIT_INTEGER] = "IR_LIT_INTEGER",
    [IR_LIT_STRING] = "IR_LIT_STRING",
    [IR_LIT_REAL] = "IR_LIT_REAL",
    [IR_VAR] = "IR_VAR",
    [IR_ASSIGN] = "IR_ASSIGN",
    [IR_BINOP] = "IR_BINOP",
    [IR_BINOP_RELOP] = "IR_BINOP_RELOP",
    [IR_BINOP_GENERIC] = "IR_BINOP_GENERIC",
    [IR_UNOP] = "IR_UNOP",
    [IR_UNOP_TEST] = "IR_UNOP_TEST",
    [IR_UNOP_GENERIC] = "IR_UNOP_GENERIC",
    [IR_CALL] = "IR_CALL",
    [IR_CALL_PROC_STAGED] = "IR_CALL_PROC_STAGED",
    [IR_CALL_USERPROC] = "IR_CALL_USERPROC",
    [IR_CALL_BYNAME] = "IR_CALL_BYNAME",
    [IR_CALL_BUILTIN] = "IR_CALL_BUILTIN",
    [IR_CALL_GVAR_USERPROC] = "IR_CALL_GVAR_USERPROC",
    [IR_FAIL] = "IR_FAIL",
    [IR_SUCCEED] = "IR_SUCCEED",
    [IR_GOTO] = "IR_GOTO",
    [IR_RETURN] = "IR_RETURN",
    [IR_CONJ] = "IR_CONJ",
    [IR_ALT] = "IR_ALT",
    [IR_NOT] = "IR_NOT",
    [IR_TO] = "IR_TO",
    [IR_PROC_GEN] = "IR_PROC_GEN",
    [IR_KEYWORD] = "IR_KEYWORD",
    [IR_LIT_CHARSET] = "IR_LIT_CHARSET",
    [IR_FIELD] = "IR_FIELD",
    [IR_TERNOP] = "IR_TERNOP",
    [IR_SUBSCRIPT] = "IR_SUBSCRIPT",
    [IR_DEREF] = "IR_DEREF",
    [IR_MAKE_LIST] = "IR_MAKE_LIST",
    [IR_SCAN_SWAP] = "IR_SCAN_SWAP",
    [IR_ENTER_INIT] = "IR_ENTER_INIT",
    [IR_INDIRECT_GOTO] = "IR_INDIRECT_GOTO",
    [IR_UNREACHABLE] = "IR_UNREACHABLE",
    [IR_CREATE] = "IR_CREATE",
    [IR_CORET] = "IR_CORET",
    [IR_COFAIL] = "IR_COFAIL",
    [IR_MOVE] = "IR_MOVE",
    [IR_RESUME_VALUE] = "IR_RESUME_VALUE",
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
    bb->op       = t;
    bb->γ.node = NULL;
    bb->ω.node = NULL;
    bb->tmp      = -1;
    if (bbg->n >= bbg->max) { free(bb); return NULL; }
    bbg->all[bbg->n++] = bb;
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
    int ix = -1; for (int _i = 0; _i < bbg->n; _i++) if (bbg->all[_i] == nd) { ix = _i; break; }
    if (ix < 0 || vis[ix]) return;
    vis[ix] = 1; order[(*norder)++] = ix;
    bb_emit_order_visit(bbg, nd->γ.node, vis, order, norder);
    bb_emit_order_visit(bbg, nd->ω.node, vis, order, norder);
    int na = 0; IR_t * const * ops = NULL;
    if (nd->n_operands > 0) { na = nd->n_operands; ops = nd->operands; } else { ops = bb_operand_aux_get((IR_graph_t *)bbg, (IR_t *)nd, &na); }
    if (ops) for (int j = 0; j < na; j++) if (ops[j]) bb_emit_order_visit(bbg, ops[j], vis, order, norder);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_node_produces_value(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_VAR || op == IR_BINOP || op == IR_BINOP_RELOP || op == IR_BINOP_GENERIC || op == IR_UNOP || op == IR_UNOP_TEST || op == IR_UNOP_GENERIC || op == IR_TERNOP || op == IR_SUBSCRIPT || op == IR_CALL || ir_is_call_kind(op) || op == IR_PROC_GEN; }
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_tmp_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int cursor = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && ir_node_produces_value(nd->op)) { nd->tmp = cursor; cursor += 16; } }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_tmp_slot_assign_flat(IR_graph_t * g) {
    if (!g) return;
    int n = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op != IR_VAR && ir_node_produces_value(nd->op)) { nd->tmp = n * 16; n++; } }
    g->nvalue_slots = n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int jcon_converted_producer(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_KEYWORD; }
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_jcon_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int k = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && jcon_converted_producer(nd->op)) { nd->tmp = 16 + k * 16; k++; } }
    g->jcon_value_region = k * 16;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void ir_drive_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int k = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op != IR_VAR && ir_node_produces_value(nd->op)) { nd->tmp = 16 + k * 16; k++; } }
    g->jcon_value_region = 16 + k * 16;
    g->nvalue_slots = k;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void bb_print_node_line(const IR_graph_t *bbg, FILE *fp, int seq, int i) {
    const IR_t * bb = (i >= 0 && i < bbg->n) ? bbg->all[i] : NULL;
    char sq[8]; if (seq >= 0) snprintf(sq, sizeof sq, "%d", seq); else snprintf(sq, sizeof sq, "-");
    if (!bb) { fprintf(fp, "%4s %4d:    .    .  %-22s []\n", sq, i, "(null)"); return; }
    char gp[8], wp[8];
    if (bb->γ.node) { int _k=-1; for(int _i=0;_i<bbg->n;_i++) if(bbg->all[_i]==bb->γ.node){_k=_i;break;} snprintf(gp,sizeof gp,"%d",_k); } else snprintf(gp, sizeof gp, ".");
    if (bb->ω.node) { int _k=-1; for(int _i=0;_i<bbg->n;_i++) if(bbg->all[_i]==bb->ω.node){_k=_i;break;} snprintf(wp,sizeof wp,"%d",_k); } else snprintf(wp, sizeof wp, ".");
    int na = 0; IR_t * const * ops = NULL;
    if (bb->n_operands > 0) { na = bb->n_operands; ops = bb->operands; } else { ops = bb_operand_aux_get((IR_graph_t *)bbg, (IR_t *)bb, &na); }
    char ob[160]; int op = 0; ob[0] = 0;
    const char * opn = bb_op_name(bb->op);
    fprintf(fp, "%4s %4d: %4s %4s  %-22s [%s]", sq, i, gp, wp, opn, ob);
    if (bb->tmp >= 0) fprintf(fp, " tmp=%d", bb->tmp);
    if (bb->op == IR_SUCCEED && IR_LIT(bb).ival != 0) fprintf(fp, " stno=%d", (int)IR_LIT(bb).ival);
    switch (bb->op) {
        case IR_LIT_INTEGER: fprintf(fp, " ival=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_LIT_REAL: fprintf(fp, " dval=%g", IR_LIT(bb).dval); break;
        case IR_LIT_STRING: fprintf(fp, " sval=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_VAR: fprintf(fp, " var=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_BINOP: fprintf(fp, " binop=%lld", (long long)IR_LIT(bb).ival); break;
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
    for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        IR_graph_t * pg = (IR_graph_t *)0;
        if (pg) bb_print(pg, fp);
        for (int j = 0; j < bb->n_operands; j++) if (bb->operands[j]) bb_print((IR_graph_t *)(void *) bb->operands[j], fp);
    }
    { static int xd2 = -1; if (xd2 < 0) { const char * e = getenv("SCRIP_DUMP_X"); xd2 = (e && e[0] == '1') ? 1 : 0; }
      if (xd2) for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op != IR_CALL) continue;
        if (IR_LIT(bb).dval != 2.0 && IR_LIT(bb).dval != 3.0 && IR_LIT(bb).dval != 5.0) continue;
        IR_graph_t ** blks = (IR_graph_t **)0;
        if (!blks) continue;
        for (int j = 0; j < (int) IR_LIT(bb).ival; j++) if (blks[j]) { fprintf(fp, "; X argblk call=%d arg=%d\n", i, j); bb_print(blks[j], fp); }
      } }
}
