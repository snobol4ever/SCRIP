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
    [IR_VAR_REF] = "IR_VAR_REF",
    [IR_ACTIVATE] = "IR_ACTIVATE",
    [IR_ASSIGN] = "IR_ASSIGN",
    [IR_ASSIGN_VAR] = "IR_ASSIGN_VAR",
    [IR_BINOP] = "IR_BINOP",
    [IR_BINOP_TEST] = "IR_BINOP_TEST",
    [IR_BINOP_RELOP_VAL] = "IR_BINOP_RELOP_VAL",
    [IR_UNOP] = "IR_UNOP",
    [IR_UNOP_TEST] = "IR_UNOP_TEST",
    [IR_CALL] = "IR_CALL",
    [IR_CALL_PROC_STAGED] = "IR_CALL_PROC_STAGED",
    [IR_CALL_BUILTIN] = "IR_CALL_BUILTIN",
    [IR_CALL_BUILTIN_GEN] = "IR_CALL_BUILTIN_GEN",
    [IR_CALL_BUILTIN_ICON] = "IR_CALL_BUILTIN_ICON",
    [IR_CALL_BUILTIN_SNOBOL4] = "IR_CALL_BUILTIN_SNOBOL4",
    [IR_FAIL] = "IR_FAIL",
    [IR_SUCCEED]   = "IR_SUCCEED",
    [IR_SUSPEND]   = "IR_SUSPEND",
    [IR_RETURN] = "IR_RETURN",
    [IR_TO] = "IR_TO",
    [IR_TO_BY] = "IR_TO_BY",
    [IR_PROC_GEN] = "IR_PROC_GEN",
    [IR_RANDOM] = "IR_RANDOM",
    [IR_KEYWORD_ICON] = "IR_KEYWORD_ICON",
    [IR_KEYWORD_ICON_GEN] = "IR_KEYWORD_ICON_GEN",
    [IR_KEYWORD_SNOBOL4] = "IR_KEYWORD_SNOBOL4",
    [IR_KEYWORD_ASSIGN] = "IR_KEYWORD_ASSIGN",
    [IR_LIT_CHARSET] = "IR_LIT_CHARSET",
    [IR_FIELD_GET] = "IR_FIELD_GET",
    [IR_FIELD_VAR] = "IR_FIELD_VAR",
    [IR_NULLTEST_VAR] = "IR_NULLTEST_VAR",
    [IR_GOTO] = "IR_GOTO",
    [IR_SUBSCRIPT] = "IR_SUBSCRIPT",
    [IR_LIMIT]  = "IR_LIMIT",
    [IR_REPALT] = "IR_REPALT",
    [IR_REV_ASSIGN] = "IR_REV_ASSIGN",
    [IR_REV_ASSIGN_VAR] = "IR_REV_ASSIGN_VAR",
    [IR_REV_SWAP] = "IR_REV_SWAP",
    [IR_SCAN]       = "IR_SCAN",
    [IR_SCAN_ENTER] = "IR_SCAN_ENTER",
    [IR_SCAN_ANY]   = "IR_SCAN_ANY",
    [IR_SCAN_BAL]   = "IR_SCAN_BAL",
    [IR_SCAN_FIND]  = "IR_SCAN_FIND",
    [IR_SCAN_MANY]  = "IR_SCAN_MANY",
    [IR_SCAN_MATCH] = "IR_SCAN_MATCH",
    [IR_SCAN_MOVE]  = "IR_SCAN_MOVE",
    [IR_SCAN_POS]   = "IR_SCAN_POS",
    [IR_SCAN_TAB]   = "IR_SCAN_TAB",
    [IR_SCAN_UPTO]  = "IR_SCAN_UPTO",
    [IR_SCAN_SEQUENCE]  = "IR_SCAN_SEQUENCE",
    [IR_SCAN_ALTERNATE] = "IR_SCAN_ALTERNATE",
    [IR_SWAP]   = "IR_SWAP",
    [IR_SWAP_VAR] = "IR_SWAP_VAR",
    [IR_PROC_VALUE] = "IR_PROC_VALUE",
    [IR_CALL_VALUE] = "IR_CALL_VALUE",
    [IR_DEREF] = "IR_DEREF",
    [IR_MAKE_LIST] = "IR_MAKE_LIST",
    [IR_INITIAL] = "IR_INITIAL",
    [IR_ITERATE] = "IR_ITERATE",
    [IR_CREATE] = "IR_CREATE",
    [IR_CORET] = "IR_CORET",
    [IR_COFAIL] = "IR_COFAIL",
    [IR_CONJUNCTION] = "IR_CONJUNCTION",
    [IR_MOVE_LABEL] = "IR_MOVE_LABEL",
    [IR_INDIRECT_GOTO] = "IR_INDIRECT_GOTO",
    [IR_DISJUNCTION] = "IR_DISJUNCTION",
    [IR_MATCH] = "IR_MATCH",
    [IR_MATCH_LIT] = "IR_MATCH_LIT",
    [IR_MATCH_ANY] = "IR_MATCH_ANY",
    [IR_MATCH_NOTANY] = "IR_MATCH_NOTANY",
    [IR_MATCH_SPAN] = "IR_MATCH_SPAN",
    [IR_MATCH_SPAN_VAR] = "IR_MATCH_SPAN_VAR",
    [IR_MATCH_BREAK] = "IR_MATCH_BREAK",
    [IR_MATCH_BREAKX] = "IR_MATCH_BREAKX",
    [IR_MATCH_LEN] = "IR_MATCH_LEN",
    [IR_MATCH_POS] = "IR_MATCH_POS",
    [IR_MATCH_RPOS] = "IR_MATCH_RPOS",
    [IR_MATCH_TAB] = "IR_MATCH_TAB",
    [IR_MATCH_RTAB] = "IR_MATCH_RTAB",
    [IR_MATCH_ARB] = "IR_MATCH_ARB",
    [IR_MATCH_ARBNO] = "IR_MATCH_ARBNO",
    [IR_MATCH_REM] = "IR_MATCH_REM",
    [IR_MATCH_BAL] = "IR_MATCH_BAL",
    [IR_MATCH_FENCE1] = "IR_MATCH_FENCE1",
    [IR_MATCH_ABORT] = "IR_MATCH_ABORT",
    [IR_MATCH_SEQUENCE] = "IR_MATCH_SEQUENCE",
    [IR_MATCH_ALTERNATE] = "IR_MATCH_ALTERNATE",
    [IR_MATCH_ASSIGN_IMM] = "IR_MATCH_ASSIGN_IMM",
    [IR_MATCH_ASSIGN_COND] = "IR_MATCH_ASSIGN_COND",
    [IR_MATCH_ASSIGN_SAVE] = "IR_MATCH_ASSIGN_SAVE",
    [IR_MATCH_ATP] = "IR_MATCH_ATP",
    [IR_MATCH_CALLOUT] = "IR_MATCH_CALLOUT",
    [IR_MATCH_DEFER] = "IR_MATCH_DEFER",
    [IR_MATCH_PATREF] = "IR_MATCH_PATREF",
    [IR_MATCH_VALUE] = "IR_MATCH_VALUE",
    [IR_MATCH_HEAD] = "IR_MATCH_HEAD",
    [IR_MATCH_RELEASE] = "IR_MATCH_RELEASE",
    [IR_MATCH_REPLACE] = "IR_MATCH_REPLACE",
    [IR_GOTO_DEFERRED] = "IR_GOTO_DEFERRED",
    [IR_MATCH_RETRY] = "IR_MATCH_RETRY",
    [IR_MATCH_ADVANCE] = "IR_MATCH_ADVANCE",
    [IR_GLIT] = "IR_GLIT",
    [IR_GCC] = "IR_GCC",
    [IR_GSUBRULE] = "IR_GSUBRULE",
    [IR_BOUND] = "IR_BOUND",
    [IR_UNMARK] = "IR_UNMARK",
    [IR_SAVE_RESTORE] = "IR_SAVE_RESTORE",
    [IR_COERCE_STRING] = "IR_COERCE_STRING",
    [IR_COERCE_INTEGER] = "IR_COERCE_INTEGER",
    [IR_COERCE_NUMERIC] = "IR_COERCE_NUMERIC",
    [IR_COERCE_REAL] = "IR_COERCE_REAL",
    [IR_CMP_TEST] = "IR_CMP_TEST",
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * bb_op_name(IR_e k) {
    if (k >= 0 && k < IR_OP_COUNT) return kind_names[k];
    return "IR_UNKNOWN";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_alloc(int max_nodes) {
    IR_graph_t * bbg = calloc(1, sizeof(IR_graph_t));
    if (!bbg) return NULL;
    bbg->all  = calloc((size_t)max_nodes, sizeof(IR_t *));
    if (!bbg->all) { free(bbg); return NULL; }
    bbg->n    = 0;
    bbg->max  = max_nodes;
    bbg->entry = NULL;
    bbg->resume_slot = -1;
    bbg->resumable_callable = 0;
    bbg->zeta_mark_slot = -1;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * IR_node_alloc(IR_graph_t * bbg, IR_e t) {
    IR_t * bb = calloc(1, sizeof(IR_t));
    if (!bb) return NULL;
    bb->op       = t;
    bb->γ.node = NULL;
    bb->ω.node = NULL;
    if (bbg->n >= bbg->max) { int nm = bbg->max > 0 ? bbg->max * 2 : 16; IR_t ** na = (IR_t **) realloc(bbg->all, (size_t) nm * sizeof(IR_t *)); if (!na) { free(bb); return NULL; } bbg->all = na; bbg->max = nm; }
    bbg->all[bbg->n++] = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ir_operand_push(IR_t * nd, IR_t * child) {
    if (!nd) return 0;
    IR_t ** p = realloc(nd->operands, (size_t)(nd->n_operands + 1) * sizeof(IR_t *));
    if (!p) return 0;
    nd->operands = p;
    nd->operands[nd->n_operands++] = child;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void IR_free(IR_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        free(bb);
    }
    free(bbg->all);
    free(bbg->vslots);
    free((void *)bbg->pnames);
    free(bbg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void IR_free_dyn(void * g) { IR_free((IR_graph_t *)g); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_program_free(bb_program_t * p) {
    if (!p) return;
    for (int i = 0; i < p->count; i++) { IR_free(p->table[i]); p->table[i] = NULL; }
    p->count = 0;
    p->cap   = 0;
    free(p->table);
    p->table = NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_index_of(const IR_graph_t * bbg, const IR_t * bb) {
    if (!bbg || !bb) return -1;
    for (int i = 0; i < bbg->n; i++) if (bbg->all[i] == bb) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_emit_order_visit(const IR_graph_t *bbg, const IR_t *nd, char *vis, int *order, int *norder) {
    if (!nd) return;
    int ix = -1; for (int _i = 0; _i < bbg->n; _i++) if (bbg->all[_i] == nd) { ix = _i; break; }
    if (ix < 0 || vis[ix]) return;
    vis[ix] = 1; order[(*norder)++] = ix;
    bb_emit_order_visit(bbg, nd->γ.node, vis, order, norder);
    bb_emit_order_visit(bbg, nd->ω.node, vis, order, norder);
    int na = 0; IR_t * const * ops = NULL;
    na = nd->n_operands; ops = nd->operands;
    if (ops) for (int j = 0; j < na; j++) if (ops[j]) bb_emit_order_visit(bbg, ops[j], vis, order, norder);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ir_node_produces_value(IR_e op) {
    return op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET || op == IR_VAR || op == IR_VAR_REF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_COERCE_NUMERIC || op == IR_COERCE_REAL || op == IR_CMP_TEST
        || op == IR_BINOP || op == IR_BINOP_TEST || op == IR_UNOP || op == IR_UNOP_TEST || op == IR_SUBSCRIPT || op == IR_RANDOM || op == IR_LIMIT
        || op == IR_SWAP || op == IR_SWAP_VAR || op == IR_CALL || ir_is_call_kind(op) || op == IR_PROC_GEN || op == IR_PROC_VALUE
        || op == IR_FIELD_GET || op == IR_FIELD_VAR || op == IR_NULLTEST_VAR || op == IR_SCAN_TAB || op == IR_SCAN_MOVE || op == IR_SCAN_MATCH
        || op == IR_SCAN_POS || op == IR_SCAN_UPTO || op == IR_SCAN_ANY || op == IR_SCAN_MANY || op == IR_SCAN_FIND || op == IR_SCAN_BAL
        || op == IR_SCAN_SEQUENCE || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION
        || op == IR_CREATE || op == IR_ACTIVATE || op == IR_REV_ASSIGN || op == IR_REV_ASSIGN_VAR || op == IR_REV_SWAP || op == IR_KEYWORD_ASSIGN;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drv_vslot_push(IR_graph_t * g, const char * name, int off) {
    if (!name) return;
    for (int i = 0; i < g->n_vslots; i++) if (g->vslots[i].name && strcmp(g->vslots[i].name, name) == 0) return;
    { void * nv = realloc(g->vslots, (size_t)(g->n_vslots + 1) * sizeof(g->vslots[0])); if (!nv) return; g->vslots = nv; }
    g->vslots[g->n_vslots].name = name; g->vslots[g->n_vslots].off = off; g->n_vslots++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ir_varslot_of(const IR_graph_t * g, const char * name) {
    if (!g || !name) return -1;
    for (int i = 0; i < g->n_vslots; i++) if (g->vslots[i].name && strcmp(g->vslots[i].name, name) == 0) return g->vslots[i].off;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int graph_has_local(const IR_graph_t * g, const char * name) {
    if (!g || !name) return 0;
    for (int i = 0; i < g->nparams; i++) if (g->pnames && g->pnames[i] && strcmp(g->pnames[i], name) == 0) return 1;
    for (int i = 0; i < g->nlocals; i++) if (g->lnames && g->lnames[i] && strcmp(g->lnames[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_drive_slot_assign(IR_graph_t * g) {
    if (!g) return;
    extern void zls_build(IR_graph_t *);
    extern int zls_off(const IR_t *);
    extern int zls_g_nslots(const IR_graph_t *);
    extern int zls_g_region(const IR_graph_t *);
    extern int zls_g_resume(const IR_graph_t *);
    extern int zls_g_zeta_mark(const IR_graph_t *);
    extern int zls_g_vslot_count(const IR_graph_t *);
    extern const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
    zls_build(g);
    g->n_vslots = 0;
    for (int v = 0; v < zls_g_vslot_count(g); v++) { int off = -1; const char * vn = zls_g_vslot_get(g, v, &off); if (vn && off >= 0) drv_vslot_push(g, vn, off); }
    g->resume_slot = zls_g_resume(g);
    g->zeta_mark_slot = zls_g_zeta_mark(g);
    g->jcon_value_region = zls_g_region(g);
    g->nvalue_slots = zls_g_nslots(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const int * g_seq_of_node = (const int *)0;
static int g_seq_of_node_n = 0;
static int bb_seq_of(int ix) { return (g_seq_of_node && ix >= 0 && ix < g_seq_of_node_n) ? g_seq_of_node[ix] : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_ref_fmt(const IR_graph_t *bbg, const IR_t *target, char *out, size_t outsz) {
    if (!target) { snprintf(out, outsz, "."); return; }
    int ix = bb_index_of(bbg, target);
    extern int zls_off(const IR_t *);
    extern int zls_result_off(const IR_t *);
    int _z = zls_result_off(target);
    int sq = bb_seq_of(ix);
    if (sq >= 0 && _z >= 0)  snprintf(out, outsz, "%d", sq);
    else if (sq >= 0)        snprintf(out, outsz, "%d@", sq);
    else if (_z >= 0)        snprintf(out, outsz, "+%d", _z);
    else                     snprintf(out, outsz, "?");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_print_node_line(const IR_graph_t *bbg, FILE *fp, int seq, int i, int verbose) {
    const IR_t * bb = (i >= 0 && i < bbg->n) ? bbg->all[i] : NULL;
    if (!bb) { fprintf(fp, "%-6s    .    .  %-22s []\n", "(null)", "(null)"); return; }
    char self[12]; { extern int zls_result_off(const IR_t *); int _z = zls_result_off(bb); int sq2 = bb_seq_of(i); int ln = (sq2 >= 0) ? sq2 : (seq >= 0 ? seq : -1);
                     if (ln >= 0 && _z >= 0)  snprintf(self, sizeof self, "%-5d", ln);
                     else if (ln >= 0)        { char t[8]; snprintf(t, sizeof t, "%d@", ln); snprintf(self, sizeof self, "%-5s", t); }
                     else                     snprintf(self, sizeof self, "%-5s", "?"); }
    char gp[12], wp[12];
    bb_ref_fmt(bbg, bb->γ.node, gp, sizeof gp);
    bb_ref_fmt(bbg, bb->ω.node, wp, sizeof wp);
    int na = 0; IR_t * const * ops = NULL;
    na = bb->n_operands; ops = bb->operands;
    char ob[160]; size_t op = 0; ob[0] = 0;
    for (int j = 0; j < na && op < sizeof ob - 4; j++) {
        char r[12]; bb_ref_fmt(bbg, ops ? ops[j] : NULL, r, sizeof r);
        size_t rl = strlen(r);
        if (op + rl + 2 >= sizeof ob) break;
        if (op > 0) ob[op++] = ',';
        memcpy(ob + op, r, rl); op += rl; ob[op] = 0;
    }
    const char * opn = bb_op_name(bb->op);
    if (opn && !strncmp(opn, "IR_", 3)) opn += 3;
    fprintf(fp, "%-6s %-4s %-4s %-22s [%s]", self, gp, wp, opn, ob);
    if (verbose) fprintf(fp, " (n%d)", i);
    switch (bb->op) {
        case IR_LIT_INTEGER: fprintf(fp, " %lld", (long long)IR_LIT(bb).ival); break;
        case IR_LIT_REAL: fprintf(fp, " %g", IR_LIT(bb).dval); break;
        case IR_LIT_STRING: fprintf(fp, " \"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_LIT_CHARSET: fprintf(fp, " \"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_VAR: fprintf(fp, " var=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_ASSIGN: fprintf(fp, " var=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_KEYWORD_ICON: case IR_KEYWORD_ICON_GEN: case IR_KEYWORD_SNOBOL4: fprintf(fp, " kw=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_BINOP: case IR_BINOP_TEST: fprintf(fp, " binop=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_SUCCEED: if (IR_LIT(bb).ival != 0) fprintf(fp, " stno=%d", (int)IR_LIT(bb).ival); break;
        case IR_CALL: case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
        case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4:
            fprintf(fp, " \"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        default: break;
    }
    fprintf(fp, "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_print(const IR_graph_t * bbg, FILE * fp) { bb_print_v(bbg, fp, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_print_v(const IR_graph_t * bbg, FILE * fp, int verbose) {
    if (!bbg) { fprintf(fp, "(null IR_graph_t)\n"); return; }
    int nn0 = bbg->n;
    char * vis0 = (char *) calloc(nn0 > 0 ? nn0 : 1, 1);
    int * order0 = (int *) malloc((size_t)(nn0 > 0 ? nn0 : 1) * sizeof(int));
    int * seqmap = (int *) malloc((size_t)(nn0 > 0 ? nn0 : 1) * sizeof(int));
    int norder = 0;
    if (vis0 && order0 && seqmap) {
        bb_emit_order_visit(bbg, bbg->entry, vis0, order0, &norder);
        for (int i = 0; i < nn0; i++) seqmap[i] = -1;
        for (int sq = 0; sq < norder; sq++) if (order0[sq] >= 0 && order0[sq] < nn0) seqmap[order0[sq]] = sq;
        int nxt = norder;
        for (int i = 0; i < nn0; i++) if (!vis0[i] && bbg->all[i] && seqmap[i] < 0) seqmap[i] = nxt++;
        g_seq_of_node = seqmap; g_seq_of_node_n = nn0;
    }
    char ent[12]; bb_ref_fmt(bbg, bbg->entry, ent, sizeof ent);
    fprintf(fp, "IR_graph_t n=%d entry=%s nslots=%d\n", bbg->n, ent, bbg->nslots);
    if (verbose)
        fprintf(fp, ";  slot   γ    ω    kind                   [operands]  payload   (self/γ/ω/operands: N=box at line N (result slot), N@=wiring at line N; (nN)=node id, verbose only "
                    "— linear emit order: γ-spine DFS from entry, then ω, then operands)\n");
    else         fprintf(fp, ";  slot   γ    ω    kind                   [operands]  payload      (N = box at line N, its result slot; N@ = wiring at line N, no value)\n");
    int nn = nn0;
    if (vis0 && order0 && seqmap) {
        for (int sq = 0; sq < norder; sq++) bb_print_node_line(bbg, fp, sq, order0[sq], verbose);
        int any_unreached = 0;
        for (int i = 0; i < nn; i++) if (!vis0[i] && bbg->all[i]) { any_unreached = 1; break; }
        if (any_unreached) fprintf(fp, "; --- unreached (not on emit spine; shown for completeness) ---\n");
        if (any_unreached) for (int i = 0; i < nn; i++) if (!vis0[i] && bbg->all[i]) bb_print_node_line(bbg, fp, seqmap[i], i, verbose);
    } else { for (int i = 0; i < nn; i++) bb_print_node_line(bbg, fp, i, i, verbose); }
    g_seq_of_node = (const int *)0; g_seq_of_node_n = 0;
    free(vis0); free(order0); free(seqmap);
    for (int i = 0; i < bbg->n; i++) {
        const IR_t * bb = bbg->all[i];
        if (!bb) continue;
        IR_graph_t * pg = (IR_graph_t *)0;
        if (pg) bb_print(pg, fp);
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
