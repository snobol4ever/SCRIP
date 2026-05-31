/*====================================================================================================================================================================================================*/
/* lower_program.c — the regrown program-walker trunk. Holds lower(): walk the TT_PROGRAM, lower each top-level construct through the unified four-port lowerer, populate the stage2 BB table.       */
/* Also hosts binop_apply (the BB oracle's runtime value math) and lower_proc_gen (Icon generator box), the two helpers bb_exec.c and gen_runtime.c link against. These couple to the heavy runtime  */
/* (descr_to_str_icn, polyglot_init, stage2_reset), so they live HERE, not in lower.c, which must stay standalone-linkable for the topology proof harness (scripts/prove_lower2.sh links lower.o alone). */
/*====================================================================================================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "bb_program.h"
#include "../runtime/core/coerce.h"
#include "../runtime/interp/resolve_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
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
        case BINOP_POW: { if (!either_real && ri >= 0) { long b = li, e = ri, acc = 1; while (e-- > 0) acc *= b; return INTVAL(acc); }
                          double base = either_real ? ld : (double)li; double exp2 = either_real ? rd : (double)ri;
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
            size_t ll = ls_d.slen > 0 ? (size_t)ls_d.slen : strlen(ls);
            size_t rl = rs_d.slen > 0 ? (size_t)rs_d.slen : strlen(rs);
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
/* lower_pl_choice_graph — build an IR_CHOICE graph for a multi-clause predicate (PLG-3). Each clause in
 * the TT_CHOICE is lowered into its own GOAL graph; a bb_choice_state_t sidecar lists them so the IR_CHOICE
 * exec arm (bb_exec.c:3216) can try them in order, backtracking between clauses via trail_unwind.            */
static int lower_pl_clause_graph(const tree_t *clause);   /* forward — defined below */
static int lower_pl_choice_graph(const tree_t *choice) {
    if (!choice || choice->t != TT_CHOICE || choice->n < 1) return -1;
    int n = choice->n;
    IR_graph_t **bodies = (IR_graph_t **)GC_MALLOC((size_t)n * sizeof(IR_graph_t *));
    if (!bodies) return -1;
    int any = 0;
    for (int ci = 0; ci < n; ci++) {
        const tree_t *cl = choice->c[ci];
        int bidx = lower_pl_clause_graph(cl);
        bodies[ci] = (bidx >= 0) ? g_stage2.bbp.table[bidx] : NULL;
        if (bodies[ci]) any = 1;
    }
    if (!any) return -1;
    IR_graph_t *g = IR_alloc(8, IR_LANG_PL);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd = IR_node_alloc(g, IR_CHOICE);
    if (!nd) return -1;
    bb_choice_state_t *zc = (bb_choice_state_t *)GC_MALLOC(sizeof *zc);
    if (!zc) return -1;
    memset(zc, 0, sizeof *zc);
    zc->bodies = bodies; zc->nbodies = n; zc->last_body = NULL; zc->cp = NULL; zc->cut_barrier = NULL;
    zc->idx_ok = 0; zc->idx_key = NULL;
    nd->ival = (int64_t)(intptr_t)zc;
    nd->γ = PSUCC; nd->ω = PFAIL;
    g->entry = nd;
    return bb_program_add(&g_stage2.bbp, g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* lower_pl_register_all_preds — PLG-3: iterate resolve_pred_table, lower and register every predicate so
 * IR_GOAL's resolve_bb_lookup finds them. Called after lowering main/0. Skips already-registered entries. */
static void lower_pl_register_all_preds(void) {
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *key = pe->key;
            const tree_t *ch = pe->choice;
            const char *slash = key ? strrchr(key, '/') : NULL;
            int ar = slash ? atoi(slash + 1) : 0;
            /* IMPORTANT: bb_exec.c IR_GOAL looks up resolve_bb_lookup(key, arity) where key = "name/arity"
               (the FULL key, NOT the bare name). So we register under the full key string to match. */
            if (resolve_bb_lookup(key, ar)) continue;   /* already registered */
            int bb_idx = -1;
            if (ch->t == TT_CLAUSE) {
                bb_idx = lower_pl_clause_graph(ch);
            } else if (ch->t == TT_CHOICE) {
                if (ch->n == 1) bb_idx = lower_pl_clause_graph(ch->c[0]);
                else            bb_idx = lower_pl_choice_graph(ch);
            }
            if (bb_idx >= 0) resolve_bb_register(key, ar, bb_idx);
        }
    }
}
/* `clause` is a TT_CLAUSE; its body goals lower (via lower2_clause_body_entry) into a conjunction graph whose  */
/* success -> PSUCC and failure -> PFAIL. FAIL-LOUD: an unhandled goal sinks the whole clause (-1) so the       */
/* driver keeps its clean abort. Returns the bb_program index, or -1. Successor to lower_pl's clause walker.    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower2_clause_body_entry(IR_graph_t * bbg, const tree_t * clause, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
static int lower_pl_clause_graph(const tree_t *clause) {
    if (!clause || clause->t != TT_CLAUSE) return -1;
    IR_graph_t *g = IR_alloc(256, IR_LANG_PL);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *α = NULL, *β = NULL;
    IR_t *top = lower2_clause_body_entry(g, clause, PSUCC, PFAIL, &α, &β);
    if (!top || !α) return -1;
    g->entry = α;
    return bb_program_add(&g_stage2.bbp, g);
}
/* "main" so the driver's bb_exec_once(main) path can run it. SNOBOL4 only this increment: Icon proc-body and Prolog clause graph-building belong to their own concurrent sessions (FACT RULE).          */
/* Statements thread in reverse (each statement's gamma flows to the next; omega to PFAIL). An assignment (subj :eq repl, no :pat) becomes a synthesized TT_ASSIGN(subj, repl) lowered via the VALUE role. */
/*====================================================================================================================================================================================================*/
/* SNOBOL4 program-defined function helpers (SPITBOL ch.8).                                                                                                                                            */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *lp_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *c = (char *) GC_MALLOC(n);
    if (c) memcpy(c, s, n);
    return c;
}
/* Parse a DEFINE prototype "NAME(p1,p2,...)l1,l2,..." — the parameter list and the local list are both
   optional; whitespace is ignored. On success returns 1 with the function name in fname, the dummy-argument
   names in params[0..*np-1] and the local names in locals[0..*nl-1] (SPITBOL Manual ch.8 §8.1). */
