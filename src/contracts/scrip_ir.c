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
    [IR_UNOP] = "IR_UNOP",
    [IR_UNOP_TEST] = "IR_UNOP_TEST",
    [IR_CALL] = "IR_CALL",
    [IR_CALL_PROC_STAGED] = "IR_CALL_PROC_STAGED",
    [IR_CALL_BUILTIN] = "IR_CALL_BUILTIN",
    [IR_FAIL] = "IR_FAIL",
    [IR_SUCCEED]   = "IR_SUCCEED",
    [IR_SUSPEND]   = "IR_SUSPEND",
    [IR_RETURN] = "IR_RETURN",
    [IR_TO] = "IR_TO",
    [IR_TO_BY] = "IR_TO_BY",
    [IR_PROC_GEN] = "IR_PROC_GEN",
    [IR_RANDOM] = "IR_RANDOM",
    [IR_KEYWORD_ICON] = "IR_KEYWORD_ICON",
    [IR_KEYWORD_SNOBOL4] = "IR_KEYWORD_SNOBOL4",
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
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    return op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET || op == IR_VAR || op == IR_VAR_REF
        || op == IR_BINOP || op == IR_BINOP_TEST || op == IR_UNOP || op == IR_UNOP_TEST || op == IR_SUBSCRIPT || op == IR_RANDOM || op == IR_LIMIT
        || op == IR_SWAP || op == IR_SWAP_VAR || op == IR_CALL || ir_is_call_kind(op) || op == IR_PROC_GEN || op == IR_PROC_VALUE
        || op == IR_FIELD_GET || op == IR_FIELD_VAR || op == IR_NULLTEST_VAR || op == IR_SCAN_TAB || op == IR_SCAN_MOVE || op == IR_SCAN_MATCH
        || op == IR_SCAN_POS || op == IR_SCAN_UPTO || op == IR_SCAN_ANY || op == IR_SCAN_MANY || op == IR_SCAN_FIND || op == IR_SCAN_BAL
        || op == IR_CREATE || op == IR_ACTIVATE || op == IR_REV_ASSIGN || op == IR_REV_ASSIGN_VAR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_tmp_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int cursor = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && ir_node_produces_value(nd->op)) { nd->tmp = cursor; cursor += 16; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jcon_converted_producer(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_KEYWORD_ICON || op == IR_KEYWORD_SNOBOL4; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_jcon_slot_assign(IR_graph_t * g) {
    if (!g) return;
    int k = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && jcon_converted_producer(nd->op)) { nd->tmp = 16 + k * 16; k++; } }
    g->jcon_value_region = k * 16;
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
void ir_drive_slot_assign(IR_graph_t * g) {
    if (!g) return;
    extern int is_global(const char *);
    int base = 16 + (g->nparams > 0 ? g->nparams * 16 : 0);
    int k = 0;
    g->n_vslots = 0;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) drv_vslot_push(g, g->pnames[i], 16 + i * 16);
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if (nd->op == IR_TO || nd->op == IR_TO_BY) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_MAKE_LIST) { nd->tmp = base + k * 16; k += 1 + nd->n_operands; continue; }
        if (nd->op == IR_SCAN_ENTER) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_INITIAL) { nd->tmp = base + k * 16; k += 1; continue; }
        if (nd->op == IR_ITERATE) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_LIMIT) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_REPALT) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_REV_ASSIGN || nd->op == IR_REV_ASSIGN_VAR) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) { nd->tmp = base + k * 16; k += 1 + nd->n_operands; continue; }
        if (nd->op == IR_KEYWORD_ICON) { nd->tmp = base + k * 16; k += 2; continue; }
        if (nd->op == IR_KEYWORD_SNOBOL4) { nd->tmp = base + k * 16; k += 1; continue; }
        if (nd->op == IR_DEREF || nd->op == IR_ASSIGN_VAR || nd->op == IR_RANDOM || nd->op == IR_SWAP_VAR) { nd->tmp = base + k * 16; k += 1; continue; }
        if (nd->op == IR_CREATE) { nd->tmp = base + k * 16; k += 4; continue; }
        if (nd->op == IR_ASSIGN) { nd->tmp = base + k * 16; k += 1; continue; }
        if (nd->op == IR_INDIRECT_GOTO) { nd->tmp = base + k * 16; k += 2; continue; }
        if (ir_node_produces_value(nd->op)) { nd->tmp = base + k * 16; k++; }
    }
    g->resume_slot = -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) { g->resume_slot = base + k * 16; k += 1; break; }
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        const char * vn = (const char *)0;
        if (nd->op == IR_ASSIGN) vn = IR_LIT(nd).sval;
        else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) vn = IR_LIT(nd->operands[1]).sval;
        else if (nd->op == IR_VAR || nd->op == IR_VAR_REF) vn = IR_LIT(nd).sval;
        if (!vn || vn[0] == '&' || is_global(vn)) continue;
        if (ir_varslot_of(g, vn) >= 0) continue;
        drv_vslot_push(g, vn, base + k * 16);
        k++;
    }
    g->jcon_value_region = base + k * 16;
    g->nvalue_slots = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_ref_fmt(const IR_graph_t *bbg, const IR_t *target, char *out, size_t outsz) {
    if (!target) { snprintf(out, outsz, "."); return; }
    int ix = bb_index_of(bbg, target);
    if (target->tmp >= 0) snprintf(out, outsz, "s%d", target->tmp);
    else if (ix >= 0)     snprintf(out, outsz, "n%d", ix);
    else                  snprintf(out, outsz, "?");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_print_node_line(const IR_graph_t *bbg, FILE *fp, int seq, int i, int verbose) {
    const IR_t * bb = (i >= 0 && i < bbg->n) ? bbg->all[i] : NULL;
    char sq[8]; if (seq >= 0) snprintf(sq, sizeof sq, "%d", seq); else snprintf(sq, sizeof sq, "-");
    if (!bb) { fprintf(fp, "%4s %-6s    .    .  %-22s []\n", sq, "(null)", "(null)"); return; }
    char self[12]; if (bb->tmp >= 0) snprintf(self, sizeof self, "s%-4d", bb->tmp); else snprintf(self, sizeof self, "n%-4d", i);
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
    fprintf(fp, "%4s %-6s %4s %4s  %-22s [%s]", sq, self, gp, wp, opn, ob);
    if (verbose) fprintf(fp, " (n%d)", i);
    switch (bb->op) {
        case IR_LIT_INTEGER: fprintf(fp, " ival=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_LIT_REAL: fprintf(fp, " dval=%g", IR_LIT(bb).dval); break;
        case IR_LIT_STRING: fprintf(fp, " sval=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_LIT_CHARSET: fprintf(fp, " sval=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_VAR: fprintf(fp, " var=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_ASSIGN: fprintf(fp, " var=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_KEYWORD_ICON: case IR_KEYWORD_SNOBOL4: fprintf(fp, " kw=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        case IR_BINOP: case IR_BINOP_TEST: fprintf(fp, " binop=%lld", (long long)IR_LIT(bb).ival); break;
        case IR_SUCCEED: if (IR_LIT(bb).ival != 0) fprintf(fp, " stno=%d", (int)IR_LIT(bb).ival); break;
        case IR_CALL: case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
            fprintf(fp, " fn=\"%s\"", IR_LIT(bb).sval ? IR_LIT(bb).sval : ""); break;
        default: break;
    }
    fprintf(fp, "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_print(const IR_graph_t * bbg, FILE * fp) { bb_print_v(bbg, fp, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_print_v(const IR_graph_t * bbg, FILE * fp, int verbose) {
    if (!bbg) { fprintf(fp, "(null IR_graph_t)\n"); return; }
    char ent[12]; bb_ref_fmt(bbg, bbg->entry, ent, sizeof ent);
    fprintf(fp, "IR_graph_t n=%d entry=%s nslots=%d\n", bbg->n, ent, bbg->nslots);
    if (verbose)
        fprintf(fp, ";  seq self     γ    ω  kind                   [operands]  payload   (self/γ/ω/operands: sN=value slot N, nN=node id N "
                    "when no slot; linear emit order: γ-spine DFS from entry, then ω, then operands)\n");
    else         fprintf(fp, ";  seq self     γ    ω  kind                   [operands]  payload\n");
    int nn = bbg->n;
    char * vis = (char *) calloc(nn > 0 ? nn : 1, 1);
    int * order = (int *) malloc((size_t)(nn > 0 ? nn : 1) * sizeof(int));
    int norder = 0;
    if (vis && order) {
        bb_emit_order_visit(bbg, bbg->entry, vis, order, &norder);
        for (int sq = 0; sq < norder; sq++) bb_print_node_line(bbg, fp, sq, order[sq], verbose);
        int any_unreached = 0;
        for (int i = 0; i < nn; i++) if (!vis[i] && bbg->all[i]) { any_unreached = 1; break; }
        if (any_unreached) fprintf(fp, "; --- unreached (not on emit spine; shown for completeness) ---\n");
        if (any_unreached) for (int i = 0; i < nn; i++) if (!vis[i] && bbg->all[i]) bb_print_node_line(bbg, fp, -1, i, verbose);
    } else { for (int i = 0; i < nn; i++) bb_print_node_line(bbg, fp, i, i, verbose); }
    free(vis); free(order);
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
