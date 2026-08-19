#include "optimizer.h"
#include "branch_chain.h"
#include "const_fold.h"
#include "copy_prop.h"
#include "pat_fold.h"
#include "dead_pure.h"
#include "dead_goto.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-VAL-0 (s177) CONSTANT-FOLD GATE: cf_run is OFF by default on the main spine so that IR_LIT_INTEGER
 * nodes survive into the emitter as first-class fixed FORTH cells (per-BB sub-rsp, rsp-relative reads).
 * ON for proc graphs that CARRY A BODY (nparams > 0 or resumable_callable — the body STILL benefits from fold
 * because its lit operands are not yet on the FORTH cell ladder).  ⛔ CORRECTED s205 (MEASURED, not argued): this
 * arm NEVER FIRES FOR SNOBOL4 and that is CORRECT, not a defect.  A SNOBOL4 DEFINE lowers to the HOOK ONLY
 * (IR_DEFINE + an IR_CALL kind); under SN4-FLAT-PROC (s176) the function BODY is ordinary labelled
 * statements living in the ONE MAIN GRAPH, so a DEFINE contains nothing foldable and its body is governed by
 * the expression arm (OFF) by construction.  Measured with SCRIP_OPT_STATS: body-only and spine-only foldable
 * programs BOTH fold in graph #1; the stub graph folds 0.  The prior wording ("ALWAYS ON for DEFINE proc
 * graphs") is true only for Icon/Pascal/Prolog, whose proc graphs really do own their bodies — it sent an
 * s205 session chasing a phantom defect and nearly landed a VACUOUS one-line "fix" (copying nparams onto
 * sno_build_call_stub's graph, which has no body to fold: green build, zero movement, null misread as "folding
 * in DEFINEs buys nothing").  Do not re-open this without first re-measuring.  Pattern graphs stay ON (any IR_MATCH_* /
 * IR_PAT_* node present — pat_fold depends on cf_run reducing its inputs).  SCRIP_CF=1 re-enables globally
 * as a diagnostic escape hatch (mirrors the SCRIP_OPT=0 convention). */
/* ⭐⭐⭐ CN-14 -- THE FOLD IS ON BY DEFAULT ON THE MAIN SPINE (Lon in-chat 2026-08-19: "&Keywords as STATIC DESCR's in the ASM code, not a RUNTIME call ... FULL constant folding").  ZB-VAL-0 kept cf_run
 * OFF there so IR_LIT_INTEGER nodes would survive to the emitter as fixed FORTH cells -- but a SNOBOL4 main spine is EXACTLY where a declared &constant's reads land, so the tier that T1 exists to feed
 * was the one tier the folder never ran on: `&N = 5` · `&M = 3` · `OUTPUT = &N + &M` folded ZERO nodes and emitted a live rt_add@PLT.  MEASURED BEFORE FLIPPING, not argued: SNOBOL4 crosscheck FAIL sets
 * BYTE-IDENTICAL in both arms with DIVERGE=0 (307/10 m3, 306/10 m4), Icon crosscheck identical, Prolog's only mover (rung79_stream_permission) flakes in the BASELINE arm across repeated runs and is
 * therefore noise, not a CF effect.  The gate is READ ONCE into a function-local static -- the KW-5b lesson: a lazily-consulted switch whose answer can depend on call order is a defect generator. */
static int cf_spine_on(void) { static int s = -1; if (s < 0) { const char * e = getenv("SCRIP_CF"); s = (!e || *e != '0') ? 1 : 0; } return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_is_proc_or_pat(const IR_graph_t * g) {
    if (g->nparams > 0 || g->resumable_callable) return 1;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (!nd) continue; if (nd->op >= IR_MATCH_LIT && nd->op <= IR_MATCH_ADVANCE) return 1; if (nd->op == IR_PATTERN_CAT || nd->op == IR_PATTERN_ALT || nd->op == IR_PATTERN_DEFER) return 1; }
    return 0;
}
void optimizer_run(IR_graph_t *g) {
    { extern void region_report(IR_graph_t *); if (getenv("SCRIP_REGION_REPORT")) region_report(g); }
    if (!g) return;
    { extern void scc_taint_graph(IR_graph_t *); scc_taint_graph(g); }
    const char *e = getenv("SCRIP_OPT");
    if (e && *e == '0') return;
    int do_cf = cf_spine_on() || g_is_proc_or_pat(g);   /* ⭐⭐⭐ CN-14 DEFAULT FLIP: fold everywhere; SCRIP_CF=0 restores ZB-VAL-0's spine-OFF arm (proc/pat still ON, which is what makes it a real BASELINE) */
    int t_cf = 0, t_cp = 0, t_pf = 0, t_dp = 0, t_bc = 0;
    for (int round = 0; round < 8; round++) {
        int n_cf = do_cf ? cf_run(g) : 0, n_cp = cp_run(g), n_pf = pf_run(g), n_dp = dp_run(g);
        int b = bc_run(g);
        t_cf += n_cf; t_cp += n_cp; t_pf += n_pf; t_dp += n_dp; t_bc += b;
        if (!(n_cf + n_cp + n_pf + n_dp)) break;
    }
    int t_dg = 0;
    for (int round = 0; round < 8; round++) { int n = dg_run(g); t_dg += n; if (!n) break; }
    if (getenv("SCRIP_OPT_STATS")) fprintf(stderr, "[optimizer] fold=%d copy=%d pat=%d dead=%d branch_chain=%d dead_goto=%d\n", t_cf, t_cp, t_pf, t_dp, t_bc, t_dg);
}
