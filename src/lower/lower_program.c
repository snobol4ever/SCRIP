/*====================================================================================================================================================================================================*/
/* lower_program.c — the regrown program-walker trunk. Holds lower(): walk the TT_PROGRAM, lower each top-level construct through the unified four-port lowerer, populate the stage2 BB table.       */
/* Also hosts binop_apply (the BB oracle's runtime value math) and lower_proc_gen (Icon generator box), the two helpers bb_exec.c and gen_runtime.c link against. These couple to the heavy runtime  */
/* (descr_to_str_icn, polyglot_init, stage2_reset), so they live HERE, not in lower.c, which must stay standalone-linkable for the topology proof harness (scripts/prove_lower2.sh links lower.o alone). */
/*====================================================================================================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "bb_program.h"
#include "../runtime/core/coerce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <gc/gc.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern uint32_t polyglot_lang_mask(const tree_t * prog);
extern void polyglot_init(stage2_t * s2, const tree_t * prog, uint32_t lang_mask);
extern IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================================================================================================*/
/* binop_apply — apply a binary operator to two runtime descriptors. Numeric ops promote to real when either side is real; relationals return the right value or fail (rel_fail set); concat and the   */
/* string-relationals coerce via descr_to_str_icn. Ported verbatim from the pre-cut lower.c (blob d2d8c8e1); bb_exec.c calls it from every BINOP execution arm, hence the executable needs the symbol.  */
/*====================================================================================================================================================================================================*/
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail) {
    *rel_fail = 0;
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    int either_real = (IS_REAL_fn(lv) || IS_REAL_fn(rv));
    double ld = IS_REAL_fn(lv) ? lv.r : (double)(IS_INT_fn(lv) ? lv.i : 0);
    double rd = IS_REAL_fn(rv) ? rv.r : (double)(IS_INT_fn(rv) ? rv.i : 0);
    long   li = IS_INT_fn(lv) ? lv.i : (long)lv.r;
    long   ri = IS_INT_fn(rv) ? rv.i : (long)rv.r;
    DESCR_t real_result;
    switch (op) {
        case BINOP_ADD: if (either_real) { real_result.v=DT_R; real_result.r=ld+rd; return real_result; } return INTVAL(li + ri);
        case BINOP_SUB: if (either_real) { real_result.v=DT_R; real_result.r=ld-rd; return real_result; } return INTVAL(li - ri);
        case BINOP_MUL: if (either_real) { real_result.v=DT_R; real_result.r=ld*rd; return real_result; } return INTVAL(li * ri);
        case BINOP_DIV: if (either_real) { if (rd == 0.0) return FAILDESCR; real_result.v=DT_R; real_result.r=ld/rd; return real_result; } return ri ? INTVAL(li / ri) : FAILDESCR;
        case BINOP_MOD: return ri ? INTVAL(li % ri) : FAILDESCR;
        case BINOP_POW: { double base = either_real ? ld : (double)li; double exp2 = either_real ? rd : (double)ri;
                          real_result.v = DT_R; real_result.r = pow(base, exp2); return real_result; }
        case BINOP_LT: *rel_fail = !(either_real ? ld <  rd : li <  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_LE: *rel_fail = !(either_real ? ld <= rd : li <= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GT: *rel_fail = !(either_real ? ld >  rd : li >  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GE: *rel_fail = !(either_real ? ld >= rd : li >= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_EQ: *rel_fail = !(either_real ? ld == rd : li == ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_NE: *rel_fail = !(either_real ? ld != rd : li != ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_CONCAT: {
            DESCR_t ls_d; ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d; rs_d = descr_to_str_icn(rv);
            if (IS_FAIL_fn(ls_d) || IS_FAIL_fn(rs_d)) return FAILDESCR;
            const char *ls = ls_d.s ? ls_d.s : "";
            const char *rs = rs_d.s ? rs_d.s : "";
            size_t ll = GET_SLEN(ls_d) > 0 ? (size_t)GET_SLEN(ls_d) : strlen(ls);
            size_t rl = GET_SLEN(rs_d) > 0 ? (size_t)GET_SLEN(rs_d) : strlen(rs);
            char *buf = GC_malloc(ll + rl + 1);
            memcpy(buf, ls, ll); memcpy(buf + ll, rs, rl); buf[ll + rl] = '\0';
            { DESCR_t r2; r2.v = DT_S; r2.slen = (int)(ll + rl); r2.s = buf; return r2; }
        }
        case BINOP_SLT: case BINOP_SLE: case BINOP_SGT:
        case BINOP_SGE: case BINOP_SEQ: case BINOP_SNE: {
            DESCR_t ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d = descr_to_str_icn(rv);
            const char *ls = (!IS_FAIL_fn(ls_d) && ls_d.s) ? ls_d.s : "";
            const char *rs = (!IS_FAIL_fn(rs_d) && rs_d.s) ? rs_d.s : "";
            int cmp = strcmp(ls, rs);
            int ok;
            switch (op) {
            case BINOP_SLT: ok = (cmp <  0); break;
            case BINOP_SLE: ok = (cmp <= 0); break;
            case BINOP_SGT: ok = (cmp >  0); break;
            case BINOP_SGE: ok = (cmp >= 0); break;
            case BINOP_SEQ: ok = (cmp == 0); break;
            case BINOP_SNE: ok = (cmp != 0); break;
            default:            ok = 0;           break;
            }
            *rel_fail = !ok;
            return ok ? rv : FAILDESCR;
        }
        default: return FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* lower_proc_gen — wrap a live GeneratorState pointer in an IR_PROC_GEN box so the BB engine can resume an Icon generator. Ported verbatim from the pre-cut lower.c; gen_runtime.c is the sole caller. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_gen(struct GeneratorState *gs) {
    if (!gs) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_PROC_GEN);
    if (!bb) return NULL;
    bb->counter = (int64_t)(uintptr_t)gs;
    bb->α      = bb;
    bb->β      = bb;
    bb->γ      = NULL;
    bb->ω      = NULL;
    bbg->entry = bb;
    return bbg;
}
/*====================================================================================================================================================================================================*/
/* stmt-attr helpers — mirror polyglot.c's static s_int/s_expr using the exported stmt_attr_* accessors (the polyglot.c statics are not visible across translation units).                              */
/*====================================================================================================================================================================================================*/
static int lp_s_int(const tree_t *s, const char *tag) { const char *v = stmt_attr_str(stmt_attr_find(s, tag)); return v ? atoi(v) : 0; }
static tree_t *lp_s_expr(const tree_t *s, const char *tag) { return stmt_attr_expr(stmt_attr_find(s, tag)); }
/*====================================================================================================================================================================================================*/
/* lower_icon_body — build ONE Icon proc's four-port BB graph. The proc decl is TT_PROC_DECL(name, params,  */
/* body) with body = c[2], a TT_PROGRAM of statements. Each statement's expr is lowered VALUE-role and       */
/* threaded in REVERSE (stmt[i].γ -> stmt[i+1].α, stmt[i].ω -> PFAIL) into one graph — the same reverse-      */
/* threading the SNOBOL4 walker uses. FAIL-LOUD: if ANY statement fails to lower (an unhandled kind), the     */
/* whole body returns -1 so the driver keeps its clean [IBB] FATAL abort rather than running a partial graph  */
/* with a silently-dropped statement. Returns the bb_program index, or -1.                                    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
static int lower_icon_body(const tree_t *proc) {
    if (!proc || proc->t != TT_PROC_DECL || proc->n < 3) return -1;
    const tree_t *body = proc->c[2];
    if (!body || body->t != TT_PROGRAM) return -1;
    IR_graph_t *g = IR_alloc(256, IR_LANG_ICN);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *next_a = PSUCC;
    int n_stmts = 0;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t *s = body->c[i];
        if (!s) continue;
        const tree_t *expr = s;
        if (s->t == TT_STMT) { expr = lp_s_expr(s, ":subj"); if (!expr) continue; }
        n_stmts++;
        IR_t *a = NULL, *b = NULL;
        IR_t *top = lower2_value_entry(g, (const tree_t *) expr, next_a, PFAIL, &a, &b);
        if (!top || !a) return -1;       /* fail-loud: an unhandled statement sinks the whole body */
        next_a = a;
    }
    if (n_stmts == 0) return -1;
    g->entry = next_a;
    return bb_program_add(&g_stage2.bbp, g);
}
/*====================================================================================================================================================================================================*/
/* lower — minimal program walker. Resets stage2, runs polyglot_init (label/proc/clause prescan), then for SNOBOL4 threads the top-level statements into ONE four-port BB graph and registers it as     */
/* "main" so the driver's bb_exec_once(main) path can run it. SNOBOL4 only this increment: Icon proc-body and Prolog clause graph-building belong to their own concurrent sessions (FACT RULE).          */
/* Statements thread in reverse (each statement's gamma flows to the next; omega to PFAIL). An assignment (subj :eq repl, no :pat) becomes a synthesized TT_ASSIGN(subj, repl) lowered via the VALUE role. */
/*====================================================================================================================================================================================================*/
stage2_t *lower(const tree_t *prog) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    stage2_reset();
    uint32_t mask = polyglot_lang_mask(prog);
    polyglot_init(&g_stage2, prog, mask);
    if (mask & (1u << LANG_SNO)) {
        IR_graph_t *g = IR_alloc(256, IR_LANG_SNO);
        if (g) {
            IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
            IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
            IR_t *next_α = PSUCC;
            int built = 0;
            for (int i = prog->n - 1; i >= 0; i--) {
                const tree_t *s = prog->c[i];
                if (!s || s->t != TT_STMT) continue;
                if (lp_s_int(s, ":lang") != LANG_SNO) continue;
                tree_t *subj = lp_s_expr(s, ":subj");
                if (!subj) continue;
                tree_t *expr = subj;
                if (stmt_attr_find(s, ":eq")) {
                    if (stmt_attr_find(s, ":pat")) continue;
                    tree_t *repl = lp_s_expr(s, ":repl");
                    if (!repl) continue;
                    tree_t *asn = ast_stmt_new(TT_ASSIGN);
                    if (!asn) continue;
                    ast_push(asn, subj);
                    ast_push(asn, repl);
                    expr = asn;
                }
                IR_t *α = NULL, *β = NULL;
                IR_t *top = lower2_value_entry(g, expr, next_α, PFAIL, &α, &β);
                if (!top || !α) continue;
                next_α = α;
                built = 1;
            }
            if (built) {
                g->entry = next_α;
                int bb_idx = bb_program_add(&g_stage2.bbp, g);
                int pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pi].name     = "main";
                g_stage2.proc_table[pi].proc     = NULL;
                g_stage2.proc_table[pi].entry_pc = -1;
                g_stage2.proc_table[pi].bb_idx   = bb_idx;
                g_stage2.proc_table[pi].nparams  = 0;
            }
        }
    }
    if (mask & (1u << LANG_ICN)) {
        /* Icon procs were registered by polyglot_init with proc -> TT_PROC_DECL and bb_idx = -1. Lower each
           body into a four-port graph and fill bb_idx; the driver runs bb_exec_once(main). A proc whose body
           has an unhandled statement keeps bb_idx = -1 (lower_icon_body fails loud) -> driver aborts cleanly. */
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_PROC_DECL) continue;
            if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
            int bb_idx = lower_icon_body(proc);
            if (bb_idx >= 0) g_stage2.proc_table[pi].bb_idx = bb_idx;
        }
        g_stage2.lang = IR_LANG_ICN;
        return &g_stage2;
    }
    g_stage2.lang = IR_LANG_SNO;
    return &g_stage2;
}
/*====================================================================================================================================================================================================*/