static int sno_parse_define_proto(const char *proto, char fname[64],
                                  char params[STAGE2_FRAME_SLOT_MAX][64], int *np,
                                  char locals[STAGE2_FRAME_SLOT_MAX][64], int *nl) {
    *np = 0; *nl = 0; if (fname) fname[0] = '\0';
    if (!proto || !fname) return 0;
    const char *p = proto;
    while (*p && isspace((unsigned char) *p)) p++;
    int fi = 0;
    while (*p && *p != '(' && !isspace((unsigned char) *p) && fi < 63) fname[fi++] = *p++;
    fname[fi] = '\0';
    if (fi == 0) return 0;
    while (*p && isspace((unsigned char) *p)) p++;
    if (*p == '(') {                                   /* dummy-argument list */
        p++;
        while (*p && *p != ')') {
            while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
            if (!*p || *p == ')') break;
            int ci = 0; char buf[64];
            while (*p && *p != ',' && *p != ')' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
            buf[ci] = '\0';
            if (ci > 0 && *np < STAGE2_FRAME_SLOT_MAX) { memcpy(params[*np], buf, (size_t) ci + 1); (*np)++; }
            while (*p && isspace((unsigned char) *p)) p++;
        }
        if (*p == ')') p++;
    }
    while (*p) {                                       /* local list */
        while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
        if (!*p) break;
        int ci = 0; char buf[64];
        while (*p && *p != ',' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
        buf[ci] = '\0';
        if (ci > 0 && *nl < STAGE2_FRAME_SLOT_MAX) { memcpy(locals[*nl], buf, (size_t) ci + 1); (*nl)++; }
    }
    return 1;
}
/*====================================================================================================================================================================================================*/
stage2_t *lower(const tree_t *prog) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    stage2_reset();
    uint32_t mask = polyglot_lang_mask(prog);
    polyglot_init(&g_stage2, prog, mask);
    if (mask & (1u << LANG_SNO)) {
        IR_graph_t *g = IR_alloc(1024, IR_LANG_SNO);
        if (g) {
            IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
            IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
            /* Shared return targets for program-defined function bodies (SPITBOL ch.8): a `:(RETURN)` or bare
               RETURN statement transfers to RET (value = the function-named variable), `:(FRETURN)` to FRET
               (the call fails). They are reached only inside a function call (frame_depth>0); during the main
               program they are never targeted. NRETURN (return-by-name) is wired to RET as a placeholder. */
            IR_t *RET  = IR_node_alloc(g, IR_RETURN); RET->dval  = 1.0; RET->α = NULL; RET->ω = PFAIL;
            IR_t *FRET = IR_node_alloc(g, IR_RETURN); FRET->dval = 2.0; FRET->α = NULL; FRET->ω = PFAIL;
            /* PASS 1 — collect the SNOBOL4 statements in SOURCE order and give each a LANDING node (a
               pass-through IR_SUCCEED that returns its γ). A statement's label names its landing node, so a
               goto (forward OR backward) resolves to a landing that already exists. landing[i].γ is wired to
               statement i's lowered entry (or, for a subject-less statement, straight to its goto target). */
            const tree_t *stmts[1024]; IR_t *land[1024]; int ns = 0;
            for (int i = 0; i < prog->n && ns < 1024; i++) {
                const tree_t *s = prog->c[i];
                if (!s || s->t != TT_STMT) continue;
                if (lp_s_int(s, ":lang") != LANG_SNO) continue;
                stmts[ns] = s;
                land[ns]  = IR_node_alloc(g, IR_SUCCEED);
                ns++;
            }
            /* label -> landing-node resolver (case-sensitive — RULES.md). END's landing falls to PSUCC. */
            int built = 0;
            for (int i = 0; i < ns; i++) {
                const tree_t *s = stmts[i];
                IR_t *fall = (i + 1 < ns) ? land[i + 1] : PSUCC;   /* default sequential successor */
                /* resolve the three goto targets to landing nodes (static label form: goto_node_str). A
                   computed/indirect goto (goto_node_expr) is not yet wired — falls through (documented). */
                IR_t *tgt_u = NULL, *tgt_s = NULL, *tgt_f = NULL;
                const char *gu = goto_node_str(stmt_goto_find(s, TT_GOTO_U));
                const char *gs = goto_node_str(stmt_goto_find(s, TT_GOTO_S));
                const char *gf = goto_node_str(stmt_goto_find(s, TT_GOTO_F));
                for (int j = 0; j < ns; j++) {
                    const char *lj = stmt_attr_str(stmt_attr_find(stmts[j], ":lbl"));
                    if (!lj) continue;
                    if (gu && !strcmp(gu, lj)) tgt_u = land[j];
                    if (gs && !strcmp(gs, lj)) tgt_s = land[j];
                    if (gf && !strcmp(gf, lj)) tgt_f = land[j];
                }
                if (gu && !tgt_u && !strcmp(gu, "END")) tgt_u = PSUCC;
                if (gs && !tgt_s && !strcmp(gs, "END")) tgt_s = PSUCC;
                if (gf && !tgt_f && !strcmp(gf, "END")) tgt_f = PSUCC;
                /* program-defined-function exits (SPITBOL ch.8): RETURN -> RET, FRETURN -> FRET, NRETURN -> RET (placeholder). */
                if (gu && !tgt_u) { if (!strcmp(gu, "RETURN") || !strcmp(gu, "NRETURN")) tgt_u = RET; else if (!strcmp(gu, "FRETURN")) tgt_u = FRET; }
                if (gs && !tgt_s) { if (!strcmp(gs, "RETURN") || !strcmp(gs, "NRETURN")) tgt_s = RET; else if (!strcmp(gs, "FRETURN")) tgt_s = FRET; }
                if (gf && !tgt_f) { if (!strcmp(gf, "RETURN") || !strcmp(gf, "NRETURN")) tgt_f = RET; else if (!strcmp(gf, "FRETURN")) tgt_f = FRET; }
                /* SPITBOL ch.4 goto precedence: an unconditional `:(L)` overrides S/F; otherwise `:S(L)` is the
                   success exit and `:F(L)` the failure exit; an unspecified exit falls through sequentially. */
                IR_t *γ_tgt = tgt_u ? tgt_u : (tgt_s ? tgt_s : fall);
                IR_t *ω_tgt = tgt_u ? tgt_u : (tgt_f ? tgt_f : fall);
                tree_t *subj = lp_s_expr(s, ":subj");
                if (!subj) {
                    /* subject-less statement: a bare goto (`:(L)` / `L :(M)`) or the END line. Its landing just
                       transfers to the unconditional target (or falls through). No expr to lower. */
                    land[i]->γ = tgt_u ? tgt_u : fall;
                    built = 1;
                    continue;
                }
                /* A bare RETURN / FRETURN / NRETURN statement (subject is the keyword, no goto) returns from the
                   enclosing program-defined function (SPITBOL ch.8): transfer the landing straight to RET/FRET. */
                if (subj->t == TT_VAR && subj->v.sval &&
                    (!strcmp(subj->v.sval, "RETURN") || !strcmp(subj->v.sval, "FRETURN") || !strcmp(subj->v.sval, "NRETURN"))) {
                    land[i]->γ = (!strcmp(subj->v.sval, "FRETURN")) ? FRET : RET;
                    built = 1;
                    continue;
                }
                tree_t *expr = subj;
                if (stmt_attr_find(s, ":eq")) {
                    if (stmt_attr_find(s, ":pat")) { land[i]->γ = fall; continue; }
                    tree_t *repl = lp_s_expr(s, ":repl");
                    if (!repl) { land[i]->γ = fall; continue; }
                    if (subj->t == TT_SCAN && subj->n >= 2) {
                        tree_t *scn = ast_stmt_new(TT_SCAN);
                        if (!scn) { land[i]->γ = fall; continue; }
                        ast_push(scn, subj->c[0]);
                        ast_push(scn, subj->c[1]);
                        ast_push(scn, repl);
                        expr = scn;
                    } else {
                        tree_t *asn = ast_stmt_new(TT_ASSIGN);
                        if (!asn) { land[i]->γ = fall; continue; }
                        ast_push(asn, subj);
                        ast_push(asn, repl);
                        expr = asn;
                    }
                }
                IR_t *α = NULL, *β = NULL;
                /* SNOBOL4 control flow (SPITBOL ch.5): success exits to γ_tgt, failure to ω_tgt — both default
                   to the next statement when no explicit goto applies. */
                IR_t *top = lower2_value_entry(g, expr, γ_tgt, ω_tgt, &α, &β);
                if (!top || !α) { land[i]->γ = fall; continue; }
                land[i]->γ = α;       /* landing falls through into the statement's lowered entry */
                built = 1;
            }
            if (built) {
                g->entry = (ns > 0) ? land[0] : PSUCC;
                (void) PFAIL;
                int bb_idx = bb_program_add(&g_stage2.bbp, g);
                int pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pi].name     = "main";
                g_stage2.proc_table[pi].proc     = NULL;
                g_stage2.proc_table[pi].entry_pc = -1;
                g_stage2.proc_table[pi].bb_idx   = bb_idx;
                g_stage2.proc_table[pi].nparams  = 0;
                /* SNOBOL4 program-defined functions (SPITBOL ch.8): for each DEFINE('proto') register a proc
                   whose graph is a VIEW over g (shared node set; entry = the landing of the body label NAME).
                   The call (IR_CALL dval==2.0 in bb_exec) saves the globals named by params+locals+NAME, binds
                   the dummy args, runs the body to RETURN/FRETURN, then restores them and yields NAME's value.
                   lower_sc carries the saved-name list (params, then locals, then NAME); nparams = #params. */
                for (int di = 0; di < ns; di++) {
                    tree_t *dsubj = lp_s_expr(stmts[di], ":subj");
                    if (!dsubj || dsubj->t != TT_FNC || !dsubj->v.sval || strcmp(dsubj->v.sval, "DEFINE")) continue;
                    if (dsubj->n < 1 || !dsubj->c[0] || dsubj->c[0]->t != TT_QLIT || !dsubj->c[0]->v.sval) continue;
                    char fname[64];
                    char params[STAGE2_FRAME_SLOT_MAX][64]; int np = 0;
                    char locals[STAGE2_FRAME_SLOT_MAX][64]; int nl = 0;
                    if (!sno_parse_define_proto(dsubj->c[0]->v.sval, fname, params, &np, locals, &nl)) continue;
                    IR_t *body = NULL;
                    for (int j = 0; j < ns; j++) {
                        const char *lj = stmt_attr_str(stmt_attr_find(stmts[j], ":lbl"));
                        if (lj && !strcmp(lj, fname)) { body = land[j]; break; }
                    }
                    if (!body) continue;                       /* no body label -> not lowerable here (documented) */
                    IR_graph_t *fg = (IR_graph_t *) calloc(1, sizeof(IR_graph_t));
                    if (!fg) continue;
                    *fg = *g;                                  /* view: shares all/n/max; own AG ring; distinct entry */
                    fg->entry = body;
                    int fidx = bb_program_add(&g_stage2.bbp, fg);
                    int fpi  = stage2_proc_grow(&g_stage2);
                    g_stage2.proc_table[fpi].name     = lp_strdup(fname);
                    g_stage2.proc_table[fpi].proc     = NULL;
                    g_stage2.proc_table[fpi].entry_pc = -1;
                    g_stage2.proc_table[fpi].bb_idx   = fidx;
                    g_stage2.proc_table[fpi].nparams  = np;
                    Scope *sc = &g_stage2.proc_table[fpi].lower_sc;
                    sc->n = 0;
                    for (int k = 0; k < np && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = lp_strdup(params[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
                    for (int k = 0; k < nl && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = lp_strdup(locals[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
                    if (sc->n < STAGE2_FRAME_SLOT_MAX) { sc->e[sc->n].name = lp_strdup(fname); sc->e[sc->n].slot = sc->n; sc->n++; }
                }
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
    if (mask & (1u << LANG_PL)) {
        /* PLG-1 — Prolog crosses onto Byrd Boxes (hello-world tier). polyglot_init already inserted every
           clause subject (TT_CHOICE/TT_CLAUSE) into s2->resolve_pred_table keyed by "name/arity". The
           `:- initialization(G, main).` directive names the goal predicate G to run; for PLG-1 (single
           non-recursive predicate, no clause choice) we resolve G's clause, lower its body into one GOAL
           graph, and register it as proc "main" so the driver's bb_exec_once(main) path runs it. Multi-clause
           dispatch (BB_CHOICE), user-predicate calls, recursion, and head unification are the later PLG rungs.
           The directive's goal atom is read from the TT_FNC initialization subject; absent it, default main/0. */
        const char *goal_key = NULL;
        char keybuf[128];
        for (int i = 0; i < prog->n; i++) {
            const tree_t *s = prog->c[i];
            if (!s || s->t != TT_STMT) continue;
            if (lp_s_int(s, ":lang") != LANG_PL) continue;
            const tree_t *subj = lp_s_expr(s, ":subj");
            if (!subj) continue;
            if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
                const tree_t *gt = subj->c[0];
                if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME) && gt->v.sval) {
                    snprintf(keybuf, sizeof keybuf, "%s/0", gt->v.sval);
                    goal_key = keybuf;
                } else if (gt && gt->t == TT_FNC && gt->v.sval) {
                    snprintf(keybuf, sizeof keybuf, "%s/%d", gt->v.sval, gt->n);
                    goal_key = keybuf;
                }
            }
        }
        if (!goal_key) goal_key = "main/0";
        const tree_t *choice = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, goal_key);
        const tree_t *clause = NULL;
        if (choice) {
            if (choice->t == TT_CLAUSE) clause = choice;
            else if (choice->t == TT_CHOICE && choice->n >= 1) clause = choice->c[0];
        }
        if (clause) {
            int bb_idx = lower_pl_clause_graph(clause);
            if (bb_idx >= 0) {
                int pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pi].name     = "main";
                g_stage2.proc_table[pi].proc     = NULL;
                g_stage2.proc_table[pi].entry_pc = -1;
                g_stage2.proc_table[pi].bb_idx   = bb_idx;
                g_stage2.proc_table[pi].nparams  = 0;
            }
        }
        /* PLG-3: register all remaining predicates so IR_GOAL's resolve_bb_lookup finds callee graphs. */
        lower_pl_register_all_preds();
        g_stage2.lang = IR_LANG_PL;
        return &g_stage2;
    }
    g_stage2.lang = IR_LANG_SNO;
    return &g_stage2;
}
/*====================================================================================================================================================================================================*/
