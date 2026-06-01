#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <gc.h>
#include "../frontend/snobol4/scrip_cc.h"
#include "../frontend/snocone/snocone_driver.h"
#include "../frontend/prolog/prolog_driver.h"
#include "../frontend/prolog/term.h"
#include "../frontend/prolog/prolog_runtime.h"
#include "../frontend/prolog/prolog_atom.h"
#include "../frontend/prolog/prolog_builtin.h"
#include "../frontend/icon/icon_driver.h"
#include "../frontend/raku/raku_driver.h"
#include "../frontend/rebus/rebus_lower.h"
#include "../runtime/interp/gen.h"
#include "../frontend/icon/icon_lex.h"
#include "../include/bb_box.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_set_print_width(int w);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "bb_build.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "scrip_sm.h"
#include "sync_monitor.h"
extern DESCR_t pat_at_cursor(const char *varname);
static void stmt_init(void) {}
extern DESCR_t      eval_expr(const char *src);
extern const char  *exec_code(DESCR_t code_block);
extern int exec_stmt(const char *subj_name,
                          DESCR_t    *subj_var,
                          DESCR_t     pat,
                          DESCR_t    *repl,
                          int         has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
#include "../runtime/interp/gen_runtime.h"
#include "../runtime/interp/resolve_runtime.h"
#include "driver/polyglot.h"
#include "../tools/emit_per_kind_audit.h"
/*====================================================================================================================================================================================================*/
/* ICON MODE-3 RING->TREE ADAPTER (Path 1-lite, Icon --run only). The unified lower2 emits a postfix      */
/* gamma-chain for Icon expressions: operands precede their operator in gamma-order, every node has       */
/* alpha=beta=NULL, and operands are read from the AG ring at exec time (the mode-2 oracle's model). The  */
/* mode-3 flat emitter (emit_bb.c walk_bb_flat / flat_drive_*) and the GROUND-ZERO templates expect the   */
/* OLD tree-shape (operands in alpha/beta children, as the deleted lower_icn.c blob d2d8c8e1 built them:  */
/* bb->alpha=lhs; bb->beta=rhs / bb->alpha=args[0]). This adapter un-flattens the straight-line chain     */
/* into that tree by postfix evaluation (each kind's operand arity drives how many preceding nodes become */
/* its children), relinks alpha/beta, and returns the root for bb_build_flat. Fails SOFT (returns NULL)   */
/* on any shape outside the GZ-1/2/3 straight-line subset so the caller falls back to the prior behavior  */
/* (no regression for control-flow rungs not yet rebuilt). Postfix order verified empirically via         */
/* --dump-bb (write(2+3): LIT_I 2 -> LIT_I 3 -> IR_BINOP -> IR_CALL). lower.c and the templates are        */
/* UNTOUCHED; this is a mode-3 Icon emitter-input adapter, not a lowerer change.                          */
/*====================================================================================================================================================================================================*/
static int icn_rt_arity(const IR_t *n) {
    switch (n->t) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    case IR_BINOP: case IR_BINOP_GEN: return 2;
    case IR_TO:    case IR_TO_BY:     return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_EVERY: return 1;
    case IR_CALL:  return (int)n->ival;
    default:       return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_ring_to_tree(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_t *chain[256]; int nc = 0;
    for (IR_t *cur = g->entry; cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && nc < 256; cur = cur->γ) chain[nc++] = cur;
    if (nc == 0 || nc >= 256) return NULL;
    IR_t *stk[256]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = icn_rt_arity(n);
        if (ar < 0 || ar > sp) return NULL;
        if (n->t == IR_CALL) {
            if (ar != 1) return NULL;
            n->α = stk[sp - 1]; sp -= 1;
            n->dval = 0.0;
        } else if (ar == 2) {
            n->β = stk[sp - 1];
            n->α = stk[sp - 2];
            sp -= 2;
        } else if (ar == 1) {
            n->α = stk[sp - 1]; sp -= 1;
        }
        stk[sp++] = n;
    }
    if (sp != 1) return NULL;
    return stk[0];
}
/*====================================================================================================================================================================================================*/
/* sno_ring_to_tree REMOVED (Lon directive, 2026-05-31 — VIOLATION). The postfix AG-ring -> four-port-tree */
/* un-flattening adapter was a STOPGAP, never the design: it re-derived the four-port BB topology AT EMIT  */
/* time from the mode-2 oracle's postfix gamma-ring instead of LOWER producing that topology directly. The */
/* correct path (this goal's banner + LM-6 DISPATCH-UNIFY) is that LOWER emits each SNOBOL4 statement       */
/* DIRECTLY into the test_sno_1.c four-port statement-BB graph (subject-BB -> pattern-BBs -> replacement-BB */
/* -> substitution-BB), so the emitter consumes it with NO driver adapter and modes 3 and 4 light up from   */
/* the SAME graph + SAME per-box templates (two arms). Both call sites below now ABORT until LOWER does     */
/* that. NO storage outside the boxes; each BB owns its own RO + RW local allocation (see GOAL FACT RULE).  */
/*====================================================================================================================================================================================================*/
/* PROLOG MODE-3 NATIVE FLAT-WALK ROOT RECOGNIZER (PLG-8-native, 2026-05-31). The interim mode-3 route (PLG-8) ran Prolog --run through bb_exec_once (the mode-2 interpreter + the AG ring on IR_graph_t) */
/* — correct output, but that is the RING path, which is the mode-2 idiom only (Lon directive: rings are for mode-2 interp; WRONG for mode-3). Mode-3 must EMIT code+data INSIDE the boxes with values    */
/* flowing UP the BB graph chain via per-box slots, exactly as test_sno_1.c / test_icon.c do (POS0/BIRD/mult_V live in the box and the consumer reads the producer's slot — no ring, no value stack). This  */
/* recognizer returns the flat-walk ROOT (the principal IR_GCONJ in the main graph) for the shapes whose BB templates already emit a correct stackless box with NO ring traffic — today the ground          */
/* hello-world tier: a single-clause body that is a conjunction of constant-arg builtins (write/writeln/print/nl/halt) and/or IR_SUCCEED/IR_CUT, with NO logic-variable slots (nslots==0) and NO             */
/* user-proc call / choice / disjunction / ite / unify / arith goal. For every richer shape it returns NULL and the driver keeps the proven interim bb_exec_once route (zero regression) — the same          */
/* widen-as-you-go gate sno_ring_to_tree uses for SNOBOL4. The GCONJ carries its goal entries in the bb_conj_state_t sidecar (node->ival); bb_builtin reads each arg from the goal's own alpha at emit time   */
/* (write of an IR_ATOM -> mov rdi, imm64(atom); call rt_pl_write_atom), so the value is a sealed read-only constant read directly by its consumer — no rt_pl_atom_push ring push. Widens rung by rung.       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_arith_leaf_simple(const IR_t *o) {
    /* PLG-9c: an arith RHS leaf operand the integer rt_pl_is/rt_pl_arith path can evaluate from        */
    /* serialized scalars: a literal int, or a logic-variable slot (rt_pl_arith reads g_resolve_env[li] */
    /* and derefs to its int). NOT a float literal (rt_pl_is is integer-only — term_new_int), NOT a     */
    /* nested IR_ARITH (the MEDIUM_TEXT is arm flattens only ONE level of operands into lk/li,rk/ri).   */
    if (!o) return 0;
    return o->t == IR_LIT_I || o->t == IR_LOGICVAR;
}
static int pl_flat_goal_is_simple(const IR_t *g) {
    if (!g) return 0;
    switch (g->t) {
    case IR_SUCCEED: case IR_CUT: case IR_ATOM: return 1;
    case IR_BUILTIN: {
        const char *fn = g->sval ? g->sval : "";
        /* PLG-9c (2026-05-31): `Var is Expr` — integer arithmetic into a logic-variable slot. The      */
        /* bb_builtin MEDIUM_TEXT is arm (line ~1434) flattens the RHS at emit time into serialized      */
        /* scalars (dst_slot, op via _.bb_op_lbl, lk/li, rk/ri) and calls rt_pl_is, which evaluates via  */
        /* rt_pl_arith and unifies the int result into g_resolve_env[dst_slot] — NO cross-process IR_t*  */
        /* pointer (that is the MEDIUM_BINARY twin's mode-3-only path). Admit only the shapes that arm    */
        /* handles: α = IR_LOGICVAR, β = IR_ARITH binary (both operands) or unary (left only), each       */
        /* operand a scalar int/slot. Float RHS, nested arith, or a non-slot LHS decline (-> EXCISED).   */
        if (!strcmp(fn, "is")) {
            const IR_t *lhs = g->α, *rhs = g->β;
            if (!lhs || lhs->t != IR_LOGICVAR || !rhs || rhs->t != IR_ARITH) return 0;
            if (rhs->α && rhs->β) return pl_flat_arith_leaf_simple(rhs->α) && pl_flat_arith_leaf_simple(rhs->β);
            if (rhs->α && !rhs->β) return pl_flat_arith_leaf_simple(rhs->α);   /* unary op(L) */
            return 0;
        }
        int is_io = (!strcmp(fn, "write") || !strcmp(fn, "writeln") || !strcmp(fn, "print") || !strcmp(fn, "nl") || !strcmp(fn, "halt"));
        if (!is_io) return 0;
        /* PLG-9b (2026-05-31): write/print of a logic-variable slot is now in the flat tier. The bb_builtin */
        /* emit_write_term TEXT arm already renders IR_LOGICVAR via rt_pl_write_var(slot) (reads g_resolve_  */
        /* env[slot]); its MEDIUM_BINARY twin does the same. So accept an IR_LOGICVAR arg alongside the      */
        /* constant IR_ATOM / IR_LIT_I args of PLG-9a.                                                       */
        if (g->ival >= 1) { const IR_t *a = g->α; if (!a || (a->t != IR_ATOM && a->t != IR_LIT_I && a->t != IR_LOGICVAR)) return 0; }
        return 1;
    }
    case IR_UNIFY: {
        /* PLG-9b (2026-05-31): the X = world tier — one logic-variable slot bound to a constant. The      */
        /* bb_unify TEXT/BINARY arms build each operand via rt_pl_node_to_term then call rt_pl_unify_terms, */
        /* which writes the binding into g_resolve_env[slot] under a trail mark. Accept only the proven     */
        /* (LOGICVAR = ATOM|LIT_I) and the symmetric (ATOM|LIT_I = LOGICVAR) scalar shapes; a compound or   */
        /* var=var operand routes through paths PLG-9b does not yet prove, so it declines (-> NULL root ->  */
        /* EXCISED, no regression). The per-activation slot lives in g_resolve_env, allocated by the driver */
        /* before the flat walk (mode-3) / by rt_pl_env_alloc in the emitted main: wrapper (mode-4).        */
        const IR_t *l = g->α, *r = g->β;
        if (!l || !r) return 1;                                   /* vacuous-success unify (matches bb_exec.c F-6d) */
        int l_var = (l->t == IR_LOGICVAR), r_var = (r->t == IR_LOGICVAR);
        int l_con = (l->t == IR_ATOM || l->t == IR_LIT_I);
        int r_con = (r->t == IR_ATOM || r->t == IR_LIT_I);
        return (l_var && r_con) || (l_con && r_var);
    }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_flat_body_root(IR_graph_t *g) {
    /* PLG-9b (2026-05-31): the nslots==0 guard is lifted. A body now qualifies for the native flat walk    */
    /* even with logic-variable slots (nslots>0), PROVIDED every conjunction element passes                 */
    /* pl_flat_goal_is_simple — which only admits the proven unify-(var=const) + write/print(var) + nl tier. */
    /* The slots live in g_resolve_env (per-activation env), set up before the walk by the driver (mode-3)   */
    /* or by an rt_pl_env_alloc(nslots) call emitted into the main: wrapper (mode-4). A richer slot-bearing  */
    /* shape (user call, choice, compound unify) has an element that fails the simple check -> NULL -> the   */
    /* interim route (mode-3) / EXCISED (mode-4), so widening stays safe and incremental.                    */
    if (!g || !g->all) return NULL;
    IR_t *gconj = NULL;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (nd && nd->t == IR_GCONJ) { if (gconj) return NULL; gconj = nd; }
    }
    if (!gconj) {
        if (g->nslots > 0) return NULL;                /* bare non-conj body with slots: not the proven tier */
        return (g->entry && g->entry->t == IR_SUCCEED) ? g->entry : NULL;
    }
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)gconj->ival;
    if (!zs || !zs->goals || zs->ngoals <= 0) return NULL;
    for (int i = 0; i < zs->ngoals; i++) if (!pl_flat_goal_is_simple(zs->goals[i])) return NULL;
    return gconj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PROLOG MODE-3/4 RICH-BODY RECOGNIZER (PLG-9d, 2026-06-01). The flat-tier recognizer above admits only a   */
/* single-clause body with no user calls / choice / disjunction. PLG-9d crosses the next rung: facts +       */
/* clause choice + deterministic + backtracking user-predicate calls. The emit machinery is already shared   */
/* and present — walk_bb_flat dispatches IR_GOAL -> bb_goal.cpp, IR_CHOICE -> flat_drive_pl_choice, IR_DISJ  */
/* -> flat_drive_pl_alt, IR_GCONJ -> flat_drive_pl_seq, IR_UNIFY/IR_BUILTIN/IR_ARITH -> their TEXT arms (the  */
/* byte-twins of the MEDIUM_BINARY arms PLR-J proved in mode-3). What was missing is the DRIVER INVOCATION:   */
/* a predicate-registry emit loop (each callee's entry body via walk_bb_flat in TEXT, defining the           */
/* .Lplpred_<name>_<arity> labels bb_goal.cpp calls). This recognizer is the gate for that loop — it returns */
/* the main graph's entry node iff EVERY node reachable in main AND in every transitively-called predicate   */
/* is a kind the TEXT emitter handles (FALL LOUD: an unhandled kind -> NULL -> EXCISED, never a silent        */
/* miscompile). It is strictly a SUPERSET of pl_flat_body_root: anything the flat tier accepts, this does     */
/* too, plus IR_GOAL / IR_CHOICE / IR_DISJ / IR_ITE / IR_STRUCT / IR_FAIL.                                    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
/* pl_rich_node_emittable — is a single IR node a kind the TEXT walk emits? Pure structural test over the     */
/* node KIND (and, for arith/unify operand leaves, their immediate operand kinds). Operand sub-nodes hanging  */
/* off the alpha/beta ports (logicvar/atom/literal/struct) are DATA the parent box reads; they are validated  */
/* by their parent's arm, not as standalone executable boxes.                                                 */
/* pl_rich_node_emittable — is a single IR node a kind the PLG-9d DETERMINISTIC tier emits correctly? This    */
/* is deliberately NARROWER than "has a TEXT arm": IR_DISJ (`;`) and IR_CHOICE (multi-clause enumeration)     */
/* both have TEXT arms (flat_drive_pl_alt / flat_drive_pl_choice) but their fail-driven BACKTRACKING through  */
/* the standalone-binary CP spine is NOT yet proven end-to-end (that is the next sub-rung). Admitting them    */
/* here makes the harder smoke shapes (clause enumeration, recursion) EMIT-then-produce-wrong-output = FAIL,  */
/* which is strictly worse than cleanly declining (EXCISED). So the deterministic tier rejects them: a        */
/* program qualifies only if main + every callee is a single-solution conjunction of head-unifies, builtins, */
/* arith, unify, and deterministic user calls — no disjunction, no multi-clause choice. FALL LOUD: anything   */
/* outside this set -> NULL -> EXCISED, never a silent miscompile.                                            */
static int pl_rich_node_emittable(const IR_t *nd) {
    if (!nd) return 1;
    switch (nd->t) {
    /* DETERMINISTIC control-flow / goal boxes (single-solution). NOTE: IR_DISJ and IR_CHOICE are            */
    /* intentionally NOT here — they need the backtracking CP spine the deterministic tier doesn't drive.   */
    case IR_GCONJ: case IR_GOAL: case IR_ITE:
    case IR_SUCCEED: case IR_FAIL: case IR_CUT:
    /* leaf/operand kinds (appear both as boxes and as α/β data) */
    case IR_LOGICVAR: case IR_ATOM: case IR_STRUCT:
    case IR_LIT_I: case IR_LIT_F: case IR_LIT_S: case IR_LIT_NUL:
        return 1;
    case IR_UNIFY:
        /* bb_unify TEXT arm builds each operand via build_operand_term (scalars + IR_STRUCT) then           */
        /* rt_pl_unify_terms — all operand shapes covered. */
        return 1;
    case IR_ARITH:
        /* IR_ARITH appears as the RHS of an `is` IR_BUILTIN; bb_builtin's is-arm flattens it. Standalone     */
        /* IR_ARITH boxes (comparison ops > < =:= ...) are emitted by bb_arith.cpp TEXT. */
        return 1;
    case IR_BUILTIN: {
        /* Restrict to builtins with a PROVEN MEDIUM_TEXT arm verified 3-mode by PLG-9a/b/c: the write       */
        /* family + nl/halt. For `is`, defer to the flat tier's pl_flat_goal_is_simple, which admits only    */
        /* INTEGER arith (rejects IR_LIT_F / float ops) — rt_pl_arith is integer-only, so a float `is`       */
        /* (X is pi, X is sqrt(4.0)) would miscompile = FAIL (rung29). The GOAL doc's "Mode-2 only" builtins */
        /* (numbervars, writeq, write_canonical, copy_term, atomic_list_concat, findall, retract, float      */
        /* arith) have no working TEXT arm and EXCISE here. Type-tests/comparisons have TEXT arms (PLR-J-1)  */
        /* but are not yet 3-mode-verified in the mode-4 facts/call context, so they EXCISE for now.         */
        const char *fn = nd->sval ? nd->sval : "";
        if (!strcmp(fn, "is")) return pl_flat_goal_is_simple(nd);
        static const char *ok[] = { "write", "writeln", "print", "nl", "halt", NULL };
        for (int k = 0; ok[k]; k++) if (!strcmp(fn, ok[k])) return 1;
        return 0;
    }
    default:
        /* IR_DISJ, IR_CHOICE, and any unrecognized kind -> not in the deterministic tier -> EXCISED. */
        return 0;
    }
}
/* pl_rich_graph_ok — every node in one predicate graph is in the deterministic tier. */
static int pl_rich_graph_ok(IR_graph_t *g) {
    if (!g || !g->all) return 0;
    for (int i = 0; i < g->n; i++)
        if (!pl_rich_node_emittable(g->all[i])) return 0;
    return 1;
}
/* pl_rich_body_root — gate for the PLG-9d mode-3/4 DETERMINISTIC rich-emit driver. Verifies main's graph AND */
/* every registered predicate is in the deterministic tier, returning main's body ROOT (or NULL -> EXCISED). */
/* Conservative: ALL registered predicates must pass, not just the reachable ones — the registry emit loop   */
/* emits every registered predicate, so any non-deterministic one would emit code that backtracks wrongly.   */
static IR_t * pl_rich_body_root(IR_graph_t *main_g) {
    if (!main_g || !main_g->entry) return NULL;
    if (!pl_rich_graph_ok(main_g)) return NULL;
    int npred = resolve_bb_pred_count();
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        IR_graph_t *pg = resolve_bb_graph_at(i);
        if (!pg) continue;
        if (!pl_rich_graph_ok(pg)) return NULL;   /* any non-deterministic callee -> EXCISED (no miscompile) */
    }
    /* Return the body ROOT the flat walk should enter. For a conjunctive body that is the principal        */
    /* IR_GCONJ (flat_drive_pl_seq walks its goals[] in order); for a single-goal / bare body it is the     */
    /* graph entry. Mirrors pl_flat_body_root's GCONJ-first selection — walking the raw entry node alone    */
    /* would emit ONLY the first goal (its γ wired straight to the sequence γ), dropping the rest.          */
    {
        IR_t *gconj = NULL;
        for (int i = 0; i < main_g->n; i++) {
            IR_t *nd = main_g->all[i];
            if (nd && nd->t == IR_GCONJ) { if (gconj) { gconj = NULL; break; } gconj = nd; }
        }
        if (gconj) return gconj;
    }
    return main_g->entry;
}
/*====================================================================================================================================================================================================*/
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_interp        = 0;
    int mode_run           = 0;
    int mode_compile       = 0;
    int mode_monitor       = 0;
    int bb_driver          = 0;
    int bb_live            = 0;
    int dump_ast           = 0;
    int dump_ast_bison     = 0;
    int dump_sm            = 0;
    int dump_bb            = 0;
    int dump_sno           = 0;
    int opt_trace          = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    int argi = 1;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--interp")        == 0) { mode_interp        = 1; argi++; }
        else if (strcmp(argv[argi], "--run")           == 0) { mode_run           = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile       = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strcmp(argv[argi], "--monitor")       == 0) { mode_monitor       = 1; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--bb=brokered")   == 0) { bb_driver          = 1; argi++; }
        else if (strcmp(argv[argi], "--bb=wired")      == 0) { bb_live            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast-bison") == 0) { dump_ast_bison    = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sm")       == 0) { dump_sm            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb")       == 0) { dump_bb            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sno")      == 0) { dump_sno           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-width")    == 0) {
            if (argi + 1 < argc) { ir_set_print_width(atoi(argv[++argi])); argi++; }
        }
        else if (strcmp(argv[argi], "--trace")         == 0) { opt_trace          = 1; argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench          = 1; argi++; }
        else if (strcmp(argv[argi], "--case-sensitive") == 0) { argi++; }
        else if (strcmp(argv[argi], "--fold-case")     == 0) {
            fprintf(stderr, "scrip: --fold-case is no longer supported; SCRIP is case-sensitive only\n");
            return 1;
        }
        else break;
    }
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && (mode_interp || mode_run || mode_monitor)) {
        fprintf(stderr,
            "scrip: --compile (x86) is mutually exclusive with "
            "--interp / --run / --monitor\n");
        return 1;
    }
    if (!mode_interp && !mode_run && !mode_monitor && !mode_compile)
        mode_run = 1;
    if (bb_driver && (mode_run || mode_compile)) {
        fprintf(stderr,
            "scrip: --bb=brokered is only valid under --interp; "
            "--run and --compile force --bb=wired\n");
        return 1;
    }
    if (bb_driver && bb_live) {
        fprintf(stderr,
            "scrip: --bb=brokered and --bb=wired are mutually exclusive\n");
        return 1;
    }
    if (!bb_driver && !bb_live) {
        if (mode_interp) bb_driver = 1;
        else             bb_live   = 1;
    }
    if (bb_live)   g_bb_mode = BB_MODE_LIVE;
    if (bb_driver) g_bb_mode = BB_MODE_BROKERED;
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [bb] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --interp         walk the BB port-graph in-process (Icon)\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "  --monitor        in-process sync comparator (unavailable)\n"
            "\n"
            "Byrd Box mode (under --interp; --run and --compile force wired):\n"
            "  --bb=brokered    pattern matching via driver/broker  [DEFAULT under --interp]\n"
            "  --bb=wired       live-wired BB blobs in exec memory (requires M-DYN-B* blobs)\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-bb        print BB-GRAPH for each proc\n"
            "  --dump-sno       transpile AST to portable SNOBOL4 source (GOAL-PARSER-SC-TRANSPILE.md SCT-1)\n"
            "  --trace          MONITOR trace output (diff vs CSNOBOL4)\n"
            "  --bench          print wall-clock time after execution\n"
            "  --dump-ast-bison dump AST via old Bison/Flex parser\n"
            "\n"
            "Frontend inferred from file extension:\n"
            "  .sno=SNOBOL4  .icn=Icon  .pl=Prolog  .sc=Snocone  .reb=Rebus\n"
        );
        return 1;
    }
    extern void sno_add_include_dir(const char *d);
    struct timespec _t0, _t1, _t2, _t3;
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t0);
    int first_file_argi = argi; (void)first_file_argi;
    int has_non_sno = 0;
    int is_prolog = 0;
    int is_icon = 0;
    int is_raku = 0;
    for (int fi = argi; fi < argc; fi++) {
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
    }
    CODE_t *sub = NULL;
    tree_t  *ast_prog = NULL;
    #define MERGE_AST(sub_ast) do { \
        if (sub_ast) { \
            if (!ast_prog) { ast_prog = sub_ast; } \
            else { \
                if (ast_prog->n > 0) { \
                    tree_t *_last = ast_prog->c[ast_prog->n-1]; \
                    if (_last && _last->t == TT_END) ast_prog->n--; \
                } \
                for (int _i = 0; _i < (sub_ast)->n; _i++) { \
                    ast_push(ast_prog, (sub_ast)->c[_i]); \
                } \
                if ((sub_ast)->c) free((char *)(sub_ast)->c - sizeof(size_t)); free(sub_ast); \
            } \
        } \
    } while(0)
    for (; argi < argc; argi++) {
        const char *input_path = argv[argi];
        {
            char dirbuf[4096];
            strncpy(dirbuf, input_path, sizeof dirbuf - 1);
            dirbuf[sizeof dirbuf - 1] = '\0';
            char *sl = strrchr(dirbuf, '/');
            if (sl) { *sl = '\0'; sno_add_include_dir(dirbuf); }
            else     { sno_add_include_dir("."); }
            const char *core_lib = getenv("SNO_LIB");
            if (core_lib && *core_lib) sno_add_include_dir(core_lib);
            char walk[4096];
            strncpy(walk, input_path, sizeof walk - 1);
            walk[sizeof walk - 1] = '\0';
            char *p = strrchr(walk, '/');
            while (p) {
                *p = '\0';
                char probe[4096];
                snprintf(probe, sizeof probe, "%s/lib", walk);
                struct stat st;
                if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)) {
                    sno_add_include_dir(walk);
                    break;
                }
                p = strrchr(walk, '/');
            }
            sno_add_include_dir(".");
        }
        const char *dot = strrchr(input_path, '.');
        int lang_snocone  = dot && strcmp(dot, ".sc")   == 0;
        int lang_prolog   = dot && strcmp(dot, ".pl")   == 0;
        int lang_icon     = dot && strcmp(dot, ".icn")  == 0;
        int lang_raku     = dot && strcmp(dot, ".raku") == 0;
        int lang_rebus    = dot && strcmp(dot, ".reb")  == 0;
        int lang_polyglot = dot && (strcmp(dot, ".scrip") == 0 || strcmp(dot, ".md") == 0);
        sub = NULL;
        if (lang_polyglot) {
            g_polyglot = 1;
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = parse_scrip_polyglot(src, input_path);
            free(src);
            MERGE_AST(sub_ast);
        } else if (lang_snocone || lang_prolog || lang_icon || lang_raku || lang_rebus) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = NULL;
            if (lang_icon)         icon_compile(src, input_path, &sub_ast);
            else if (lang_raku)    raku_compile(src, input_path, &sub_ast);
            else if (lang_prolog)  prolog_compile(src, input_path, &sub_ast);
            else if (lang_rebus)   rebus_compile(src, input_path, &sub_ast);
            else                   snocone_compile(src, input_path, &sub_ast);
            free(src);
            if (dump_ast && sub_ast) {
                ir_dump_program(sub_ast, stdout); return 0;
            }
            MERGE_AST(sub_ast);
        } else if (dump_ast) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            ir_dump_program(sub_ast, stdout);
            return 0;
        } else if (dump_sno) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            tree_t *sub_ast = sno_parse_ast(f, input_path, dump_ast_bison ? &sub : NULL);
            fclose(f);
            if (dump_ast_bison) { ir_dump_program(sub, stdout); return 0; }
            MERGE_AST(sub_ast);
        }
        if (!ast_prog) {
            fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
            return 1;
        }
    }
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
    const char *input_path = argv[argc - 1];
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t2);
    if (!ast_prog) {
        fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
        return 1;
    }
    {
        extern void bb_pool_init(void);
        bb_pool_init();
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    extern void SNO_INIT_fn(void);
    SNO_INIT_fn();
    stmt_init();
    register_fn("IDENT",  _builtin_IDENT,  1, 2);
    register_fn("DIFFER", _builtin_DIFFER, 1, 2);
    register_fn("EVAL",   _builtin_EVAL,   1, 1);
    register_fn("CODE",   _builtin_CODE,   1, 1);
    register_fn("DATA",   _builtin_DATA,   1, 1);
    register_fn("print",  _builtin_print,  0, 99);
    extern DESCR_t (*g_user_call_hook)(const char *, DESCR_t *, int);
    g_user_call_hook = _usercall_hook;
    {
        extern void core_set_label_exists_hook(int (*fn)(const char *));
        core_set_label_exists_hook(_label_exists_fn);
    }
    {
        extern DESCR_t (*g_eval_pat_hook)(DESCR_t pat);
        g_eval_pat_hook = _eval_pat_impl_fn;
    }
    {
        extern DESCR_t (*g_eval_str_hook)(const char *s);
        g_eval_str_hook = _eval_str_impl_fn;
    }
    g_opt_trace   = opt_trace;
    g_opt_dump_bb = dump_bb;
    if (dump_sm) {
        fprintf(stderr, "scrip: --dump-sm removed (Stack Machine excised). Use --dump-bb.\n");
        return 1;
    }
    if (dump_sno) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_bb) {
        extern void bb_print(const IR_graph_t * bbg, FILE * fp);
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            fprintf(stdout, "; proc %s\n", pname);
            bb_print(s2->bbp.table[idx], stdout);
        }
        return 0;
    }
    if (mode_compile_x86) {
        extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
        extern int g_frame_active;
        if (is_icon || is_raku) {
            /* MODE-4 (BB-native x86, GROUND ZERO 3): emit a standalone GAS .s — a C-ABI `main` wrapper that  */
            /* fetches the per-sequence frame (rt_frame), passes it as ζ (rdi) with the α entry selector      */
            /* (esi=0), and calls the flat BB body, then the body itself via codegen_flat_build (the SAME BB  */
            /* templates mode-3 emits, in MEDIUM_TEXT). g_frame_active makes the prologue/epilogue use the    */
            /* Icon stackless r12-frame form. The .s links libscrip_rt.so (rt_write_*, rt_frame, rt_call_*).  */
            /* RK-EMIT-1 (2026-05-31): Raku rides this SAME generic four-port emission path — Raku's lowered  */
            /* IR is built from the SHARED Icon kinds (IR_CALL, IR_LIT scalars, IR_VAR, IR_TO, IR_ASSIGN,     */
            /* IR_IF, IR_WHILE, IR_BINOP, IR_ALT) plus the Raku generator kinds (IR_GATHER, IR_MAP, IR_GREP), */
            /* so the SAME flat chain builder + shared templates emit it. No severed [SBB] adapter is touched */
            /* — Raku gets its OWN LOWER-direct driver exactly like Icon's, which is what the diagnosis asked.*/
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern int icn_flat_chain_build_text(IR_t * entry, FILE * out, const char * prefix);
            extern int icn_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname);
            IR_t *icn_root = icn_ring_to_tree(bbg);
            int use_chain = (icn_root == NULL);
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            /* GZ-10 mode-4: emit each user procedure body as a named slab BEFORE main so all labels are     */
            /* defined before the main body references them. Each proc gets prefix "icn_proc_<name>" whose   */
            /* alpha entry point is the globally-visible label "icn_proc_<name>_α" (UTF-8 \xce\xb1).        */
            /* A startup stub icn_proc_startup calls rt_proc_set_fn to wire each proc name -> slab fn ptr,   */
            /* then main calls the startup stub before calling main_α.                                        */
            g_frame_active = 1;
            int n_procs = 0;
            static char proc_names_buf[64][128];
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                icn_flat_chain_build_proc_text(s2->bbp.table[idx]->entry, pn, np, stdout, pname);
                if (n_procs < 64) snprintf(proc_names_buf[n_procs++], 128, "%s", pname);
                free(pn);
            }
            /* Startup stub: registers each proc slab pointer with the runtime before main_α runs.           */
            /* Uses rt_proc_set_fn(name, fn) — the existing "wire name to fn ptr" helper in rt.c.            */
            if (n_procs > 0) {
                printf("icn_proc_startup:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                for (int i = 0; i < n_procs; i++) {
                    printf("  .section .rodata\n");
                    printf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
                    printf("  .section .text\n");
                    printf("  .intel_syntax noprefix\n");
                    printf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    /* icn_proc_<name>_α is the UTF-8 alpha label — emit via hex escape in printf */
                    printf("  lea rsi, [rip + icn_proc_%s_\xce\xb1]\n", proc_names_buf[i]);
                    printf("  call rt_proc_set_fn@PLT\n");
                }
                printf("  pop rbp\n");
                printf("  ret\n");
            }
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (n_procs > 0)
                printf("  call icn_proc_startup\n");
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            int rc = use_chain ? icn_flat_chain_build_text(bbg->entry, stdout, "main")
                               : codegen_flat_build(icn_root, stdout, "main");
            g_frame_active = 0;
            fflush(stdout);
            return rc;
        }
        if (is_prolog) {
            /* PLG-9a (mode-4 --compile --target=x86, 2026-05-31): emit a standalone GAS .s for the proven    */
            /* hello-world tier, mirroring the Icon mode-4 arm above EXACTLY. pl_flat_body_root recognizes the */
            /* same shape the PLG-8-native mode-3 arm JITs (a single-clause body that is a conjunction of      */
            /* constant-arg builtins write/writeln/print/nl/halt + IR_SUCCEED/IR_CUT/IR_ATOM, nslots==0); the  */
            /* identical g_frame_active ζ-frame model is used (rt_frame -> rdi, esi=0, call main_α). The body  */
            /* is emitted by codegen_flat_build in MEDIUM_TEXT — the SAME bb_builtin write/nl TEXT arm whose    */
            /* MEDIUM_BINARY twin mode-3 emits (MIGRATION-MODE4-IS-MODE3-DUMP: one template, two output sinks). */
            /* The write/1 IR_ATOM arg label is produced by strtab_label (g_flat_intern_str hook unset), so    */
            /* the .rodata string table is flushed by xa_strtab_rodata after the walk (Prolog differs from     */
            /* Icon here: Icon's bb_call emits its rodata inline). The .s links libscrip_rt.so (rt_pl_write_    */
            /* atom, putchar, rt_frame). Every richer shape -> pl_flat_body_root returns NULL -> EXCISED banner */
            /* + non-zero return (smoke reports m4 EXCISED, not FAIL; no regression). Widen rung by rung.       */
            extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
            extern int codegen_pl_program(FILE * out);
            extern int g_frame_active;
            extern void xa_emit_strtab_rodata(void);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[PBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-4 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled)\n");
                return 1;
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (!flat_root) {
                /* PLG-9d (2026-06-01): the flat tier (single-clause, no call/choice) declined. Try the RICH  */
                /* tier — facts + clause choice + user-predicate calls. pl_rich_body_root verifies main AND   */
                /* every registered predicate are fully TEXT-emittable (else NULL -> EXCISED, no miscompile). */
                /* On accept, emit: (1) rt_pl_main_init (atoms+trail+GC the standalone binary needs); (2) the */
                /* main: C-ABI wrapper -> main_α; (3) every predicate's callee block (codegen_pl_program, so  */
                /* the .Lplpred_<name>_<arity> labels bb_goal.cpp calls resolve); (4) main_α itself. The      */
                /* SAME ζ-frame model + walk_bb_flat dispatch as the flat tier; richer kinds (IR_GOAL ->      */
                /* bb_goal, IR_CHOICE -> flat_drive_pl_choice) light up via the shared dispatch.              */
                IR_t *rich_root = pl_rich_body_root(pl_main);
                if (!rich_root) {
                    fprintf(stderr, "[SMX] --compile --target=x86: Prolog mode-4 covers the hello-world + "
                                    "unify/arith + facts/choice/call tiers; this program has a construct not "
                                    "yet wired (PLG-9e+).\n");
                    return 1;
                }
                printf("  .intel_syntax noprefix\n");
                printf("  .text\n");
                printf("  .globl main\n");
                printf("main:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                printf("  call rt_pl_main_init@PLT\n");
                if (pl_main->nslots > 0) {
                    printf("  mov edi, %d\n", pl_main->nslots);
                    printf("  call rt_pl_env_alloc@PLT\n");
                }
                printf("  call rt_frame@PLT\n");
                printf("  mov rdi, rax\n");
                printf("  xor esi, esi\n");
                printf("  call main_\xce\xb1\n");
                printf("  xor eax, eax\n");
                printf("  pop rbp\n");
                printf("  ret\n");
                g_frame_active = 1;
                int rcp = codegen_pl_program(stdout);          /* callee predicate blocks */
                int rcm = codegen_flat_build(rich_root, stdout, "main");  /* main_α body */
                g_frame_active = 0;
                xa_emit_strtab_rodata();
                fflush(stdout);
                return (rcp || rcm) ? 1 : 0;
            }
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            /* PLG-9b (2026-05-31): if the clause body has logic-variable slots, allocate the per-activation */
            /* env BEFORE running the body. The slots ARE the per-box RW storage for variables (the unify   */
            /* box writes g_resolve_env[slot], the write box reads it). Mode-3 does this in the driver; the  */
            /* emitted mode-4 binary has no driver, so it calls rt_pl_env_alloc(nslots) here. nslots==0 (the */
            /* PLG-9a hello tier) skips it (g_resolve_env stays NULL, never dereferenced).                   */
            if (pl_main->nslots > 0) {
                printf("  mov edi, %d\n", pl_main->nslots);
                printf("  call rt_pl_env_alloc@PLT\n");
            }
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            g_frame_active = 1;
            int rc = codegen_flat_build(flat_root, stdout, "main");
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
            return rc;
        }
        {
            /* SBL-RING-REMOVE (2026-05-31, Opus 4.8): SNOBOL4 mode-4 BB-native x86 emission now ABORTS.
               It previously leaned on sno_ring_to_tree (the postfix-ring → four-port-tree adapter) which is
               REMOVED as a VIOLATION (Lon directive) — the topology must come from LOWER, not be re-derived at
               emit time. The mode-4 emission scaffolding (codegen_flat_build + the XA wrap templates) is intact
               and unchanged; the missing piece is LOWER emitting the four-port statement-BB graph directly, after
               which mode-4's TEXT arm and mode-3's BINARY arm of the SAME box light up together. Until then,
               abort below (by design). NO storage outside the boxes (PER-BOX LOCAL STORAGE FACT RULE). */
            extern void xa_file_header(void);
            extern void emit_io_set_sink(FILE * out);
            extern void emitter_init_text(FILE * out, int mode);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[SBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            (void)sbbg;
            fprintf(stderr, "[SBB] mode-4: sno_ring_to_tree REMOVED (VIOLATION, Lon 2026-05-31). SNOBOL4 "
                            "mode-4 emission must come from LOWER producing the four-port statement-BB graph "
                            "directly (no ring->tree adapter); not yet wired. Aborting (by design).\n");
            abort();
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen excised).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_monitor) {
        fprintf(stderr, "[NO-SM-BB] --monitor: trampoline codegen deleted (FACT RULE); unavailable\n");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    } else if (mode_interp) {
        extern int g_icn_postfix_resume;
        if (is_icon) g_icn_postfix_resume = 1;   /* mode-2 port-walker: wire deterministic-builtin CALL resume to arg resume (re-pump generator args); Icon-only */
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-2 driver: main BB graph not found\n");
                abort();
            }
            (void)bb_exec_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (!is_icon && !is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[SBB] FATAL: mode-2 driver: SNOBOL4 main BB graph not found\n");
                abort();
            }
            (void)bb_exec_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-2 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            (void)bb_exec_once(pl_main);
            goto run_done;
        }
        fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Non-Icon mode-2 (--interp) "
                        "execution is gone. This language has not yet crossed onto Byrd Boxes. "
                        "Aborting (by design).\n");
        abort();
    } else if (mode_run) {
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_raku) {
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            extern bb_box_fn icn_flat_chain_build_proc(IR_t * entry, const char ** pnames, int np);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
            g_frame_active = 1;   /* GZ-10: proc slabs (and main below) build with the push-r12 ζ-frame prologue */
            /* GZ-10 (modes 3/4): TWO PHASES. Phase 1 registers EVERY user proc (name + params + entry) so   */
            /* that during Phase 2 slab emission a call to ANY proc — including a forward or mutually-         */
            /* recursive one not yet built — passes the rt_proc_is_registered gate in the bb_call dval==3.0   */
            /* arm. Building a slab the moment its proc was registered (one-pass) made `iseven` calling the    */
            /* later-registered `isodd` fall through to the unsupported-shape abort. Registration is cheap     */
            /* (no emission); only Phase 2 emits, by which point the whole proc set is known.                  */
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                /* GZ-10 (modes 3/4): build the procedure body NOW as a stackless flat slab with the        */
                /* return-slot/param-slot convention; rt_icn_call_proc_descr invokes this fn per activation. */
                bb_box_fn pfn = icn_flat_chain_build_proc(s2->bbp.table[idx]->entry, pn, np);
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            {
                extern void *dat_register(const char *spec);
                for (int _gi = 0; _gi < s2->bbp.count; _gi++) {
                    IR_graph_t *g = s2->bbp.table[_gi];
                    if (!g || !g->all) continue;
                    for (int _ni = 0; _ni < g->n; _ni++) {
                        IR_t *nd = g->all[_ni];
                        if (nd && nd->t == IR_RECORD_DEF && nd->sval) dat_register(nd->sval);
                    }
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph not found\n");
                abort();
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            if (!bbg->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph has no entry\n");
                abort();
            }
            extern void *rt_frame(void);
            extern bb_box_fn icn_flat_chain_build(IR_t * entry);
            IR_t *icn_root = icn_ring_to_tree(bbg);
            bb_box_fn fn;
            if (icn_root) {
                fn = bb_build_flat(icn_root);
            } else {
                /* GZ-7 (GROUND ZERO 3): the single-expression-tree adapter could not linearize this graph */
                /* (multi-statement, a variable read/assign, or branching control flow). Emit it as a FLAT  */
                /* GOTO-GRAPH in the test_sno_*.c named-slot model — every box once, wired by its native    */
                /* gamma/omega ports, operands read from producer slots. NO ring (mode-2 only), NO stack.   */
                fn = icn_flat_chain_build(bbg->entry);
            }
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: bb_build_flat returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            (void)fn(rt_frame(), 0);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void *rt_frame(void);
            extern int g_frame_active;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-3 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            /* PLG-8-native (2026-05-31): for the proven hello-world shape, EMIT the boxes natively via    */
            /* bb_build_flat (code+data in the boxes; values up the chain in per-box slots; NO ring) and    */
            /* run the JIT'd box, exactly as Icon/SNOBOL4 mode-3 do. Every richer shape -> pl_flat_body_root */
            /* returns NULL and we keep the proven interim bb_exec_once route (no regression). Widen rung-   */
            /* by-rung as the flat templates are verified for choice/unify/arith/user-call.                 */
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (flat_root) {
                g_frame_active = 1;
                bb_box_fn pfn = bb_build_flat(flat_root);
                g_frame_active = 0;
                if (pfn) { (void)pfn(rt_frame(), 0); goto run_done; }
            }
            (void)bb_exec_once(pl_main);
            goto run_done;
        }
        {
            if (is_prolog) {
                fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Prolog mode-3 (--run) "
                                "native execution is gone. This language has not yet crossed "
                                "onto Byrd Boxes. Aborting (by design).\n");
                (void)s2;
                abort();
            }
            /* SBL-M3-CHAIN (2026-05-31, Opus 4.8): SNOBOL4 mode-3 native execution from LOWER's four-port
               statement-BB graph directly — NO ring->tree adapter (sno_ring_to_tree stays removed). The
               graph's entry is land[0] (an IR_SUCCEED landing); sno_flat_chain_build resolves landings
               transitively and emits the flat goto-graph (every box once, native γ/ω ports, NO value stack,
               NO ring; per-box RO [rip+disp] / RW [ζ=r12+off]). Shapes whose boxes have no BINARY arm yet
               make sno_flat_chain_build return NULL -> SOFT honest fall (loud stderr, clean exit, NO abort),
               so a working shape (e.g. OUTPUT='hello') runs while unbuilt shapes produce empty output. */
            extern bb_box_fn sno_flat_chain_build(IR_graph_t * g);
            extern void *rt_frame(void);
            extern int g_frame_active;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                bb_box_fn fn = sno_flat_chain_build(sbbg);
                g_frame_active = 0;
                if (fn) { (void)fn(rt_frame(), 0); goto run_done; }
            }
            fprintf(stderr, "[SBB] mode-3: SNOBOL4 statement shape not yet flat-emittable (a box lacks a "
                            "MEDIUM_BINARY arm); soft fall — no output for this shape. No abort.\n");
            goto run_done;
        }
    } else if (has_non_sno) {
        (void)sm_preamble;
        fprintf(stderr, "[MODE] FATAL: reached dead has_non_sno branch — mode resolution is broken; "
                        "refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    } else {
        fprintf(stderr, "[MODE] FATAL: reached dead default dispatch branch — no execution mode "
                        "selected; refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    }
run_done:
    if (opt_bench) {
        clock_gettime(CLOCK_MONOTONIC, &_t3);
        double parse_ms = (_t1.tv_sec - _t0.tv_sec)*1e3 + (_t1.tv_nsec - _t0.tv_nsec)/1e6;
        double lower_ms = (_t2.tv_sec - _t1.tv_sec)*1e3 + (_t2.tv_nsec - _t1.tv_nsec)/1e6;
        double exec_ms  = (_t3.tv_sec - _t2.tv_sec)*1e3 + (_t3.tv_nsec - _t2.tv_nsec)/1e6;
        fprintf(stderr, "BENCH parse=%.2fms lower=%.2fms exec=%.2fms total=%.2fms\n",
                parse_ms, lower_ms, exec_ms, parse_ms + lower_ms + exec_ms);
    }
    if (getenv("BINARY_AUDIT") || getenv("SNO_BINARY_BOXES")) {
        extern void bin_audit_print(void);
        bin_audit_print();
    }
    return 0;
}
