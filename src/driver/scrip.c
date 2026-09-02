#include "rt/rt_arena.h"
#include "../ir/pin_va.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include "../parsers/snobol4/scrip_cc.h"
#include "../parsers/snocone/snocone_driver.h"
#include "../parsers/prolog/prolog_driver.h"
#include "../parsers/prolog/term.h"
#include "../parsers/prolog/prolog_runtime.h"
#include "../parsers/prolog/prolog_atom.h"
#include "../parsers/icon/icon_driver.h"
#include "../parsers/pascal/pascal_driver.h"
#include "../parsers/raku/raku_driver.h"
#include "../parsers/rebus/rebus_lower.h"
#include "../runtime/builtins/gen.h"
#include "../parsers/icon/icon_lex.h"
#include "../ir/bb_box.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
extern int pl_dyn_is_marked(const char *name, int arity);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "emit.h"
#include "bb_build.h"
#include "scrip_sm.h"
extern DESCR_t pat_at_cursor(const char *varname);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void stmt_init(void) {}
static IR_graph_t *g_ab_posthook_g = NULL; static int g_ab_posthook_gva = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_ab_posthook(void) { extern void bb_ab_emit_nodes(IR_graph_t*, int); if (g_ab_posthook_g) bb_ab_emit_nodes(g_ab_posthook_g, g_ab_posthook_gva); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* N-2 item 2 step 1 (hq_P): HOST-DETECTION PREDICATE, INERT -- emits nothing.  Answers two questions: (1) which graphs host a DIRECT
   call to a generator proc, and (2) whether that callee is emitted LATER than its host -- the forward reference under which
   proc_fb_buf[] reads 0 rather than erroring, sizing a host carve silently too small.
   The host edge is IR_PROC_GEN, NOT a call op.  lower_icon.c:148 already resolves the callee and encodes the answer in the OPCODE
   (`icn_proc_is_generator(name) ? IR_PROC_GEN : IR_CALL`), and :1374 rewrites the same way post-lower -- so the NODE KIND IS THE
   PREDICATE and no is_generator lookup is needed.  The proc table is consulted only to resolve the callee INDEX for the
   forward-reference test.  IR_CALL_VALUE is counted separately and never as a host edge: indirect dispatch is explicitly UNRULED
   (ceo 2026-08-27) and must not enter this slice.  A callee absent from the proc table is reported as callee_idx=-1, never as 0.
   ⛔ THE TEST IS EMISSION ORDER, NOT TABLE ORDER.  Every emit loop `continue`s on main and emits it separately afterwards, so a
   main host is emitted LAST and can never forward-reference anything -- scoring it by table index reports a hazard that cannot
   occur.  Emission position is therefore proc_count for main and the table index for every other proc. */
static int n2_proc_index(const stage2_t *s2, const char *fn) { if (!s2 || !fn) return -1;
    for (int i = 0; i < s2->proc_count; i++) { const char *pn = s2->proc_table[i].name; if (pn && !strcmp(pn, fn)) return i; } return -1; }
/* Selects "main" via module_registry.main_mod + that module's proc range first (fixes cross-language collisions:
   picks the right MODULE, and within it the FIRST "main"-named entry -- which for Prolog's two main/0 registrations
   is the dedicated goal-entry-point graph, registered before the generic-predicate one). Falls back to the old
   flat-table first-wins scan when the registry is unset or its range doesn't resolve, so a frontend whose
   main_mod/nprocs bookkeeping has a gap this pass didn't find never regresses below the already-landed baseline. */
static int polyglot_main_bb_idx(const stage2_t *s2) { if (!s2) return -1;
    int mm = s2->module_registry.main_mod;
    if (mm >= 0 && mm < s2->module_registry.nmod) { const ScripModule *m = &s2->module_registry.mods[mm];
        int lo = m->proc_start, hi = lo + m->nprocs; if (lo < 0) lo = 0; if (hi > s2->proc_count) hi = s2->proc_count;
        for (int i = lo; i < hi; i++) { const char *pn = s2->proc_table[i].name; if (pn && !strcmp(pn, "main")) return s2->proc_table[i].bb_idx; } }
    for (int i = 0; i < s2->proc_count; i++) { const char *pn = s2->proc_table[i].name; if (pn && !strcmp(pn, "main")) return s2->proc_table[i].bb_idx; }
    return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void n2_host_scan_diag(const stage2_t *s2) { if (!s2 || !getenv("SCRIP_N2_HOST_DIAG")) return; int hosts = 0, fwd = 0, indirect = 0, edges = 0, unresolved = 0;
    for (int hi = 0; hi < s2->proc_count; hi++) { const char *hn = s2->proc_table[hi].name; int gi = s2->proc_table[hi].bb_idx;
        if (!hn || gi < 0 || gi >= s2->bbp.count || !s2->bbp.table[gi]) continue; IR_graph_t *g = s2->bbp.table[gi]; int fired = 0;
        for (int k = 0; k < g->n; k++) { IR_t *nd = g->all[k]; if (!nd) continue; if (nd->op == IR_CALL_VALUE) { indirect++; continue; }
            if (nd->op != IR_PROC_GEN) continue; const char *fn = IR_LIT(nd).sval; int ci = n2_proc_index(s2, fn);
            int hpos = !strcmp(hn, "main") ? s2->proc_count : hi; int cpos = (ci >= 0 && s2->proc_table[ci].name && !strcmp(s2->proc_table[ci].name, "main")) ? s2->proc_count : ci;
            int is_fwd = (ci >= 0) && (cpos > hpos); fired = 1; edges++; if (is_fwd) fwd++; if (ci < 0) unresolved++;
            fprintf(stderr, "[N2-HOST] host=%s host_idx=%d emit_pos=%d gen_callee=%s callee_idx=%d emit_pos=%d forward=%d\n", hn, hi, hpos, fn ? fn : "(null)", ci, cpos, is_fwd); }
        if (fired) hosts++; }
    fprintf(stderr, "[N2-HOST] SUMMARY procs=%d hosts=%d edges=%d forward_refs=%d unresolved_callees=%d indirect_call_nodes=%d\n", s2->proc_count, hosts, edges, fwd, unresolved, indirect); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* N-2 item 2, THE FORWARD-REFERENCE GUARD (hq_P 2026-08-27).  Step 1 measured 3 genuine forward host->generator edges across bench_correct (geddump event->gedval,
   gedload->gedwalk; tgrlink dumpcode->aseq) and warned they read proc_fb_buf[] as 0 rather than erroring, sizing a host carve silently too small.  MEASURED HERE:
   THE ARRAY IS THE HAZARD, NOT THE INFORMATION.  proc_fb_buf[] is filled DURING the emission loop from g_last_flat_frame_bytes, so a host emitted before its callee
   reads an unwritten slot -- but g_last_flat_frame_bytes is only g_emit_cfg->jcon_value_region (emit.cpp:3475), a PER-GRAPH field assigned exactly once at IR
   finalization (scrip_ir.c:283, after zls_build) and never assigned again anywhere in src/.  So a callee generator's frame bytes are already knowable at host-alpha
   time by reading the callee graph directly -- no table, no two-pass emission, and NO NEW GLOBAL, which the standing rule forbids without an in-chat grant.
   ⛔ MEASURED CORRECTION, and it is why this probe exists rather than a bare assertion: the field is NOT live as soon as the stage2 table is built.  Probing it
   at sm_preamble() time reads region=0 for nearly every proc (measured 427 of 429 comparisons mismatching, against post-emit values of 992/720/3616/...).  It is
   assigned by drive_slots_all() -> ir_drive_slot_assign() -> scrip_ir.c:283, which runs per-language at scrip.c:1261/1439/1683/1805 -- still BEFORE every emission
   loop, so the claim survives, but ONLY when read after that pass.  An item-2 carve sized from the field any earlier would have silently read 0 for every callee.
   These two probes exist to PROVE that before item 2 relies on it, rather than to assert it: PREPASS prints every proc's region BEFORE any graph is emitted;
   POSTEMIT prints the emission-time value the old array would have stored, on both the mode-4 and mode-3 loops; scripts/test_icn_n2_fb_prepass.sh REFUSES rc=2
   unless they agree for every proc.  Read-only, getenv-gated, no emission change -- an inert step, landed and proven inert before anything is built on it. */
static void n2_fb_prepass_diag(const stage2_t *s2) { if (!s2 || !getenv("SCRIP_N2_FB_PREPASS")) return;
    for (int i = 0; i < s2->proc_count; i++) { const char *pn = s2->proc_table[i].name; int gi = s2->proc_table[i].bb_idx;
        if (!pn || gi < 0 || gi >= s2->bbp.count || !s2->bbp.table[gi]) continue;
        fprintf(stderr, "[N2-FB] PREPASS proc=%s idx=%d gen=%d region=%d\n", pn, i, s2->proc_table[i].is_generator, s2->bbp.table[gi]->jcon_value_region); }
    fprintf(stderr, "[N2-FB] PREPASS-END procs=%d\n", s2->proc_count); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ N-2 TRANSITIVE-RESERVE STEP 1 (hq_P 2026-08-29, row icon-n2-flat-gen-host-transitive-reserve): VERIFY CROSS-GRAPH
   NODE ACCESS BEFORE BUILDING THE RECURSIVE SUM ON IT. The row's own GOAL orders this first and says why: this rung has
   FIVE plausible-zero scars (step 1 hosts=0; step 1b ft "obviously" 0 and measured 96; icn_gen_host_reserve_offset
   returning off=0 from a host that never carved). ⛔ reserve(g) = Σ over g's generator call nodes of (align16(ft)+48+
   reserve(callee)) has to walk a graph that is NOT g_emit_cfg, and every existing consumer of this predicate walks only
   the CURRENT graph. Two independent things could each silently answer zero here and both look identical to "this host
   calls no generators": (1) another graph's all[]/n may not be populated in the post-drive_slots_all window, and
   (2) the rt_proc_is_* registration lookups may not answer yet this early -- registration is ORDERED, which is exactly
   why icn_gen_host_reserve() carries a forward-reference refusal at all. This probe reports BOTH, per graph, so the
   answer is measured rather than assumed. Inert: getenv-gated, stderr only, zero emission effect. */
static void n2_xgraph_probe(const stage2_t *s2) { if (!s2 || !getenv("SCRIP_N2_XGRAPH")) return;
    fprintf(stderr, "[N2-XG] window=post-drive_slots_all procs=%d bbp.count=%d\n", s2->proc_count, s2->bbp.count);
    for (int i = 0; i < s2->proc_count; i++) { const char *pn = s2->proc_table[i].name; int gi = s2->proc_table[i].bb_idx;
        if (!pn || gi < 0 || gi >= s2->bbp.count || !s2->bbp.table[gi]) { fprintf(stderr, "[N2-XG] proc=%s UNREACHABLE gi=%d\n", pn ? pn : "(null)", gi); continue; }
        IR_graph_t *g = s2->bbp.table[gi];
        int calls = 0, named = 0, reg = 0, gen = 0;
        for (int k = 0; k < g->n; k++) { IR_t *hn = g->all[k]; if (!hn) continue;
            if (!ir_is_call_kind(hn->op) && hn->op != IR_CALL && hn->op != IR_PROC_GEN) continue;
            calls++;
            { const char *cn = IR_LIT(hn).sval; if (!cn || !cn[0]) continue; named++;
              if (rt_proc_is_registered(cn)) { reg++; if (rt_proc_is_generator(cn)) gen++; } } }
        fprintf(stderr, "[N2-XG] proc=%-10s is_gen=%d all=%s n=%-4d calls=%-3d named=%-3d registered=%-3d generator=%d\n",
                pn, s2->proc_table[i].is_generator, g->all ? "OK" : "NULL", g->n, calls, named, reg, gen); }
    fprintf(stderr, "[N2-XG] END -- a 0 in n/calls is the plausible-zero this probe exists to expose, not evidence of no callees\n"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* N-2 FORWARD-REF PRE-PASS, THE CURE (seat01 2026-08-29, row icon-n2-forward-ref-gen-prepass): registers every generator proc's frame bytes into the
   pz[] registry (zeta_storage.c) BEFORE any graph is emitted, so icn_gen_host_reserve()'s emit_patzeta_frame_reserve() lookup HITS even when a host
   is emitted before the generator it calls (a forward reference in source order) -- measured live in bench_correct: geddump 6 such call sites,
   tgrlink 2. Same window step 1b proved stable (AGREE=429/MISMATCH=0 on jcon_value_region): live immediately after drive_slots_all(), before any
   emission loop reads or writes a graph's fields again. ⛔ ONLY frame bytes (fb) and fp are computed; uniform is passed 0, not derived: its one
   reader, fct_defer_susp() (zeta_storage.c), only ever looks up names prefixed "PAT$" (SNOBOL4 deferred-pattern names) -- a namespace disjoint from
   Icon proc names -- so no entry this pre-pass writes is ever read through that field, and computing it correctly would need a non-static export of
   emit.cpp's emit_graph_uniform() for a value nothing here consumes. fp mirrors the REAL per-emission registration (g_last_flat_fp, emit.cpp:3639)
   against the callee's OWN graph, since no graph is "current" yet at this window.
   ⛔⭐ THE ORIGINAL CLAIM HERE -- "the two calls agree ... a pre-pass value is never stale, only ever superseded by an identical one" -- WAS TRUE WHEN
   WRITTEN AND WENT FALSE UNDER A CURE LANDED ELSEWHERE (hq_B 2026-08-30, SCRIP 3fe34608): that commit gave the per-emission side a zframe arm
   (fp=0, because xa_flat_zframe_prologue_str carves EXACTLY flat_frame_bytes) while this pre-pass kept registering the Icon-shaped (np+nl)*16 for
   every is_generator proc -- and lower_prolog.c:1514 marks EVERY Prolog graph zframe_graph, so every Prolog predicate was registered here too.
   MEASURED on fact/2 (np=2 nl=4): pre-pass ft=1328 vs per-emission ft=1232 vs the callee's own `sub rsp, 1232` -- over by exactly 96 == (np+nl)*16.
   The disagreement is WINDOW-DEPENDENT, which is the hazard: a host emitted AFTER its callee reads the corrected 1232, a host emitted BEFORE it (the
   forward reference this pre-pass exists to serve) reads 1328. The zframe arm below restores the agreement the paragraph asserts.
   ⭐ A COMMENT CANNOT NOTICE THAT THE CODE IT DESCRIBES MOVED. This one asserted an invariant across two files, nothing read it, and the cure that
   broke it was correct in its own file -- so the assertion decayed silently in the gap between them (hq_C 2026-08-30: a declared expectation nothing
   reads is a comment, not a gate). If the two formulas ever diverge again, SCRIP_N2_FT_PROBE=1 prints both sides on one line each -- [N2-PREPASS]
   here and [N2-FT] EMIT from emit.cpp -- so the check is one run, not a re-derivation.
   ⛔ THE _zfA KEY IS LOAD-BEARING AND IS NOT A TUNING KNOB: it mirrors emit.cpp:3639 term for term, and the key is the α-SELECTION STATE
   (zframe_graph && !icn_cells_graph), never the bare zframe_graph field -- a graph carrying BOTH flags takes an icn_cells α that DOES add
   (np+nl)*16 back, so keying on the field alone would register a frame SILENTLY TOO SMALL, the exact silent-overflow direction ceo refused
   worst-case reservation to avoid. Icon graphs never set zframe_graph at all (lower_prolog.c:1514, lower_raku.c:1141, lower_pascal.c:790 are
   the only three writers), so this arm cannot reach an Icon proc -- measured, not assumed: Icon watermark unmoved, emitted .s byte-identical
   Only is_generator procs are registered -- the sole population icn_gen_host_reserve() ever queries by name -- leaving pz[]'s 512-slot table
   headroom untouched for the rest. ⭐ INERT ON THE UNARMED PATH BY CONSTRUCTION, NOT BY A GATE: pz[] registration already runs unconditionally today
   (all four real call sites carry no icn_genframe2() check), and every consumer of emit_patzeta_frame_reserve() is itself gated on icn_genframe2()
   (icn_gen_host_reserve() etc., x86_asm.h) -- so an unarmed build looks up nothing this pre-pass touches, armed or not. The forward-reference refusal
   in icn_gen_host_reserve() is UNTOUCHED and still fires on a genuine miss (see the poison-arm test) -- this pre-pass only removes the miss that was
   never supposed to be one. */
static void n2_fb_prepass_register(const stage2_t *s2) { if (!s2) return;
    extern void emit_patzeta_register(const char *, int, int, int);
    for (int i = 0; i < s2->proc_count; i++) { if (!s2->proc_table[i].is_generator) continue;
        const char *pn = s2->proc_table[i].name; int gi = s2->proc_table[i].bb_idx;
        if (!pn || gi < 0 || gi >= s2->bbp.count || !s2->bbp.table[gi]) continue;
        IR_graph_t *g = s2->bbp.table[gi];
        { int _zfA = (g->zframe_graph && !g->icn_cells_graph); int _fp = _zfA ? 0 : (g->nparams + g->nlocals) * 16;
          if (getenv("SCRIP_N2_FT_PROBE")) fprintf(stderr, "[N2-PREPASS] proc=%-14s np=%-3d nl=%-3d zframeA=%d icncells=%d rg=%-5d fp=%-5d ft=%d\n",
                                                  pn, g->nparams, g->nlocals, _zfA, g->icn_cells_graph, g->jcon_value_region, _fp, ((32 + g->jcon_value_region + 15) & ~15) + _fp + 16);
          emit_patzeta_register(pn, g->jcon_value_region, _fp, 0); } } }   /* ⛔⛔ fp term = (np+nl)*16, MIRRORING the callee alpha's own frame_total = flat_frame_bytes + (np+nl)*16 (ceo s283d): the previous zls_g_fp_total(g) read 0 for every Icon generator (fct_fp_range knows zls FIELDS, not Icon params/locals), so the registry undersold frame_total by 16 per param+local -- the caller's landing then read the yielded value 16 low ([rdx-288] vs the suspend's write at [rbp-304], MEASURED on the w1 concat witness: every suspended value from a generator WITH a local arrived EMPTY -- seat15's concord one-empty-key table, seat02's garbage suspend-n) and the host reservation was one slot too small (silent slice overflow). Every proof witness to date had np=nl=0, where 0 was the right answer -- plausible-zero instance SIX on this rung. The reserve consumer's align16(32+fb)+fp+16 algebra equals flat_frame_bytes+fp exactly, so with this term the registry IS frame_total. ⛔ The _zfA arm is NOT a tuning knob -- see the zframe paragraph in the block comment above for why the key is the α-SELECTION STATE and never the bare field. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_zf_exit_γ(void) { exit(0); }
static void icn_zf_exit_ω(void) { exit(1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_zf_main_call(void *fn, void *mf, void *wire_γ, void *wire_ω) {
    __asm__ volatile(
        "push %%r12\n\t"
        "sub $8, %%rsp\n\t"
        "mov $0x70000000, %%r12\n\t"
        "mov (%%r12), %%r12\n\t"
        "xor %%esi, %%esi\n\t"
        "xor %%r14d, %%r14d\n\t"
        "jmp *%%rax\n\t"
        :
        : "a"(fn), "D"(mf), "c"(wire_γ), "d"(wire_ω)
        : "memory", "rsi", "r8", "r9", "r10", "r11"
    );
}
extern DESCR_t      eval_expr(const char *src);
extern int exec_stmt(const char *subj_name,
                          DESCR_t    *subj_var,
                          DESCR_t     pat,
                          DESCR_t    *repl,
                          int         has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
#include "../runtime/builtins/gen_runtime.h"
#include "../runtime/builtins/resolution.h"
#include "driver/polyglot.h"
#include "../tools/emit_per_kind_audit.h"
#include "../ir/zeta_choices.h"
#include "../runtime/rt/zeta_alloc.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scrip_symmap(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_SYMMAP"); v = e ? (atoi(e) != 0) : 0; } return v; }
static int proc_role3_kind(const IR_graph_t *g) { if (!g || !g->entry) return 0; if (g->entry->op == IR_GOTO_DEFERRED) return 1;  const IR_t *e = (g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) ? g->entry : (const IR_t *)0; return !e ? 0 : (e->γ.node && e->γ.node->op == IR_GOTO_DEFERRED) ? 1 : 2; }
static const char *asm_sym_name(const char *nm) { extern const char * bb_ab_sym_name(const char *); return bb_ab_sym_name(nm); }
static const char * ir_define_plain_name(const IR_t * nd) { return (nd && nd->op == IR_DEFINE && !ir_define_sr_citizen(nd)) ? IR_LIT(nd).sval : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int keyword_supported(const char *kw) {
    if (!kw) return 0;
    if (kw[0] == '&') kw++;
    return !strcmp(kw, "subject") || !strcmp(kw, "pos") || !strcmp(kw, "null") || !strcmp(kw, "fail");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_safe_kind(IR_e t) {
    return t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_INTEGER || t == IR_LIT_STRING || t == IR_LIT_REAL || t == IR_OP_COUNT ||
           t == IR_VAR || t == IR_KW_ICON || t == IR_KW_SNOBOL4 || t == IR_OP_COUNT || t == IR_CALL || ir_is_scan_kind(t) || t == IR_BINOP
        || t == IR_OP_COUNT || t == IR_CONJUNCTION || t == IR_ASSIGN || t == IR_OP_COUNT || t == IR_OP_COUNT || t == IR_OP_COUNT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sg_var_assigned(IR_graph_t *sg, const char *name) {
    if (!sg || !sg->all || !name) return 0;
    for (int i = 0; i < sg->n; i++) { IR_t *m = sg->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *scan_lit_entry(IR_t *nd, IR_e want) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != want) return (IR_t *)0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return (IR_t *)0;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_fn_lit_arg(IR_t *nd, IR_e want) {
    return scan_lit_entry(nd, want) != (IR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_fn_cset_arg(IR_t *nd) {
    extern const char *kw_cset_const_str(const char *kw);
    if (scan_lit_entry(nd, IR_LIT_STRING) != (IR_t *)0) return 1;
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || (ae->op != IR_KW_ICON && ae->op != IR_KW_SNOBOL4)) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return kw_cset_const_str(IR_LIT(ae).sval) != (const char *)0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_any_cset_var_ok(stage2_t *s2, int gi, IR_graph_t *g, IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != IR_VAR || !IR_LIT(ae).sval || IR_LIT(ae).sval[0] == '&') return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return graph_var_assigned_or_param(s2, gi, g, IR_LIT(ae).sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_tab_arg_ok(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    if (ae->op == IR_LIT_INTEGER && IR_LIT(ae).ival >= 1) return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval
        && (!strcmp(IR_LIT(ae).sval, "any") || !strcmp(IR_LIT(ae).sval, "many") || !strcmp(IR_LIT(ae).sval, "upto")) && scan_fn_cset_arg(ae))
        return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval
        && (!strcmp(IR_LIT(ae).sval, "match") || !strcmp(IR_LIT(ae).sval, "find") || !strcmp(IR_LIT(ae).sval, "bal")) && scan_fn_lit_arg(ae, IR_LIT_STRING))
        return 1;
    return 0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_subgraph_safe(stage2_t *s2, int gi, IR_graph_t *g, IR_graph_t *sg, int depth) {
    if (!sg || !sg->all || sg->n <= 0 || depth > 16) return 0;
    for (int i = 0; i < sg->n; i++) {
        IR_t *nd = sg->all[i];
        if (!nd) continue;
        if (!scan_safe_kind(nd->op)) return 0;
        if (nd->op == IR_VAR) {
            if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { if (!keyword_supported(IR_LIT(nd).sval)) return 0; }
            else if (!IR_LIT(nd).sval || (!graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval) && !sg_var_assigned(sg, IR_LIT(nd).sval))) return 0;
        }
        if (nd->op == IR_ASSIGN) { if (!IR_LIT(nd).sval || is_global(IR_LIT(nd).sval)) return 0; }
        if ((nd->op == IR_KW_ICON || nd->op == IR_KW_SNOBOL4) && !keyword_supported(IR_LIT(nd).sval)) return 0;
        if (nd->op == IR_CALL || ir_is_scan_kind(nd->op)) {
            if (!IR_LIT(nd).sval) return 0;
            if (!strcmp(IR_LIT(nd).sval, "any")) { if (!(IR_LIT(nd).dval == 3.0 && (scan_fn_cset_arg(nd) || scan_any_cset_var_ok(s2, gi, g, nd)))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "many") || !strcmp(IR_LIT(nd).sval, "upto")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_cset_arg(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "match")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_STRING))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "tab")) { if (!(IR_LIT(nd).dval == 3.0 && scan_tab_arg_ok(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "move")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_INTEGER))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "pos")) { IR_t *pe = scan_lit_entry(nd, IR_LIT_INTEGER); if (!(IR_LIT(nd).dval == 3.0 && pe && IR_LIT(pe).ival >= 1)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "find")) {
                IR_t *fe = scan_lit_entry(nd, IR_LIT_STRING); if (!(IR_LIT(nd).dval == 3.0 && fe && IR_LIT(fe).sval && IR_LIT(fe).sval[0] && strlen(IR_LIT(fe).sval) <= 32)) return 0;
            }
            else if (!strcmp(IR_LIT(nd).sval, "bal")) {
                IR_t *be = scan_lit_entry(nd, IR_LIT_STRING);
                if (!(IR_LIT(nd).dval == 3.0 && be && IR_LIT(be).sval && IR_LIT(be).sval[0] && !strchr(IR_LIT(be).sval, 40) && !strchr(IR_LIT(be).sval, 41))) return 0;
            }
            else if (!(!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes"))) return 0;
        }
        if (nd->op == IR_BINOP) { int64_t bc = IR_LIT(nd).ival; int is_rel = (bc >= BINOP_LT && bc <= BINOP_NE) || (bc >= BINOP_SLT && bc <= BINOP_SNE) || bc == BINOP_EQV || bc == BINOP_NEQV; if (!binop_is_concat((long)bc) && !is_rel) return 0; }
        if (nd->op == IR_OP_COUNT) {
            IR_graph_t *ssg = (IR_graph_t *)0;
            IR_graph_t *bsg = (IR_graph_t *) 0;
            if (!scan_subgraph_safe(s2, gi, g, ssg, depth + 1) || !scan_subgraph_safe(s2, gi, g, bsg, depth + 1)) return 0;
        }
    }
    return 1;
}
static int graph_native_emittable_mode(stage2_t *s2, int for_run, char *why, size_t whysz);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_native_emittable(stage2_t *s2, char *why, size_t whysz) { return graph_native_emittable_mode(s2, 0, why, whysz); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_scan_body_slotful(IR_t *r) {
    if (!r || r->op != IR_OP_COUNT || IR_LIT(r).dval != 1.0) return 0;
    IR_graph_t *bsg = (IR_graph_t *) 0;
    IR_t *bt = bsg ? bsg->entry : (IR_t *)0;
    int gd = 0;
    while (bt && bt->γ.node && bt->γ.node->op != IR_SUCCEED && bt->γ.node->op != IR_FAIL && gd++ < 512) bt = bt->γ.node;
    if (bt && (bt->op == IR_LIT_INTEGER || bt->op == IR_LIT_STRING)) return 1;
    if (bt && bt->op == IR_VAR && IR_LIT(bt).sval && IR_LIT(bt).sval[0] != '&') return 1;
    if (bt && (bt->op == IR_CALL || ir_is_scan_kind(bt->op)) && IR_LIT(bt).dval == 3.0 && IR_LIT(bt).sval
        && (!strcmp(IR_LIT(bt).sval, "tab") || !strcmp(IR_LIT(bt).sval, "move") || !strcmp(IR_LIT(bt).sval, "pos") || !strcmp(IR_LIT(bt).sval, "any")
            || !strcmp(IR_LIT(bt).sval, "match") || !strcmp(IR_LIT(bt).sval, "many") || !strcmp(IR_LIT(bt).sval, "upto") || !strcmp(IR_LIT(bt).sval, "find") || !strcmp(IR_LIT(bt).sval, "bal")))
        return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rhs_kind_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_OP_COUNT || r->op == IR_LIT_REAL) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && !strcmp(IR_LIT(r).sval, "&null")) return 1;
    if (r->op == IR_BINOP_RELOP_VAL) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL
                               || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD || binop_is_concat((long)IR_LIT(r).ival)))
        return 1;
    if (ir_norm_call_kind(r->op) == IR_CALL || r->op == IR_UNOP || r->op == IR_FIELD_GET || r->op == IR_PROC_GEN) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 0.0) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 1.0) return 1;
    { extern void *dat_find_type(const char *name); if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && IR_LIT(r).sval && dat_find_type(IR_LIT(r).sval)) return 1; }
    { extern int rt_builtin_is_known(const char *name); const char *bn = IR_LIT(r).sval; if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && bn && rt_builtin_is_known(bn)) return 1; }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_TO || r->op == IR_TO_BY || r->op == IR_PROC_GEN || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_MAKE_LIST) return 1;
    if (r->op == IR_CONJUNCTION) { IR_t *lv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return lv ? rhs_kind_ok(lv) : 0; }
    {
        extern int is_global(const char *);
        if (r->op == IR_ASSIGN && IR_LIT(r).sval && !is_global(IR_LIT(r).sval)) {
            IR_t *rv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return rv ? rhs_kind_ok(rv) : 0;
        }
    }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 2.0 && !(IR_LIT(r).sval && (!strcmp(IR_LIT(r).sval,"__rk_bool")||!strcmp(IR_LIT(r).sval,"__rk_try")))) return 1;
    if (r->op == IR_OP_COUNT) { int64_t u = IR_LIT(r).ival; if (u == TT_MNS || u == TT_PLS || u == TT_SIZE || u == TT_NONNULL || u == TT_NULL || u == TT_NOT) return 1; }
    if (r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return gen_scan_body_slotful(r);
    return 0;
}
static int graph_has_binop(const IR_graph_t *g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int local_assign_rhs_ok_g(const IR_graph_t *g, IR_t *nd) {
    IR_t *rhs = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    if (!rhs) for (int i = 0; i < g->n; i++) { IR_t *p = g->all[i]; if (p && p->γ.node == nd) { rhs = p; break; } }
    if (rhs && rhs->op == IR_LIT_REAL) return 1;
    return rhs_kind_ok(rhs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_operand_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_jct_call(IR_t *r) {
    return r && r->op == IR_CALL && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_marshallable(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_CALL && (IR_LIT(r).dval == 2.0 || IR_LIT(r).dval == 3.0 || IR_LIT(r).dval == 5.0)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bool_truthy_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval,"__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond || !cond->entry) return 0;
    IR_t *e = cond->entry;
    return (e->op == IR_LIT_INTEGER || e->op == IR_LIT_STRING || (e->op == IR_VAR && IR_LIT(e).sval && IR_LIT(e).sval[0] != '&'));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bool_cond_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond) return 0;
    IR_t *p = cond->entry; IR_t *rel = (IR_t *)0; int gd = 0;
    while (p && gd++ < 256) { if (p->op == IR_BINOP && IR_LIT(p).ival >= BINOP_LT && IR_LIT(p).ival <= BINOP_NE) { rel = p; break; } if (!p->γ.node) break; p = p->γ.node; }
    if (!rel) return 0;
    IR_t *ra = ir_pair_arg(rel, 0); IR_t *rb = ir_pair_arg(rel, 1);
    if ((is_jct_call(ra) || is_jct_call(rb)) && jct_marshallable(ra) && jct_marshallable(rb)) return 1;
    return arith_operand_ok(ra) && arith_operand_ok(rb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_has_local_assign(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) {
        IR_t *nd = g->all[ni];
        if (nd && nd->op == IR_ASSIGN && IR_LIT(nd).sval && !is_global(IR_LIT(nd).sval)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_has_binop(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->op == IR_BINOP) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name) {
    for (int i = 0; i < g->n; i++) { IR_t *m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    for (int p = 0; p < s2->proc_count; p++) {
        if (s2->proc_table[p].bb_idx != gi) continue;
        for (int k = 0; k < s2->proc_table[p].nparams && k < s2->proc_table[p].lower_sc.n; k++)
            if (s2->proc_table[p].lower_sc.e[k].name && !strcmp(s2->proc_table[p].lower_sc.e[k].name, name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_native_emittable_mode(stage2_t *s2, int for_run, char *why, size_t whysz) {
    /* ⛔⭐ THE REASON IS REPORTED, NEVER GUESSED (hq_B 2026-09-01, row bench-grids-rebase-to-two-number-basis). Until this edit both callers printed a HARDCODED
       "(a box has no MEDIUM_BINARY arm — Raku map/grep)" for every 0 this function returned -- measured live on `my $t = now;`, a program with no map, no grep and
       no box at all: it was refused by the IR_VAR check below because `now` is read and never assigned, and the message blamed a construct the program did not
       contain. A refusal that names the wrong cause costs more than no message: the reader goes looking for map/grep. Each return 0 now writes its own reason into
       the caller's buffer and the two fprintf sites print that. (Raku's clock term `now` is still not a SCRIP builtin; the bench kernels use wall_us() instead.)
       ⛔⛔ IR_OP_COUNT IS A LIVE OP, NOT A TOMBSTONE -- AND THIS EDIT LEARNED THAT THE EXPENSIVE WAY (hq_B, same day, one hour later). The first draft of
       this rewrite deleted the six `nd->op == IR_OP_COUNT` guards as dead code, on a census that asked "is IR_OP_COUNT ever ASSIGNED with `=`" (zero hits)
       and read the answer as "does any node ever carry it". lower_raku.c:82 rk_excise() and lower_prolog.c:461/773/877 BUILD nodes with op IR_OP_COUNT --
       the excised/placeholder node, printed as UNKNOWN by the IR dump -- and the templates (bb_assign_local, bb_unop_gvar_slot, ...) test for it. With the
       guards gone, `reverse(@a)` sailed through this gate and died in emit_drive as "IR op=133 has no template", SIGABRT, in both modes: two smoke programs
       went from REFUSED (a clean [SMX] banner, which the smoke's PASS-or-REFUSED bar accepts) to a core dump. The old body's line 459 refused ANY such node
       unconditionally and shadowed the later IR_OP_COUNT branches, so ONE named guard below reproduces the original semantics exactly, and now says why.
       ⭐ THE REAL BEAUTY DEFECT is that the enum's COUNT sentinel doubles as an op: a reader who knows what a count sentinel is will call these guards dead,
       as I did. Naming the excised node (IR_EXCISED) is row driver-emittability-predicates-sentinel-tombstones, re-scoped to that. */
    extern int rt_builtin_is_known(const char *name);
    (void)for_run;
    if (!s2) { snprintf(why, whysz, "no stage2 graph"); return 0; }
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (!nd) continue;
            if (nd->op == IR_OP_COUNT)
                { snprintf(why, whysz, "an excised node (op IR_OP_COUNT, dumped as UNKNOWN; rk_excise) has no native template%s%s", IR_LIT(nd).sval ? " -- " : "", IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return 0; }
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval,"__rk_bool") && strcmp(IR_LIT(nd).sval,"__rk_try") && !rt_builtin_is_known(IR_LIT(nd).sval))
                { snprintf(why, whysz, "call '%s' is neither a user sub nor a known builtin", IR_LIT(nd).sval); return 0; }
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"__rk_bool")||!strcmp(IR_LIT(nd).sval,"__rk_try"))) {
                if (bool_cond_emittable(nd)||bool_truthy_emittable(nd)) {} else { snprintf(why, whysz, "%s condition shape has no native arm", IR_LIT(nd).sval); return 0; }
            }
            if (nd->op == IR_VAR && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' && !is_global(IR_LIT(nd).sval) && !graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval))
                { snprintf(why, whysz, "variable '%s' is read but never assigned and is not a parameter", IR_LIT(nd).sval); return 0; }
            if (nd->op == IR_ASSIGN && IR_LIT(nd).sval && !is_global(IR_LIT(nd).sval) && !local_assign_rhs_ok_g(g, nd))
                { snprintf(why, whysz, "assignment to '%s' has an rhs shape with no native arm", IR_LIT(nd).sval); return 0; }
        }
    }
    why[0] = '\0';
    return 1;
}
static IR_graph_t **g_gz_visiting = NULL; static int g_gz_nvisiting = 0; static int g_gz_visiting_cap = 0;
static IR_graph_t **g_gz_det_visiting = NULL; static int g_gz_det_nvisiting = 0; static int g_gz_det_visiting_cap = 0;
static int g_gz_no_struct_ptr = 0;
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
static int    g_prog_argc = 0;
static char **g_prog_argv = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_slots_all(stage2_t * s2) {
    extern void ir_drive_slot_assign(IR_graph_t * g);
    for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) ir_drive_slot_assign(s2->bbp.table[_gi]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long parse_mem_arg(const char *s) {
    char *end = NULL; errno = 0; long v = strtol(s, &end, 10); if (errno || end == s || v < 0) return -1;
    if (*end == 'k' || *end == 'K') { v *= 1024L; end++; } else if (*end == 'm' || *end == 'M') { v *= 1024L * 1024L; end++; }
    return (*end == '\0') ? v : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int apply_stack_limit(long bytes) {
    struct rlimit rl; if (getrlimit(RLIMIT_STACK, &rl) != 0) return -1;
    if (rl.rlim_max != RLIM_INFINITY && (rlim_t)bytes > rl.rlim_max) bytes = (long)rl.rlim_max;
    if (rl.rlim_cur != RLIM_INFINITY && (rlim_t)bytes <= rl.rlim_cur) return 0;
    rl.rlim_cur = (rlim_t)bytes; return setrlimit(RLIMIT_STACK, &rl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_str(FILE * fp, const char * s) {
    fputc('"', fp);
    for (const unsigned char * p = (const unsigned char *) (s ? s : ""); *p; p++) {
        if (*p == '"' || *p == '\\') { fputc('\\', fp); fputc(*p, fp); }
        else if (*p == '\n') fputs("\\n", fp);
        else if (*p == '\t') fputs("\\t", fp);
        else if (*p == '\r') fputs("\\r", fp);
        else if (*p < 0x20) fprintf(fp, "\\u%04x", *p);
        else fputc(*p, fp);
    }
    fputc('"', fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_label(FILE * fp, const IR_t * bb) {
    switch (bb->op) {
        case IR_LIT_INTEGER: fprintf(fp, ",\"label\":\"%lld\"", (long long) IR_LIT(bb).ival); break;
        case IR_LIT_REAL: fprintf(fp, ",\"label\":\"%g\"", IR_LIT(bb).dval); break;
        case IR_LIT_STRING: case IR_LIT_CHARSET: case IR_LIT_NAME: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_VAR: case IR_ASSIGN: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_KW_ICON: case IR_KW_ICON_GEN: case IR_KW_SNOBOL4: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_BINOP: case IR_BINOP_TEST: fprintf(fp, ",\"label\":\"op%lld\"", (long long) IR_LIT(bb).ival); break;
        case IR_CALL: case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_ICON: case IR_CALL_SNOBOL4: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbj_index(const IR_graph_t * g, const IR_t * nd) {
    for (int i = 0; i < g->n; i++) if (g->all[i] == nd) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_edge(FILE * fp, int * first, int gi, int i, const char * fp_name, const IR_graph_t * g, const IR_ref_t * r) {
    if (!r->node) return;
    int j = bbj_index(g, r->node);
    if (j < 0) return;
    const char * tp = (r->sz[0] && strcmp(r->sz, "β") == 0) ? "beta" : "alpha";
    fprintf(fp, "%s\n  {\"from\":\"b%d_%d\",\"fp\":\"%s\",\"to\":\"b%d_%d\",\"tp\":\"%s\"}", *first ? "" : ",", gi, i, fp_name, gi, j, tp);
    *first = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void m3_seal_entry_cells(const char *pname, void *fnbase, int alpha_face) {
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    bb_ab_seal_entry_cells(pname, fnbase, alpha_face);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_module_init_bottom(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_MODULE_INIT"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sn4_m4_alpha_seal(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_M4_ALPHA_SEAL"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_module_init_body(stage2_t *s2, const char **proc_names_buf, int *proc_nparams_buf, int *proc_pidx_buf, int *proc_fb_buf, int *proc_ispat_buf, int *proc_zstatic_buf, int n_procs, int n_cls_emit, int n_gram_emit, int is_raku, const char *mi_name) {
    if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0) {
        emit_textf("%s:\n", mi_name);
        emit_textf("  sub rsp, 8\n");
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              emit_textf("  .section .rodata\n");
              emit_textf("  .Lclassspec%d: .string \"%s(", ci, cn);
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) { if (fj) emit_textf(","); emit_textf("%s", dat_type_field(ci, fj)); }
              emit_textf(")\"\n");
              emit_textf("  .section .text\n  .intel_syntax noprefix\n");
              emit_textf("  lea rdi, [rip + .Lclassspec%d]\n", ci);
              emit_textf("  call record_register@PLT\n");
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nparents(int); extern const char *dat_type_parent_at(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              int np = dat_type_nparents(ci); if (np <= 0) continue;
              emit_textf("  .section .rodata\n");
              emit_textf("  .Lclschild%d: .string \"%s\"\n", ci, cn);
              for (int pj = 0; pj < np; pj++) emit_textf("  .Lclsp%d_%d: .string \"%s\"\n", ci, pj, dat_type_parent_at(ci, pj));
              emit_textf("  .balign 8\n  .Lclsparr%d:\n", ci);
              for (int pj = 0; pj < np; pj++) emit_textf("  .quad .Lclsp%d_%d\n", ci, pj);
              emit_textf("  .section .text\n  .intel_syntax noprefix\n");
              emit_textf("  lea rdi, [rip + .Lclschild%d]\n", ci);
              emit_textf("  lea rsi, [rip + .Lclsparr%d]\n", ci);
              emit_textf("  mov rdx, %d\n", np);
              emit_textf("  call class_inherit_multi@PLT\n");
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          extern int dat_type_field_has_default(int, int); extern DESCR_t dat_type_field_default(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                  if (!dat_type_field_has_default(ci, fj)) continue;
                  const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                  DESCR_t dv = dat_type_field_default(ci, fj);
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Ldefcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Ldeffld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  if (dv.v == DT_S) {
                      const char *sv = dv.s ? dv.s : ""; emit_textf("  .Ldefstr%d_%d: .byte ", ci, fj); for (const char *p = sv; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p);
                      emit_textf("0\n");
                  }
                  else if (dv.v == DT_R) { union { double d; unsigned long long q; } u; u.d = dv.r; emit_textf("  .Ldefdbl%d_%d: .quad %llu\n", ci, fj, u.q); }
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Ldefcls%d_%d]\n", ci, fj);
                  emit_textf("  lea rsi, [rip + .Ldeffld%d_%d]\n", ci, fj);
                  if (dv.v == DT_S) { emit_textf("  lea rdx, [rip + .Ldefstr%d_%d]\n", ci, fj); emit_textf("  call dat_set_field_default_s@PLT\n"); }
                  else if (dv.v == DT_R) { emit_textf("  movsd xmm0, qword ptr [rip + .Ldefdbl%d_%d]\n", ci, fj); emit_textf("  call dat_set_field_default_r@PLT\n"); }
                  else { emit_textf("  mov rdx, %lld\n", (long long)dv.i); emit_textf("  call dat_set_field_default_i@PLT\n"); }
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          extern int dat_type_field_required(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                  if (!dat_type_field_required(ci, fj)) continue;
                  const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lreqcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lreqfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lreqcls%d_%d]\n", ci, fj);
                  emit_textf("  lea rsi, [rip + .Lreqfld%d_%d]\n", ci, fj);
                  emit_textf("  call dat_set_field_required@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          extern int dat_type_field_rw(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                  if (!dat_type_field_rw(ci, fj)) continue;
                  const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lrwcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lrwfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lrwcls%d_%d]\n", ci, fj);
                  emit_textf("  lea rsi, [rip + .Lrwfld%d_%d]\n", ci, fj);
                  emit_textf("  call dat_set_field_rw@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          extern int dat_type_field_sigil(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                  int sg = dat_type_field_sigil(ci, fj); if (sg != '@' && sg != '%') continue;
                  const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lsigcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lsigfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lsigcls%d_%d]\n", ci, fj);
                  emit_textf("  lea rsi, [rip + .Lsigfld%d_%d]\n", ci, fj);
                  emit_textf("  mov rdx, %d\n", sg);
                  emit_textf("  call dat_set_field_sigil@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
          extern int dat_type_field_priv(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                  if (!dat_type_field_priv(ci, fj)) continue;
                  const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lprvcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lprvfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lprvcls%d_%d]\n", ci, fj);
                  emit_textf("  lea rsi, [rip + .Lprvfld%d_%d]\n", ci, fj);
                  emit_textf("  call dat_set_field_priv@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nmethods(int); extern const char *dat_type_method_at(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              int nm = dat_type_nmethods(ci); if (nm <= 0) continue;
              for (int mj = 0; mj < nm; mj++) {
                  const char *mn = dat_type_method_at(ci, mj); if (!mn || !*mn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lmethcls%d_%d: .byte ", ci, mj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lmethnm%d_%d: .byte ", ci, mj); for (const char *p = mn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lmethcls%d_%d]\n", ci, mj);
                  emit_textf("  lea rsi, [rip + .Lmethnm%d_%d]\n", ci, mj);
                  emit_textf("  call dat_add_method@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_has_build(int);
          extern int dat_type_nbuild_keys(int); extern const char *dat_type_build_key_at(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              if (!dat_type_has_build(ci)) continue;
              emit_textf("  .section .rodata\n");
              emit_textf("  .Lbldcls%d: .byte ", ci); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
              emit_textf("  .Lbldnull%d: .byte 0\n", ci);
              emit_textf("  .section .text\n  .intel_syntax noprefix\n");
              emit_textf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
              emit_textf("  lea rsi, [rip + .Lbldnull%d]\n", ci);
              emit_textf("  call dat_set_build_key@PLT\n");
              int nk = dat_type_nbuild_keys(ci);
              for (int kj = 0; kj < nk; kj++) {
                  const char *kn = dat_type_build_key_at(ci, kj); if (!kn || !*kn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lbldkey%d_%d: .byte ", ci, kj); for (const char *p = kn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
                  emit_textf("  lea rsi, [rip + .Lbldkey%d_%d]\n", ci, kj);
                  emit_textf("  call dat_set_build_key@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nhandles(int);
          extern const char *dat_type_handles_meth_at(int, int); extern const char *dat_type_handles_fld_at(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              int nh = dat_type_nhandles(ci); if (nh <= 0) continue;
              for (int hj = 0; hj < nh; hj++) {
                  const char *hm = dat_type_handles_meth_at(ci, hj); const char *hf = dat_type_handles_fld_at(ci, hj);
                  if (!hm || !*hm || !hf || !*hf) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lhndcls%d_%d: .byte ", ci, hj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lhndmeth%d_%d: .byte ", ci, hj); for (const char *p = hm; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lhndfld%d_%d: .byte ", ci, hj); for (const char *p = hf; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lhndcls%d_%d]\n", ci, hj);
                  emit_textf("  lea rsi, [rip + .Lhndmeth%d_%d]\n", ci, hj);
                  emit_textf("  lea rdx, [rip + .Lhndfld%d_%d]\n", ci, hj);
                  emit_textf("  call dat_add_handles@PLT\n");
              }
          } }
        { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nroles(int); extern const char *dat_type_role_at(int, int);
          int n_cls = dat_type_count();
          for (int ci = 0; ci < n_cls; ci++) {
              const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
              int nr = dat_type_nroles(ci); if (nr <= 0) continue;
              for (int rj = 0; rj < nr; rj++) {
                  const char *rn = dat_type_role_at(ci, rj); if (!rn || !*rn) continue;
                  emit_textf("  .section .rodata\n");
                  emit_textf("  .Lrolechild%d_%d: .byte ", ci, rj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .Lrolename%d_%d: .byte ", ci, rj); for (const char *p = rn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                  emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                  emit_textf("  lea rdi, [rip + .Lrolechild%d_%d]\n", ci, rj);
                  emit_textf("  lea rsi, [rip + .Lrolename%d_%d]\n", ci, rj);
                  emit_textf("  call class_compose_role@PLT\n");
              }
          } }
        { extern int rt_grammar_count(void); extern const char *rt_grammar_qname(int); extern const char *rt_grammar_body(int); extern int rt_grammar_flavor(int);
          int n_gram = rt_grammar_count();
          for (int gi = 0; gi < n_gram; gi++) {
              const char *qn = rt_grammar_qname(gi); const char *bd = rt_grammar_body(gi);
              if (!qn || !bd) continue;
              emit_textf("  .section .rodata\n");
              emit_textf("  .Lgramqn%d: .byte ", gi); for (const char *p = qn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
              emit_textf("  .Lgrambd%d: .byte ", gi); for (const char *p = bd; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
              emit_textf("  .section .text\n  .intel_syntax noprefix\n");
              emit_textf("  lea rdi, [rip + .Lgramqn%d]\n", gi);
              emit_textf("  lea rsi, [rip + .Lgrambd%d]\n", gi);
              emit_textf("  mov edx, %d\n", rt_grammar_flavor(gi));
              emit_textf("  call rt_grammar_register@PLT\n");
          } }
        for (int i = 0; i < n_procs; i++) {
            ProcEntry *pe = &s2->proc_table[proc_pidx_buf[i]];
            if (pe->dyn_scope && proc_role3_kind((pe->bb_idx >= 0 && pe->bb_idx < s2->bbp.count) ? s2->bbp.table[pe->bb_idx] : (IR_graph_t *)0) != 2) { if (sn4_m4_alpha_seal() && pe->name && strncmp(pe->name, "LBL__", 5) != 0 && !strchr(pe->name, '$')) { emit_textf("  .section .rodata\n  .Lseala%d: .string \"%s\"\n  .section .text\n  .intel_syntax noprefix\n", i, proc_names_buf[i]); emit_textf("  .weak %s_\xce\xb1\n  lea rdi, [rip + .Lseala%d]\n  mov rsi, qword ptr [rip + %s_\xce\xb1@GOTPCREL]\n  call rt_proc_seal_alpha@PLT\n", asm_sym_name(proc_names_buf[i]), i, asm_sym_name(proc_names_buf[i])); } continue; }
            if (pe->name && strncmp(pe->name, "LBL__", 5) == 0) { int _dfl = 0; for (int _z = 0; _z < s2->proc_count; _z++) { ProcEntry *_dr = &s2->proc_table[_z]; if (!_dr->name || strncmp(_dr->name, "LBL__", 5) == 0 || !_dr->dyn_scope) continue; IR_t *_dn = bb_proc_entry(_dr); if (!_dn) continue; int _dg = 0; while (_dn && (_dn->op == IR_SUCCEED || _dn->op == IR_FAIL || _dn->op == IR_GOTO) && _dn->γ.node && _dg++ < 64) _dn = _dn->γ.node; IR_t *_dd = (_dn && _dn->op == IR_DEFINE && ir_define_sr_citizen(_dn)) ? _dn->γ.node : _dn; if (!_dd || _dd->op != IR_GOTO_DEFERRED || !IR_LIT(_dd).sval) continue; const char *_de = IR_LIT(_dd).sval; if (strncmp(_de, "LBL__", 5) == 0) _de += 5; if (!strcmp(_de, pe->name + 5)) { _dfl = 1; break; } } if (_dfl) continue; }
            { static int _onereg = -1; if (_onereg < 0) { const char *_e = getenv("SCRIP_ONE_REG"); _onereg = (_e && *_e == '0') ? 0 : 1; }
            if (_onereg) {
                extern int rt_pl_dc_ok(const char *, int); int _dc = (!proc_ispat_buf[i] && rt_pl_dc_ok(proc_names_buf[i], proc_nparams_buf[i]));
                int _pin = proc_pidx_buf[i]; int _nf = (_pin >= 0 && _pin < s2->proc_count) ? s2->proc_table[_pin].nformals : 0;
                int _rkflags = (pe->dyn_scope ? 1 : 0) | ((proc_ispat_buf[i] && proc_zstatic_buf[i]) ? 2 : 0) | (pe->is_variadic ? 4 : 0) | (pe->is_generator ? 8 : 0) | ((strncmp(proc_names_buf[i], "gram__", 6) != 0) ? 16 : 0);
                int _rkulex = (is_raku && !pe->dyn_scope && pe->nparams > 0 && pe->lower_sc.n > 0);
                emit_textf("  .section .rodata\n");
                emit_textf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
                if (pe->dyn_scope) {
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .Lstartup_pp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                    emit_textf("  .align 8\n  .Lstartup_pnames%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .quad .Lstartup_pp%d_%d\n", i, k);
                    emit_textf("  .quad 0\n");
                }
                if (_rkulex) {
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) if (pe->lower_sc.e[k].name) emit_textf("  .Lstartup_qp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name);
                    emit_textf("  .align 8\n  .Lstartup_qparr%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) { if (pe->lower_sc.e[k].name) emit_textf("  .quad .Lstartup_qp%d_%d\n", i, k); else emit_textf("  .quad 0\n"); }
                    emit_textf("  .quad 0\n");
                }
                if (pe->dyn_scope && pe->result_name && strcmp(pe->result_name, pe->name)) emit_textf("  .Lstartup_prn%d: .string \"%s\"\n", i, pe->result_name);
                emit_textf("  .align 8\n  .Lstartup_prec%d:\n", i);
                emit_textf("  .quad .Lstartup_pname%d\n", i);
                if (strncmp(proc_names_buf[i], "LBL__", 5) == 0) emit_textf("  .quad LBL__%s\n", asm_sym_name(proc_names_buf[i] + 5)); else emit_textf("  .quad FN__%s\n", asm_sym_name(proc_names_buf[i]));
                if (_dc) emit_textf("  .quad %s_dc\xce\xb1\n", asm_sym_name(proc_names_buf[i])); else emit_textf("  .quad 0\n");
                if (pe->dyn_scope && pe->result_name && strcmp(pe->result_name, pe->name)) emit_textf("  .quad .Lstartup_prn%d\n", i); else emit_textf("  .quad 0\n");
                if (pe->dyn_scope) emit_textf("  .quad .Lstartup_pnames%d\n", i); else if (_rkulex) emit_textf("  .quad .Lstartup_qparr%d\n", i); else emit_textf("  .quad 0\n");
                emit_textf("  .long %d\n  .long %d\n  .long %d\n  .long %d\n  .long %d\n  .long %d\n", proc_nparams_buf[i], _nf, proc_fb_buf[i], _rkflags, pe->rest_kind, pe->named_rest);
                emit_textf("  .section .text\n");
                emit_textf("  .intel_syntax noprefix\n");
                emit_textf("  lea rdi, [rip + .Lstartup_prec%d]\n", i);
                emit_textf("  call rt_proc_register_rec@PLT\n");
                { extern int emit_icn_n2_gen_region_ft(const char *, int, IR_graph_t *); int _gpi = proc_pidx_buf[i]; int _gft2 = 0;   /* N-2 apply-call cure (ceo s283h): the ONE_REG default lane registers via the compact record, which has no gen_region_ft field -- stamp it with one extra setter call so mode-4 binaries know the region-resident alphas too (the per-setter stamp below sits in the legacy _onereg==0 lane and never runs by default) */
                  if (_gpi >= 0 && _gpi < s2->proc_count) { int _ggi = s2->proc_table[_gpi].bb_idx; if (_ggi >= 0 && _ggi < s2->bbp.count && s2->bbp.table[_ggi]) _gft2 = emit_icn_n2_gen_region_ft(proc_names_buf[i], s2->proc_table[_gpi].is_generator, s2->bbp.table[_ggi]); }
                  if (_gft2 > 0) {
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  mov esi, %d\n", _gft2);
                    emit_textf("  call rt_proc_set_gen_region_ft@PLT\n");
                  } }
            } else {
            emit_textf("  .section .rodata\n");
            emit_textf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
            if (pe->dyn_scope) {
                for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                    emit_textf("  .Lstartup_pp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                emit_textf("  .align 8\n  .Lstartup_pnames%d:\n", i);
                for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .quad .Lstartup_pp%d_%d\n", i, k);
                emit_textf("  .quad 0\n");
            }
            emit_textf("  .section .text\n");
            emit_textf("  .intel_syntax noprefix\n");
            if (pe->dyn_scope) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  lea rsi, [rip + .Lstartup_pnames%d]\n", i);
                emit_textf("  mov edx, %d\n", proc_nparams_buf[i]);
                emit_textf("  call rt_proc_register@PLT\n");
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, 1\n");
                emit_textf("  call rt_proc_set_dyn_scope@PLT\n");
                if (pe->result_name && strcmp(pe->result_name, pe->name)) {
                    emit_textf("  .section .rodata\n  .Lstartup_prn%d: .string \"%s\"\n  .section .text\n  .intel_syntax noprefix\n", i, pe->result_name);
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  lea rsi, [rip + .Lstartup_prn%d]\n", i);
                    emit_textf("  call rt_proc_set_result_name@PLT\n");
                }
            }
            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
            if (strncmp(proc_names_buf[i], "LBL__", 5) == 0) emit_textf("  lea rsi, [rip + LBL__%s]\n", asm_sym_name(proc_names_buf[i] + 5));
            else emit_textf("  lea rsi, [rip + FN__%s]\n", asm_sym_name(proc_names_buf[i]));
            emit_textf("  call rt_proc_set_fn@PLT\n");
            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
            emit_textf("  mov esi, %d\n", proc_nparams_buf[i]);
            emit_textf("  call rt_proc_set_nparams@PLT\n");
            { int _pin = proc_pidx_buf[i]; int _nf = (_pin >= 0 && _pin < s2->proc_count) ? s2->proc_table[_pin].nformals : 0;
              emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
              emit_textf("  mov esi, %d\n", _nf);
              emit_textf("  call rt_proc_set_nformals@PLT\n"); }
            { int _pi2 = proc_pidx_buf[i];
              if (is_raku && _pi2 >= 0 && _pi2 < s2->proc_count) { ProcEntry *_pe = &s2->proc_table[_pi2];
                if (!_pe->dyn_scope) for (int k = 0; k < _pe->nparams && k < _pe->lower_sc.n; k++) {
                    const char *_pn = _pe->lower_sc.e[k].name; if (!_pn) continue;
                    emit_textf("  .section .rodata\n  .Lstartup_qp%d_%d: .string \"%s\"\n  .section .text\n  .intel_syntax noprefix\n", i, k, _pn);
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  mov esi, %d\n", k);
                    emit_textf("  lea rdx, [rip + .Lstartup_qp%d_%d]\n", i, k);
                    emit_textf("  call rt_proc_set_pname@PLT\n");
                } } }
            if (proc_fb_buf[i] > 0) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, %d\n", proc_fb_buf[i]);
                emit_textf("  call rt_proc_set_frame_bytes@PLT\n");
            }
            { extern int emit_icn_n2_gen_region_ft(const char *, int, IR_graph_t *); int _pig = proc_pidx_buf[i]; int _gft = 0;   /* N-2 apply-call cure (ceo s283h): mode-4 twin of the mode-3 rt_proc_set_gen_region_ft stamp -- emitted into the startup block so the standalone binary's runtime table knows the region-resident alphas too */
              if (_pig >= 0 && _pig < s2->proc_count) { int _gi2 = s2->proc_table[_pig].bb_idx; if (_gi2 >= 0 && _gi2 < s2->bbp.count && s2->bbp.table[_gi2]) _gft = emit_icn_n2_gen_region_ft(proc_names_buf[i], s2->proc_table[_pig].is_generator, s2->bbp.table[_gi2]); }
              if (_gft > 0) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, %d\n", _gft);
                emit_textf("  call rt_proc_set_gen_region_ft@PLT\n");
              } }
            if (proc_ispat_buf[i] && proc_zstatic_buf[i]) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, 1\n");
                emit_textf("  call rt_proc_set_zstatic@PLT\n");
            }
            if (pe->is_variadic) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, 1\n");
                emit_textf("  call rt_proc_set_variadic@PLT\n");
            }
            if (pe->rest_kind) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, %d\n", pe->rest_kind);
                emit_textf("  call rt_proc_set_rest_kind@PLT\n");
            }
            if (pe->named_rest) {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, %d\n", pe->named_rest);
                emit_textf("  call rt_proc_set_named_rest@PLT\n");
            }
            {
                emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                emit_textf("  mov esi, %d\n", strncmp(proc_names_buf[i], "gram__", 6) != 0);
                emit_textf("  call rt_proc_set_jmpentry@PLT\n");
                { extern int rt_pl_dc_ok(const char *, int); if (!proc_ispat_buf[i] && rt_pl_dc_ok(proc_names_buf[i], proc_nparams_buf[i])) {
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  lea rsi, [rip + %s_dc\xce\xb1]\n", asm_sym_name(proc_names_buf[i]));
                    emit_textf("  call rt_proc_set_dcfn@PLT\n"); } }
                if (pe->is_generator) {
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  mov esi, 1\n");
                    emit_textf("  call rt_proc_set_generator@PLT\n");
                }
            }
            } }
        }
        emit_textf("  add rsp, 8\n");
        emit_textf("  ret\n");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_run           = 0;
    int mode_compile       = 0;
    int dump_ast           = 0;
    int dump_ir            = 0;
    int dump_bb            = 0;
    int dump_ir_verbose    = 0;
    int dump_zeta          = 0;
    int dump_transpile     = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    const char * output_path = NULL;
    int argi = 1;
    for (;;) { int before_argi = argi;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--run")           == 0) { mode_run       = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile   = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast       = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir-verbose") == 0) { dump_ir = 1; dump_ir_verbose = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir")       == 0) { dump_ir        = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb")       == 0) { dump_bb        = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-zeta")     == 0) { dump_zeta      = 1; argi++; }
        else if (strcmp(argv[argi], "--transpile")     == 0) { dump_transpile = 1; argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench      = 1; argi++; }
        else if (strcmp(argv[argi], "--monitor")       == 0) { extern int g_monitor_bin; g_monitor_bin = 1; argi++; }
        else if (strcmp(argv[argi], "--no-monitor")    == 0) { extern int g_monitor_bin; g_monitor_bin = 0; argi++; }
        else break;
    }
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] != '-' && argv[argi][1] != '\0' && strchr("sdimo", argv[argi][1])) {
        char sw = argv[argi][1]; const char *rest = argv[argi] + 2; long v;
        if (sw == 'o') { if (*rest == '\0') { if (argi + 1 >= argc) { fprintf(stderr, "scrip: -o needs a filename\n"); return 2; } rest = argv[++argi]; } output_path = rest; argi++; continue; }
        if (*rest == '\0') { if (argi + 1 >= argc) { fprintf(stderr, "scrip: -%c needs a value\n", sw); return 2; } rest = argv[++argi]; }
        v = parse_mem_arg(rest); if (v < 0) { fprintf(stderr, "scrip: bad -%c value '%s' (want e.g. 256m, 20m, 65536)\n", sw, rest); return 2; }
        if (sw == 's') { if (apply_stack_limit(v) != 0) { fprintf(stderr, "scrip: -s%ld: could not raise stack limit\n", v); return 2; } }
        else if (sw == 'm') { extern long g_maxlngth; g_maxlngth = v; }
        argi++;
    }
    if (argi == before_argi) break; }
    for (int oi = argi; oi < argc; oi++) { if (strcmp(argv[oi], "--") == 0) break; if (argv[oi][0] == '-' && argv[oi][1] == 'o') { int eat = 1; if (argv[oi][2] != '\0') output_path = argv[oi] + 2; else { if (oi + 1 >= argc || strcmp(argv[oi+1], "--") == 0) { fprintf(stderr, "scrip: -o needs a filename\n"); return 2; } output_path = argv[oi+1]; eat = 2; } for (int mj = oi; mj + eat < argc; mj++) argv[mj] = argv[mj + eat]; argc -= eat; oi--; } }
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && mode_run) {
        fprintf(stderr, "scrip: --compile (x86) is mutually exclusive with --run\n");
        return 1;
    }
    if (!mode_run && !mode_compile)
        mode_run = 1;
    /* SCRIP_PERF_MAP=1 (Lon 2026-08-28, "make the symbols accessible by the perf tools"): the emitter writes the
     * perf jit-map at slab-seal time (emit.cpp, per sealed graph) so a mode-3 profile names graphs instead of [JIT].
     * ⛔ The map is a MODE-3 artefact only -- mode-4 already carries real symbols in its own symbol table -- so say
     * so plainly rather than let a --compile run leave the user waiting for a file that will never be written.
     * ⛔ perf reads /tmp/perf-<pid>.map REGARDLESS of TMPDIR, and stale content is APPENDED to, not replaced: an
     * old map from a previous run of the same recycled pid mis-names samples silently, which is worse than [JIT]
     * because it is plausible. Name the path so the caller can delete it first. */
    { const char *_pm = getenv("SCRIP_PERF_MAP");
      if (_pm && *_pm && *_pm != '0')
          fprintf(stderr, "[PERF-MAP] %s: /tmp/perf-%d.map (perf jit convention; ⛔ APPEND -- delete a stale map for this pid before profiling)\n",
                  mode_run ? "mode-3 graph names will be written to" : "⛔ NOT written: --compile is mode-4, which already has real symbols; expected", (int)getpid()); }
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-ir        print IR/BB-graph for each proc (terse: slot/op refs only)\n"
            "  --dump-bb        print the Byrd-box graph as JSON (boxes + gamma/omega port edges) for tools/bb_viewer.html\n"
            "  --dump-ir-verbose  same, plus node-id alongside each slot and the legend line\n"
            "  --dump-zeta      print the ZB-2 zeta layout table: scope tree, typed field maps, vslots (post-optimizer)\n"
            "  --transpile      transpile AST to portable SNOBOL4 source\n"
            "  --bench          print wall-clock time after execution\n"
            "\n"
            "Memory options (SPITBOL-compatible; value may end in k or m, e.g. -s256m -m8m):\n"
            "  -sN              max stack space; raises RLIMIT_STACK for deep pattern backtracking (default: OS, 8m)\n"
            "  -mN              max object size -> &MAXLNGTH (default 5m)\n"
            "  -dN -iN          accepted for SPITBOL invocation compatibility (SCRIP's GC arena is not byte-sized)\n"
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
    int is_pascal = 0;
    int saw_sno = 0;
    int is_scrip = 0;
    for (int fi = argi; fi < argc; fi++) {
        if (strcmp(argv[fi], "--") == 0) break;
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && (strcmp(d,".scrip")==0 || strcmp(d,".md")==0)) is_scrip = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
        if (d && strcmp(d,".pas")==0) is_pascal = 1;
        if (!d || strcmp(d,".sno")==0 || strcmp(d,".sc")==0 || strcmp(d,".reb")==0) saw_sno = 1;
    }
    int is_sno_bb = (saw_sno || is_scrip) && !is_pascal;
    lower_seg_t segs[64];
    int nsegs = 0;
    tree_t  *ast_prog = NULL;
    #define RECORD_SEG(sub_ast, seg_fn) do { \
        if ((sub_ast) && nsegs < 64) { \
            tree_t *_sp = calloc(1, sizeof(tree_t)); \
            if (_sp) { _sp->t = TT_PROGRAM; \
                for (int _si = 0; _si < (sub_ast)->n; _si++) if ((sub_ast)->c[_si]) ast_push(_sp, (sub_ast)->c[_si]); \
                segs[nsegs].prog = _sp; segs[nsegs].fn = (seg_fn); nsegs++; } } \
    } while(0)
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
        if (strcmp(argv[argi], "--") == 0) { argi++; g_prog_argv = &argv[argi]; g_prog_argc = argc - argi; break; }
        const char *input_path = argv[argi];
        {
            char rp[4096];
            const char *abs_path = realpath(input_path, rp) ? rp : input_path;
            char dirbuf[4096];
            strncpy(dirbuf, abs_path, sizeof dirbuf - 1);
            dirbuf[sizeof dirbuf - 1] = '\0';
            char *sl = strrchr(dirbuf, '/');
            if (sl) { *sl = '\0'; sno_add_include_dir(strdup(dirbuf)); }
            else     { sno_add_include_dir("."); }
            const char *core_lib = getenv("SNO_LIB");
            if (core_lib && *core_lib) {
                char envb[4096]; strncpy(envb, core_lib, sizeof envb - 1); envb[sizeof envb - 1] = '\0';
                char *sp = envb; char *tk;
                while ((tk = strsep(&sp, ":")) != (char *)0) if (*tk) sno_add_include_dir(strdup(tk));
            }
            char walk[4096];
            strncpy(walk, abs_path, sizeof walk - 1);
            walk[sizeof walk - 1] = '\0';
            char *p = strrchr(walk, '/');
            while (p) {
                *p = '\0';
                char probe[4096];
                snprintf(probe, sizeof probe, "%s/lib", walk);
                struct stat st;
                int hit = (stat(probe, &st) == 0 && S_ISDIR(st.st_mode));
                if (!hit) { snprintf(probe, sizeof probe, "%s/library", walk); hit = (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)); if (hit) sno_add_include_dir(strdup(probe)); }
                snprintf(probe, sizeof probe, "%s/include", walk);
                if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)) { hit = 1; sno_add_include_dir(strdup(probe)); }
                if (hit) sno_add_include_dir(strdup(walk));
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
        int lang_pascal   = dot && strcmp(dot, ".pas")  == 0;
        int lang_polyglot = dot && (strcmp(dot, ".scrip") == 0 || strcmp(dot, ".md") == 0);
        if (lang_polyglot) {
            g_polyglot = 1;
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = parse_scrip_polyglot(src, input_path, segs, &nsegs, 64);
            free(src);
            MERGE_AST(sub_ast);
        } else if (lang_snocone || lang_prolog || lang_icon || lang_raku || lang_rebus || lang_pascal) {
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
            else if (lang_pascal)  pascal_compile(src, input_path, &sub_ast);
            else                   snocone_compile(src, input_path, &sub_ast);
            free(src);
            if (dump_ast && sub_ast) {
                ir_dump_program(sub_ast, stdout); return 0;
            }
            lower_entry_fn seg_fn = lower_sno_stage2;
            if      (lang_pascal) seg_fn = lower_pascal_stage2;
            else if (lang_icon)   seg_fn = lower_icon_stage2;
            else if (lang_prolog) seg_fn = lower_pl_stage2;
            else if (lang_raku)   seg_fn = lower_raku_stage2;
            RECORD_SEG(sub_ast, seg_fn);
            MERGE_AST(sub_ast);
        } else if (dump_ast) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            ir_dump_program(sub_ast, stdout);
            return 0;
        } else if (dump_transpile) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            RECORD_SEG(sub_ast, lower_sno_stage2);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            { extern void stmt_src_set_file(const char *); stmt_src_set_file(input_path); }
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            RECORD_SEG(sub_ast, lower_sno_stage2);
            MERGE_AST(sub_ast);
        }
        if (!ast_prog) {
            fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
            return 1;
        }
        { extern int sno_nerrors; if (sno_nerrors > 0) { fprintf(stderr, "scrip: %d parse error(s) in '%s' -- no code generated\n", sno_nerrors, input_path); return 1; } }
    }
    if (nsegs == 1) segs[0].prog = ast_prog;
    /* zframe-registration-order fix (row polyglot-demo-empty-output-rc0): the Prolog eager-name prepass below is
       gated on is_prolog, which is set ONLY for a standalone .pl file (scrip.c:971) -- for a .scrip/.md polyglot
       file every section routes through lang_polyglot instead, so is_prolog stays false even when one of the
       parsed segments IS Prolog, and the prepass silently never runs. Without it, a graph's name is attached to
       its zg[] registry entry lazily, one proc at a time, only when that proc reaches its own turn in the
       proc_table emission loop (scrip.c ~1774/~1367) -- so a call site emitted for an EARLIER proc that calls a
       LATER-emitted proc (in proc_table order, not source order) does its zls_g_resume_by_name() lookup before
       that callee has a name at all, and silently gets -1 even though drive_slots_all() already computed its
       resume_off correctly by pointer. This does not depend on WHICH proc is affected or on any language's
       arming/depth computation -- has_prolog_seg only widens WHEN the pre-existing prepass runs, it changes no
       computed value. */
    int has_prolog_seg = is_prolog;
    if (!has_prolog_seg) for (int _si = 0; _si < nsegs; _si++) if (segs[_si].fn == lower_pl_stage2) { has_prolog_seg = 1; break; }
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
        { extern void pat_pool_init(void); pat_pool_init(); }
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    extern void core_lib_init(void);
    core_lib_init();
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
    g_opt_dump_bb = dump_ir;
    if (dump_transpile) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_ir || dump_zeta) {
        extern void bb_print_v(const IR_graph_t * bbg, FILE * fp, int verbose);
        extern void ir_drive_slot_assign(IR_graph_t * g);
        extern void zls_graph_name(const IR_graph_t * g, const char * name);
        extern void zls_dump(FILE * fp);
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs); n2_host_scan_diag(s2);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (dump_zeta && (is_icon || is_sno_bb)) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
        const IR_t ** seen_all = (const IR_t **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const IR_t *));
        int seen_n = 0;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            const IR_t ** all = (const IR_t **) s2->bbp.table[idx]->all;
            int dup = 0;
            for (int s = 0; s < seen_n; s++) if (seen_all[s] == (const IR_t *) all) { dup = 1; break; }
            if (dup) continue;
            seen_all[seen_n++] = (const IR_t *) all;
            if (dump_ir) fprintf(stdout, "; proc %s\n", pname);
            if (is_icon || is_sno_bb) zls_graph_name(s2->bbp.table[idx], pname);
            ir_drive_slot_assign(s2->bbp.table[idx]);
            if (dump_ir) bb_print_v(s2->bbp.table[idx], stdout, dump_ir_verbose);
        }
        if (dump_zeta) zls_dump(stdout);
        free(seen_all);
        return 0;
    }
    if (dump_bb) {
        extern void optimizer_run(IR_graph_t * g);
        extern const char * bb_src_of(const IR_t * nd);
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs); n2_host_scan_diag(s2);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_sno_bb || is_prolog) for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]);
        const IR_graph_t ** gset = (const IR_graph_t **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const IR_graph_t *));
        const char ** gname = (const char **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const char *));
        int gn = 0;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            int dup = 0;
            for (int s = 0; s < gn; s++) if (gset[s] == (const IR_graph_t *) s2->bbp.table[idx]) { dup = 1; break; }
            if (dup) continue;
            gname[gn] = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            gset[gn++] = s2->bbp.table[idx];
        }
        fputs("{\"meta\":{\"program\":", stdout); bbj_str(stdout, input_path ? input_path : "?");
        fputs(",\"generator\":\"scrip --dump-bb\"},\n \"boxes\":[", stdout);
        int first = 1, stno = 0;
        for (int gi = 0; gi < gn; gi++) {
            const IR_graph_t * g = gset[gi];
            int * stno_of = (int *) calloc(g->n > 0 ? g->n : 1, sizeof(int));
            const char ** src_of = (const char **) calloc(g->n > 0 ? g->n : 1, sizeof(const char *));
            int pend = 0;
            for (int i = 0; i < g->n; i++) {
                if (!g->all[i]) continue;
                const char * src = bb_src_of(g->all[i]);
                if (!src) continue;
                stno++;
                for (int j = pend; j <= i; j++) if (g->all[j]) stno_of[j] = stno;
                src_of[i] = src;
                pend = i + 1;
            }
            for (int i = 0; i < g->n; i++) {
                const IR_t * bb = g->all[i];
                if (!bb) continue;
                const char * opn = bb_op_name(bb->op);
                if (opn && !strncmp(opn, "IR_", 3)) opn += 3;
                fprintf(stdout, "%s\n  {\"id\":\"b%d_%d\",\"kind\":", first ? "" : ",", gi, i); first = 0;
                bbj_str(stdout, opn ? opn : "?");
                bbj_label(stdout, bb);
                if (stno_of[i] > 0) fprintf(stdout, ",\"stmt\":%d", stno_of[i]);
                if (gi > 0) { fputs(",\"proc\":", stdout); bbj_str(stdout, gname[gi]); }
                if (src_of[i]) { fputs(",\"src\":", stdout); bbj_str(stdout, src_of[i]); }
                fputs("}", stdout);
            }
            free(stno_of); free((void *) src_of);
        }
        fputs("],\n \"edges\":[", stdout);
        first = 1;
        if (gn > 0 && gset[0]->n > 0) {
            int e0 = gset[0]->entry ? bbj_index(gset[0], gset[0]->entry) : 0;
            fprintf(stdout, "\n  {\"from\":\"$start\",\"to\":\"b0_%d\",\"tp\":\"alpha\"}", e0 < 0 ? 0 : e0); first = 0;
        }
        for (int gi = 0; gi < gn; gi++) {
            const IR_graph_t * g = gset[gi];
            for (int i = 0; i < g->n; i++) {
                const IR_t * bb = g->all[i];
                if (!bb) continue;
                bbj_edge(stdout, &first, gi, i, "gamma", g, &bb->γ);
                bbj_edge(stdout, &first, gi, i, "omega", g, &bb->ω);
                for (int oj = 0; oj < bb->n_operands; oj++) {
                    int j = bb->operands && bb->operands[oj] ? bbj_index(g, bb->operands[oj]) : -1;
                    if (j < 0) continue;
                    fprintf(stdout, "%s\n  {\"from\":\"b%d_%d\",\"to\":\"b%d_%d\",\"rel\":\"op\",\"slot\":%d}", first ? "" : ",", gi, i, gi, j, oj);
                    first = 0;
                }
            }
        }
        fputs("]}\n", stdout);
        free(gset); free(gname);
        return 0;
    }
    if (mode_compile_x86) {
        extern int g_frame_active;
        if (is_icon || is_raku || is_sno_bb || is_prolog) {
            extern int g_postfix_resume;
            extern int g_m4_dense_nid; extern void g_bb_alpha_seq_reset(void);
            g_m4_dense_nid = 1; g_bb_alpha_seq_reset();
            if (is_icon) g_postfix_resume = 1;
            stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs); n2_host_scan_diag(s2);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            if (is_icon || is_sno_bb || is_prolog) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            int main_bb_idx = polyglot_main_bb_idx(s2);
            rt_proc_reset();
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
            }
            if (is_icon || is_sno_bb || is_prolog || is_raku || is_pascal) drive_slots_all(s2); n2_fb_prepass_diag(s2); n2_xgraph_probe(s2); n2_fb_prepass_register(s2);
            char smx_why[256];
            if (is_raku && !graph_native_emittable(s2, smx_why, sizeof smx_why)) {
                fprintf(stderr, "[SMX] --compile --target=x86: mode-4 native emitter does not yet cover this program: %s. REJECTED — native BB emission pending (no interpreter fallback).\n", smx_why);
                /* ⛔⭐ 1, NOT 0 (hq_C verdict 2026-08-30, row smx-refusal-exits-zero). This returned 0, so "the compiler could not build this" and "the program ran and printed nothing" shared an exit code AND an empty stdout -- indistinguishable to any caller that checks either. The deciding evidence is nine lines below: the [IBB] FATAL neighbour already returns 1, so the driver ALREADY treats an inability-to-build as non-zero and this path was an inconsistency with its own neighbour rather than a considered refusal policy. ⛔ NOT rc=2: that is the GATE convention for cannot-measure; for a compiler "I cannot build this" is an ordinary build failure and 1 is the honest code. ⭐ Landed only after the exposed-consumer census hq_C required (FINDING-2026-08-30-hq_B-smx-rc0-exposed-population-is-empty-today): eight raku consumers, [SMX] seen by none -- the refusing programs are graded by corpus_suite_harness.py on OUTPUT vs a committed .ref, never on rc. ⚠️ Both sites are is_raku-gated, so today's blast radius is Raku-only; the refusal is the EMITTER's own not-covered path, so any frontend added to that predicate inherits this rc and the census must be re-run then. */
                return 1;
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            g_medium = BB_MEDIUM_TEXT; FILE * _out = stdout; if (output_path) { _out = fopen(output_path, "w"); if (!_out) { perror(output_path); return 1; } } emit_set_sink(_out);
            emit_textf("  .intel_syntax noprefix\n");
            emit_textf("  .text\n");
            { extern int emit_dwarf_loc_on(void); extern const char * stmt_src_get_file(void); const char * _sf = stmt_src_get_file();
              if (emit_dwarf_loc_on() && _sf && *_sf) { emit_textf("  .file 1 \"%s\"\n", _sf); emit_textf("  .file 2 \"<included>\"\n"); } }
            for (int _dz = 0; _dz < bbg->n; _dz++) {
                const char * _dn = ir_define_plain_name(bbg->all[_dz]);
                if (!_dn) continue;
                int _dhave = 0; for (int _q = 0; _q < s2->proc_count; _q++) if (s2->proc_table[_q].name && !strcmp(s2->proc_table[_q].name, _dn)) { _dhave = 1; break; }
                if (_dhave) continue;
                int _ddup = 0; for (int _dw = 0; _dw < _dz; _dw++) { const char * _dwn = ir_define_plain_name(bbg->all[_dw]); if (_dwn && !strcmp(_dwn, _dn)) { _ddup = 1; break; } }
                if (_ddup) continue;
                emit_sep_rule_c('-');
                if (scrip_symmap()) emit_textf("  .type FN__%s, @function\n", asm_sym_name(_dn));
                emit_textf("FN__%s:\n  call rt_ab_undef_fn_stub@PLT\n  ud2\n", asm_sym_name(_dn));
                if (scrip_symmap()) emit_textf("  .size FN__%s, .-FN__%s\n", asm_sym_name(_dn), asm_sym_name(_dn));
            }
            g_frame_active = 1;
            extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            gva_collect_reset();
            { extern void gva_io_refuse_scan_graph(IR_graph_t *); for (int _si = 0; _si < s2->bbp.count; _si++) if (s2->bbp.table[_si]) gva_io_refuse_scan_graph(s2->bbp.table[_si]); }
            gva_collect_icon_globals();
            int n_gva_icn = gva_count();
            g_gva_active = (n_gva_icn > 0) ? 1 : 0;
            int n_procs = 0;
            int _pnbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            const char **proc_names_buf = (const char **)malloc((size_t)_pnbcap * sizeof(const char *));
            int *proc_nparams_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_pidx_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_fb_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_ispat_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_zstatic_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            if (has_prolog_seg) { extern void zls_graph_name(const IR_graph_t *, const char *); for (int _pi2 = 0; _pi2 < s2->proc_count; _pi2++) { const char *_pn2 = s2->proc_table[_pi2].name; if (!_pn2 || strcmp(_pn2, "main") == 0) continue; int _idx2 = s2->proc_table[_pi2].bb_idx; if (_idx2 >= 0 && _idx2 < s2->bbp.count && s2->bbp.table[_idx2]) zls_graph_name(s2->bbp.table[_idx2], _pn2); } }
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
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; int _is_lbl = pname && strncmp(pname, "LBL__", 5) == 0; g_flat_frame_floor = 0; int _floor_hit = (_is_lbl || (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED))); if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG] pname=%s is_lbl=%d entry_op=%d hit=%d\n", pname ? pname : "(null)", _is_lbl, _pg && _pg->entry ? (int)_pg->entry->op : -1, _floor_hit); if (_floor_hit) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } if (getenv("SCRIP_FLOOR_DIAG") && _floor_hit) fprintf(stderr, "[FLOOR-DIAG] -> g_flat_frame_floor=%d\n", g_flat_frame_floor); }
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
                  int _isp = emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]);
                  g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, np)) ? np : -1; proc_ispat_buf[n_procs] = _isp; }
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }
                { char _pfx[256]; snprintf(_pfx, sizeof(_pfx), "proc_%s", asm_sym_name(pname)); int _islbl = pname && strncmp(pname, "LBL__", 5) == 0; IR_graph_t *_bg = s2->bbp.table[idx]; int _bare = (proc_role3_kind(_bg) == 1);    if (!_islbl && !_bare) {    emit_sep_rule_c('-'); if (scrip_symmap()) emit_textf("  .type FN__%s, @function\n", asm_sym_name(pname)); g_emit.flat_bare_chain = _bare; emit_chain(bb_proc_entry(&s2->proc_table[_pi]), _out, _pfx); g_emit.flat_bare_chain = 0; if (scrip_symmap()) emit_textf("  .size FN__%s, .-FN__%s\n", asm_sym_name(pname), asm_sym_name(pname)); } }
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                { extern int g_last_flat_frame_bytes; proc_fb_buf[n_procs] = (pname && strncmp(pname, "LBL__", 5) == 0) ? 0 : g_last_flat_frame_bytes;
                  if (getenv("SCRIP_N2_FB_PREPASS")) fprintf(stderr, "[N2-FB] POSTEMIT mode=4 proc=%s idx=%d fb=%d\n", pname ? pname : "(null)", _pi, g_last_flat_frame_bytes); }
                { extern int g_last_flat_zstatic; proc_zstatic_buf[n_procs] = (pname && strncmp(pname, "LBL__", 5) == 0) ? 0 : g_last_flat_zstatic; }
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); if (!(pname && strncmp(pname, "LBL__", 5) == 0)) emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }
                proc_nparams_buf[n_procs] = np;
                proc_pidx_buf[n_procs] = _pi;
                proc_names_buf[n_procs++] = pname ? strdup(pname) : NULL;
                free(pn);
            }
            int n_cls_emit = 0;
            { extern int dat_type_count(void); n_cls_emit = dat_type_count(); }
            int n_gram_emit = 0;
            { extern int rt_grammar_count(void); n_gram_emit = rt_grammar_count(); }
            emit_textf("  .globl main\n");
            emit_textf("main:\n");
            /* see the sibling emission's comment (same file, ~line 1543): the activation-tier
               offsets collide with the kernel-placed envp/argv block a few hundred bytes above entry rsp
               without this guard (pascal-m4-intermittent-segv-layout-sensitive). +65536 matches the other
               frame modes' own guard band a few lines down; 65544 keeps the same post-sub alignment. */
            emit_textf("  sub rsp, 65544\n");
            emit_textf("  push rdi\n");
            emit_textf("  push rsi\n");
            { const char * hr = getenv("SCRIP_M4_HEADROOM"); if (hr && *hr) { long hb = atol(hr); if (hb > 0) { hb = (hb + 15) & ~15L; emit_textf("  sub rsp, %ld\n", hb); } } }
            emit_textf("  call core_lib_init@PLT\n");
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0)
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0)
                emit_textf("  call %s\n", sn4_module_init_bottom() ? "module_init" : "main_init");
            if (n_gva_icn > 0) emit_textf("  mov edi, %d\n  call rt_gva_island@PLT\n  mov rsi, rax\n  lea rdi, [rip + __gva_names]\n  mov edx, %d\n  call gva_register@PLT\n", n_gva_icn, n_gva_icn);
            { extern int g_monitor_bin; if (g_monitor_bin) emit_textf("  mov edi, dword ptr [rip + __mon_maxst]\n  call rt_mon_set_max_stno@PLT\n"); }
            { extern int prolog_op_user_count(void); extern int prolog_op_user_get(int, const char **, int *, const char **); int n_uop = prolog_op_user_count();
              if (n_uop > 0) { emit_textf("  .section .rodata\n"); for (int k = 0; k < n_uop; k++) { const char *onm = 0; int opr = 0; const char *oty = 0; if (!prolog_op_user_get(k, &onm, &opr, &oty)) continue; char eb[512]; int ei = 0; for (const char *s = onm ? onm : ""; *s && ei < 508; s++) { if (*s == '\\' || *s == '"') eb[ei++] = '\\'; eb[ei++] = *s; } eb[ei] = 0; emit_textf("  .Lopn%d: .string \"%s\"\n  .Lopt%d: .string \"%s\"\n", k, eb, k, oty ? oty : "xfx"); }
                emit_textf("  .section .text\n  .intel_syntax noprefix\n"); for (int k = 0; k < n_uop; k++) { const char *onm = 0; int opr = 0; const char *oty = 0; if (!prolog_op_user_get(k, &onm, &opr, &oty)) continue; emit_textf("  lea rdi, [rip + .Lopn%d]\n  mov esi, %d\n  lea rdx, [rip + .Lopt%d]\n  call prolog_op_table_add@PLT\n", k, opr, k); } } }
            if (bbg->nparams >= 1) emit_textf("  mov rdi, qword ptr [rsp]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 8]\n  sub esi, 1\n  call rt_main_args_stage@PLT\n");
            emit_textf("  mov r12, qword ptr [0x70000000]\n");
            if (is_prolog && bbg->zframe_graph && !bbg->icn_cells_graph) emit_textf("  call rt_gcheap_warmup@PLT\n  call rt_plw_floor_bypass_on@PLT\n");
            { extern unsigned char g_rtcc_on; if (g_rtcc_on) emit_textf("  call rtcc_load_all@PLT\n"); }
            emit_textf("  xor esi, esi\n");
            if (bbg->zframe_graph && !bbg->icn_cells_graph) {
                emit_textf("  xor r14d, r14d\n");
                emit_textf("  lea rcx, [rip + .Lmain_zf_γ]\n");
                emit_textf("  lea rdx, [rip + .Lmain_zf_ω]\n");
                emit_textf("  jmp main_\xce\xb1\n");
                emit_textf(".Lmain_zf_γ:\n  xor edi, edi\n  call exit@PLT\n");
                emit_textf(".Lmain_zf_ω:\n  mov edi, 1\n  call exit@PLT\n");
            } else {
            emit_textf("  xor r14d, r14d\n");
            emit_textf("  jmp main_\xce\xb1\n");
            }
            if (!sn4_module_init_bottom()) emit_module_init_body(s2, proc_names_buf, proc_nparams_buf, proc_pidx_buf, proc_fb_buf, proc_ispat_buf, proc_zstatic_buf, n_procs, n_cls_emit, n_gram_emit, is_raku, "main_init");
            if (n_gva_icn > 0) {
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_gva_icn; k++) emit_textf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                emit_textf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva_icn; k++) emit_textf("  .quad .Lgvan%d\n", k);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            int rc;
            {
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
                { int _bd = 0; for (int _s = 0; _s < bbg->n; _s++) { IR_t *_c = bbg->all[_s]; if (_c && _c->op == IR_DEFINE && !ir_define_sr_citizen(_c) && _c->n_operands == 0) _bd++; }
                  if (_bd > 0 && bbg->n_dentry == 0) { bbg->dentry_node = (IR_t **)calloc((size_t)_bd, sizeof(IR_t *)); bbg->dentry_entry = (IR_t **)calloc((size_t)_bd, sizeof(IR_t *)); bbg->dentry_name = (const char **)calloc((size_t)_bd, sizeof(char *));
                      if (bbg->dentry_node && bbg->dentry_entry && bbg->dentry_name) for (int _s = 0; _s < bbg->n; _s++) { IR_t *_c = bbg->all[_s]; if (!_c || _c->op != IR_DEFINE || ir_define_sr_citizen(_c) || _c->n_operands != 0 || !IR_LIT(_c).sval) continue;
                          if (bbg->n_dentry >= _bd) break;
                          for (int _q = 0; _q < s2->proc_count; _q++) { ProcEntry *_pr = &s2->proc_table[_q]; if (!_pr->name || strcmp(_pr->name, IR_LIT(_c).sval)) continue;
                              IR_t *_sn = bb_proc_entry(_pr); if (!_sn) continue; int _sg = 0; while (_sn && (_sn->op == IR_SUCCEED || _sn->op == IR_FAIL || _sn->op == IR_GOTO) && _sn->γ.node && _sg++ < 64) _sn = _sn->γ.node;
                              IR_t *_gd = (_sn && _sn->op == IR_DEFINE && ir_define_sr_citizen(_sn)) ? _sn->γ.node : _sn; if (!_gd) break;
                              if (_gd->op == IR_GOTO_DEFERRED) { if (!IR_LIT(_gd).sval) break;
                                  const char *_en = IR_LIT(_gd).sval; if (strncmp(_en, "LBL__", 5) == 0) _en += 5;
                                  IR_t *_tn = (IR_t *)0; for (int _w = 0; _w < s2->proc_count; _w++) { ProcEntry *_lr = &s2->proc_table[_w]; if (!_lr->name || strncmp(_lr->name, "LBL__", 5) != 0 || strcmp(_lr->name + 5, _en) || !_lr->proc_entry_node) continue;
                                      _tn = _lr->proc_entry_node; int _tg = 0; while (_tn && (_tn->op == IR_SUCCEED || _tn->op == IR_FAIL || _tn->op == IR_GOTO) && _tn->γ.node && _tg++ < 65536) _tn = _tn->γ.node; break; }
                                  if (!_tn) break;
                                  bbg->dentry_node[bbg->n_dentry] = _c; bbg->dentry_entry[bbg->n_dentry] = _tn; bbg->dentry_name[bbg->n_dentry] = (const char *)0; bbg->n_dentry++; break; }
                              { char _anb[300]; snprintf(_anb, sizeof _anb, "%s_\xce\xb1", IR_LIT(_c).sval);
                                bbg->dentry_node[bbg->n_dentry] = _c; bbg->dentry_entry[bbg->n_dentry] = (IR_t *)0; bbg->dentry_name[bbg->n_dentry] = strdup(_anb); bbg->n_dentry++; break; } } } } }
                { int _na = 0; for (int _q = 0; _q < s2->proc_count; _q++) if (s2->proc_table[_q].name && strncmp(s2->proc_table[_q].name, "LBL__", 5) == 0 && s2->proc_table[_q].proc_entry_node) _na++;
                  if (_na > 0 && bbg->n_balias == 0) { bbg->balias_node = (IR_t **)calloc((size_t)_na, sizeof(IR_t *)); bbg->balias_name = (const char **)calloc((size_t)_na, sizeof(char *));
                      if (bbg->balias_node && bbg->balias_name) for (int _q = 0; _q < s2->proc_count; _q++) { if (!s2->proc_table[_q].name || strncmp(s2->proc_table[_q].name, "LBL__", 5) != 0 || !s2->proc_table[_q].proc_entry_node) continue;
                          if (bbg->n_balias >= _na) break;
                          IR_t * _bn = s2->proc_table[_q].proc_entry_node; int _bgg = 0; while (_bn && (_bn->op == IR_SUCCEED || _bn->op == IR_FAIL || _bn->op == IR_GOTO) && _bn->γ.node && _bgg++ < 65536) _bn = _bn->γ.node;
                          { int _dl = 0; for (int _dq = 0; _dq < bbg->n_dentry; _dq++) if (bbg->dentry_entry[_dq] == _bn) { _dl = 1; break; } if (_dl) continue; }
                          char _ab[300]; snprintf(_ab, sizeof _ab, "LBL__%s", asm_sym_name(s2->proc_table[_q].name + 5)); bbg->balias_node[bbg->n_balias] = _bn; bbg->balias_name[bbg->n_balias] = strdup(_ab); if (bbg->balias_name[bbg->n_balias]) bbg->n_balias++; } } }
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = bbg->nparams; }
                emit_sep_rule_c('-'); rc = emit_chain(bbg->entry, _out, "main") ? 0 : 1;
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
                { extern void bb_ab_emit_nodes(IR_graph_t *g, int gva_active); bb_ab_emit_nodes(bbg, g_gva_active); }
                { extern int g_last_flat_frame_bytes; int _main_fb = g_last_flat_frame_bytes; for (int _q = 0; _q < n_procs; _q++) { if (proc_fb_buf[_q] != 0) continue; int _pi2 = proc_pidx_buf[_q]; if (_pi2 < 0 || _pi2 >= s2->proc_count) continue; const char *_qn = s2->proc_table[_pi2].name; if (!_qn || strncmp(_qn, "LBL__", 5) != 0) continue; if (s2->proc_table[_pi2].bb_idx == main_bb_idx) proc_fb_buf[_q] = _main_fb; } }
                if (sn4_module_init_bottom()) emit_module_init_body(s2, proc_names_buf, proc_nparams_buf, proc_pidx_buf, proc_fb_buf, proc_ispat_buf, proc_zstatic_buf, n_procs, n_cls_emit, n_gram_emit, is_raku, "module_init");
            }
            for (int _fq = 0; _fq < n_procs; _fq++) if (proc_names_buf[_fq]) { free((void *)proc_names_buf[_fq]); proc_names_buf[_fq] = NULL; }
            free(proc_names_buf); free(proc_nparams_buf); free(proc_pidx_buf); free(proc_fb_buf); free(proc_zstatic_buf);
            g_gva_active = 0;
            g_frame_active = 0;
            extern void xa_emit_strtab_rodata(void);
            xa_emit_strtab_rodata();
            { extern void xa_emit_csettab_rodata(void); xa_emit_csettab_rodata(); }
            { extern int g_monitor_bin; extern int g_mon_max_stno; if (g_monitor_bin) emit_textf("  .align 4\n__mon_maxst:\n  .long %d\n", g_mon_max_stno); }
            emit_textf("  .section .note.GNU-stack,\"\",@progbits\n");
            emit_textf_flush();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
        {
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern void xa_emit_strtab_rodata(void);
            extern int g_frame_active;
            extern void rt_proc_reset(void);
            extern void rt_proc_register(const char * name, const char ** pnames, int nparams);
            stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs); n2_host_scan_diag(s2);
            if (!s2) { fprintf(stderr, "[SBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            if (is_pascal) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            drive_slots_all(s2); n2_fb_prepass_diag(s2); n2_xgraph_probe(s2); n2_fb_prepass_register(s2);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[SBB] FATAL: mode-4 driver: SNOBOL4 main BB graph not found\n");
                return 1;
            }
            IR_graph_t *sbbg = s2->bbp.table[main_bb_idx];
            extern int g_flat_node_id;
            extern int g_m4_dense_nid;
            g_flat_node_id = 0;
            g_m4_dense_nid = 1;
            g_medium = BB_MEDIUM_TEXT; FILE * _out = stdout; if (output_path) { _out = fopen(output_path, "w"); if (!_out) { perror(output_path); return 1; } } emit_set_sink(_out);
            emit_textf("  .intel_syntax noprefix\n");
            emit_textf("  .text\n");
            { extern int emit_dwarf_loc_on(void); extern const char * stmt_src_get_file(void); const char * _sf = stmt_src_get_file();
              if (emit_dwarf_loc_on() && _sf && *_sf) { emit_textf("  .file 1 \"%s\"\n", _sf); emit_textf("  .file 2 \"<included>\"\n"); } }
            rt_proc_reset();
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
            }
            { extern int g_proc_direct_active; extern void proc_collect_reset(void); extern void proc_collect_graph(IR_graph_t *); extern int proc_slot_count(void);
              proc_collect_reset(); proc_collect_graph(sbbg);
              for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                  const char *pn2 = s2->proc_table[_pi].name; if (!pn2 || strcmp(pn2, "main") == 0) continue; int idx2 = s2->proc_table[_pi].bb_idx;
                  if (idx2 < 0 || idx2 >= s2->bbp.count || !s2->bbp.table[idx2] || !s2->bbp.table[idx2]->entry) continue; proc_collect_graph(s2->bbp.table[idx2]);
              }
              g_proc_direct_active = (proc_slot_count() > 0) ? 1 : 0; }
            if (is_pascal) { extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern int g_gva_active; gva_collect_reset(); gva_collect_graph(sbbg); for (int _pgi = 0; _pgi < s2->bbp.count; _pgi++) { if (s2->bbp.table[_pgi] && s2->bbp.table[_pgi] != sbbg) gva_collect_graph(s2->bbp.table[_pgi]); } g_gva_active = (gva_count() > 0) ? 1 : 0; }
            int _pbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            int *pidx_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int *peak_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int n_procs = 0;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; g_flat_frame_floor = 0; int _floor_hit = (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED)); if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG-B] pname=%s entry_op=%d hit=%d\n", pname ? pname : "(null)", _pg && _pg->entry ? (int)_pg->entry->op : -1, _floor_hit); if (_floor_hit) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } if (getenv("SCRIP_FLOOR_DIAG") && _floor_hit) fprintf(stderr, "[FLOOR-DIAG-B] -> g_flat_frame_floor=%d\n", g_flat_frame_floor); }
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); if (!emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx])) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]); }
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }
                { char _pfx[256]; snprintf(_pfx, sizeof(_pfx), "proc_%s", asm_sym_name(pname)); int _islbl = pname && strncmp(pname, "LBL__", 5) == 0; emit_sep_rule_c('-'); if (!_islbl && scrip_symmap()) emit_textf("  .type FN__%s, @function\n", asm_sym_name(pname)); emit_chain(s2->proc_table[_pi].proc_entry_node, _out, _pfx); if (!_islbl && scrip_symmap()) emit_textf("  .size FN__%s, .-FN__%s\n", asm_sym_name(pname), asm_sym_name(pname)); }
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; peak_buf[n_procs] = g_last_flat_frame_bytes; }
                pidx_buf[n_procs++] = _pi;
            }
            int n_gva = gva_count();
            int n_proc_slot = proc_slot_count();
            /* The activation-tier slots are addressed at fixed positive offsets from this
               entry rsp (up into the 100s-1000s of bytes for a nontrivial "main"), with no subsequent
               reservation of that range -- on a real exec() stack that range collides with the kernel-
               placed envp/argv block a few hundred bytes above entry rsp (pascal-m4-intermittent-segv-
               layout-sensitive: boolmix's own literal-cell write landed on environ[]'s slot and flipped
               a byte from NULL to a small tag, so glibc's getenv() walked off the end into unrelated
               memory -- intermittent because it depends on ASLR-shifted stack contents past the array).
               +65536 matches the guard band the non-RSP frame modes already reserve+zero a few lines
               down for the identical reason; 65536 is 16-aligned so the residual "+8" keeps the original
               post-sub alignment (entry rsp%16==8 from the call instruction, 0 after this sub). */
            emit_textf("  .globl main\nmain:\n  sub rsp, 65544\n  push rdi\n  push rsi\n");
            { const char * hr = getenv("SCRIP_M4_HEADROOM"); if (hr && *hr) { long hb = atol(hr); if (hb > 0) { hb = (hb + 15) & ~15L; emit_textf("  sub rsp, %ld\n", hb); } } }
            if (n_procs > 0) emit_textf("  call main_init\n");
            else emit_textf("  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
            if (n_proc_slot > 0) emit_textf("  lea rdi, [rip + __proc]\n  lea rsi, [rip + __proc_names]\n  mov edx, %d\n  call rt_proc_table_fill@PLT\n", n_proc_slot);
            if (n_gva > 0) emit_textf("  mov edi, %d\n  call rt_gva_island@PLT\n  mov rsi, rax\n  lea rdi, [rip + __gva_names]\n  mov edx, %d\n  call gva_register@PLT\n", n_gva, n_gva);
            { extern int g_monitor_bin; if (g_monitor_bin) emit_textf("  mov edi, dword ptr [rip + __mon_maxst]\n  call rt_mon_set_max_stno@PLT\n"); }
            if (sbbg->nparams >= 1) emit_textf("  mov rdi, qword ptr [rsp]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 8]\n  sub esi, 1\n  call rt_main_args_stage@PLT\n");
            emit_textf("  mov r12, qword ptr [0x70000000]\n");
            emit_textf("  xor esi, esi\n");
            { extern unsigned char g_rtcc_on; if (g_rtcc_on) emit_textf("  call rtcc_load_all@PLT\n"); }
            emit_textf("  jmp flat_\xce\xb1\n");
            if (n_procs > 0) {
                emit_textf("  .section .rodata\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    emit_textf("  .Lpn%d: .string \"%s\"\n", i, pe->name);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                        emit_textf("  .Lpp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                    emit_textf("  .Lpnames%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .quad .Lpp%d_%d\n", i, k);
                    emit_textf("  .quad 0\n");
                }
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                emit_textf("main_init:\n  sub rsp, 8\n  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                    emit_textf("  lea rsi, [rip + .Lpnames%d]\n", i);
                    emit_textf("  mov edx, %d\n", pe->nparams);
                    emit_textf("  call rt_proc_register@PLT\n");
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                    emit_textf("  mov esi, %d\n", pe->nformals);
                    emit_textf("  call rt_proc_set_nformals@PLT\n");
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                    /* ENTRY label is what codegen_flat_chain_body minted: FN__<name> for a non-bare "proc_" chain; <name>_alpha is a CLASS-C alias, undefined here. Mirrors emit_module_init_body(). */
                    if (pe->name && strncmp(pe->name, "LBL__", 5) == 0) emit_textf("  lea rsi, [rip + LBL__%s]\n", asm_sym_name(pe->name + 5));
                    else emit_textf("  lea rsi, [rip + FN__%s]\n", asm_sym_name(pe->name));
                    emit_textf("  call rt_proc_set_fn@PLT\n");
                    int _fidx = pe->bb_idx;
                    if (_fidx >= 0 && _fidx < s2->bbp.count && s2->bbp.table[_fidx] && s2->bbp.table[_fidx]->nslots > 0) {
                        emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                        emit_textf("  mov esi, %d\n", s2->bbp.table[_fidx]->nslots - 1);
                        emit_textf("  mov edx, %d\n", pe->decl_level);
                        emit_textf("  call rt_proc_set_frame@PLT\n");
                    }
                    if (peak_buf[i] > 0) {
                        emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                        emit_textf("  mov esi, %d\n", peak_buf[i]);
                        emit_textf("  call rt_proc_set_frame_bytes@PLT\n");
                    }
                }
                emit_textf("  add rsp, 8\n  ret\n");
            }
            free(pidx_buf); free(peak_buf);
            extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            extern int proc_slot_count(void); extern int g_proc_direct_active;
            if (!is_pascal) { extern void gva_io_refuse_scan_graph(IR_graph_t *); gva_collect_reset(); gva_io_refuse_scan_graph(sbbg); gva_collect_graph(sbbg); }
            if (n_gva > 0) {
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_gva; k++) emit_textf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                emit_textf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva; k++) emit_textf("  .quad .Lgvan%d\n", k);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            if (n_proc_slot > 0) {
                extern const char *proc_slot_name(int);
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_proc_slot; k++) emit_textf("  .Lprocn%d: .string \"%s\"\n", k, proc_slot_name(k));
                emit_textf("  .align 8\n__proc_names:\n");
                for (int k = 0; k < n_proc_slot; k++) emit_textf("  .quad .Lprocn%d\n", k);
                emit_textf("  .section .bss\n  .align 8\n__proc: .space %d, 0\n", n_proc_slot * 8);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            g_gva_active = (n_gva > 0) ? 1 : 0;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = sbbg; }
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = sbbg->nparams; }
            emit_sep_rule_c('-'); int rc = emit_chain(sbbg->entry, _out, "flat") ? 0 : 1;
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
            { extern void bb_ab_emit_nodes(IR_graph_t *g, int gva_active); bb_ab_emit_nodes(sbbg, g_gva_active); }
            g_gva_active = 0;
            g_proc_direct_active = 0;
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            { extern void xa_emit_csettab_rodata(void); xa_emit_csettab_rodata(); }
            { extern int g_monitor_bin; extern int g_mon_max_stno; if (g_monitor_bin) emit_textf("  .align 4\n__mon_maxst:\n  .long %d\n", g_mon_max_stno); }
            emit_textf("  .section .note.GNU-stack,\"\",@progbits\n");
            emit_textf_flush();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen removed).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_run) {
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs); n2_host_scan_diag(s2);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_raku || is_sno_bb || is_prolog) {
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            extern int g_m4_dense_nid; extern void g_bb_alpha_seq_reset(); extern int x86_diag_regs_on_c(void);
            if (x86_diag_regs_on_c()) { g_m4_dense_nid = 1; g_bb_alpha_seq_reset(); }
            int main_bb_idx = polyglot_main_bb_idx(s2);
            rt_proc_reset();
            g_frame_active = 1;
            void *m3_gva_arena = (void *)0;
            {
                extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                { extern void gva_io_refuse_scan_graph(IR_graph_t *); for (int _si = 0; _si < s2->bbp.count; _si++) if (s2->bbp.table[_si]) gva_io_refuse_scan_graph(s2->bbp.table[_si]); }
                gva_collect_icon_globals();
                int n_gva_m3; { const char *_gv = getenv("SCRIP_M3_GVA"); n_gva_m3 = _gv && *_gv && *_gv == (char)48 ? 0 : gva_count(); }
                if (n_gva_m3 > 0) {
                    { extern DESCR_t *rt_gva_island(int); m3_gva_arena = rt_gva_island(n_gva_m3); }
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via pinned island: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) continue;   /* main_bb_idx now comes from polyglot_main_bb_idx() above (row polyglot-main-selector-ignores-main-mod-registry), not a table scan here -- see that function for the mode-3-vs-mode-4 order-collision history this comment used to carry. */
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
            }
            if (is_icon || is_sno_bb || is_prolog) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            if (is_icon || is_sno_bb || is_prolog || is_raku || is_pascal) drive_slots_all(s2); n2_fb_prepass_diag(s2); n2_xgraph_probe(s2); n2_fb_prepass_register(s2);
            char smx_why[256];
            if (is_raku && !graph_native_emittable_mode(s2, 1, smx_why, sizeof smx_why)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program: %s. REJECTED — native BB emission pending (no interpreter fallback).\n", smx_why);
                /* ⛔⭐ 1, matching the MODE-4 site above -- and DELIBERATELY NOT this site's own neighbour, which calls abort(). ⭐⭐ THE RULE IS NOT PROXIMITY, IT IS THE PRINCIPLE THE NEIGHBOURS INSTANTIATE (hq_C, restating their own "match the neighbour" ruling after it was tested rather than executed): abort() is for a VIOLATED INTERNAL INVARIANT -- a state the compiler believes impossible, like "main BB graph not found" after a successful build, where no correct user action exists because the compiler is broken. A non-zero return is for an ANTICIPATED REFUSAL -- a program we knowingly do not cover yet, where the user's action is clear. ⛔⭐ AND THE SMX PATH'S OWN TEXT SETTLES WHICH IT IS: WE PRINTED A SENTENCE EXPLAINING IT IN ADVANCE. A condition you can write prose for beforehand is not an invariant violation. Executing "match the neighbour" literally here would have exited 134 and been triaged as a compiler crash -- manufacturing the exact false signal this row exists to delete, merely pointing the other way. */
                return 1;
            }
            if (has_prolog_seg) { extern void zls_graph_name(const IR_graph_t *, const char *); for (int _pi2 = 0; _pi2 < s2->proc_count; _pi2++) { const char *_pn2 = s2->proc_table[_pi2].name; if (!_pn2 || strcmp(_pn2, "main") == 0) continue; int _idx2 = s2->proc_table[_pi2].bb_idx; if (_idx2 >= 0 && _idx2 < s2->bbp.count && s2->bbp.table[_idx2]) zls_graph_name(s2->bbp.table[_idx2], _pn2); } }
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
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; int _is_lbl = pname && strncmp(pname, "LBL__", 5) == 0; g_flat_frame_floor = 0; int _floor_hit = (_is_lbl || (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED))); if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG] pname=%s is_lbl=%d entry_op=%d hit=%d\n", pname ? pname : "(null)", _is_lbl, _pg && _pg->entry ? (int)_pg->entry->op : -1, _floor_hit); if (_floor_hit) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } if (getenv("SCRIP_FLOOR_DIAG") && _floor_hit) fprintf(stderr, "[FLOOR-DIAG] -> g_flat_frame_floor=%d\n", g_flat_frame_floor); }
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
                  int _isp = emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]);
                  g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, s2->proc_table[_pi].nparams)) ? s2->proc_table[_pi].nparams : -1; }
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }
                int _islbl3 = pname && strncmp(pname, "LBL__", 5) == 0;
                char _m3pfx[300]; snprintf(_m3pfx, sizeof _m3pfx, "proc_%s", pname);
                bb_box_fn pfn = _islbl3 ? NULL : emit_chain(bb_proc_entry(&s2->proc_table[_pi]), NULL, _m3pfx);
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); if (!_islbl3) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
                { extern int g_last_flat_frame_bytes; if (getenv("SCRIP_N2_FB_PREPASS"))
                  fprintf(stderr, "[N2-FB] POSTEMIT mode=3 proc=%s idx=%d fb=%d\n", pname ? pname : "(null)", _pi, g_last_flat_frame_bytes); }
                if (pfn) rt_proc_set_fn(pname, pfn);
                if (pfn) m3_seal_entry_cells(pname, (void *)pfn, 1);
                { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic); }
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); if (!_islbl3) emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }
                { extern int emit_icn_n2_gen_region_ft(const char *, int, IR_graph_t *); extern void rt_proc_set_gen_region_ft(const char *, int); int _gft = emit_icn_n2_gen_region_ft(pname, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]); if (_gft > 0) rt_proc_set_gen_region_ft(pname, _gft); }   /* N-2 apply-call cure (ceo s283h): tell the runtime this alpha is region-resident and how big its slice is, so the coexpr window can supply one and spine_prep can refuse the un-wired transfer */
                { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); if (pfn && g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *)((char *)pfn + g_last_dc_off)); }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); if (pfn && g_last_flat_frame_bytes > 0) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
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
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            bb_box_fn fn;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = bbg->nparams; }
            { extern void (*g_emit_chain_posthook)(void); extern void bb_ab_emit_nodes(IR_graph_t*, int); extern int g_gva_active;
              g_ab_posthook_g = bbg; g_ab_posthook_gva = g_gva_active;
              g_emit_chain_posthook = bb_ab_posthook; }
            { int _na = 0; for (int _q = 0; _q < s2->proc_count; _q++) if (s2->proc_table[_q].name && strncmp(s2->proc_table[_q].name, "LBL__", 5) == 0 && s2->proc_table[_q].proc_entry_node) _na++;
              if (_na > 0 && bbg->n_balias == 0) { bbg->balias_node = (IR_t **)calloc((size_t)_na, sizeof(IR_t *)); bbg->balias_name = (const char **)calloc((size_t)_na, sizeof(char *));
                  if (bbg->balias_node && bbg->balias_name) for (int _q = 0; _q < s2->proc_count; _q++) { if (!s2->proc_table[_q].name || strncmp(s2->proc_table[_q].name, "LBL__", 5) != 0 || !s2->proc_table[_q].proc_entry_node) continue;
                      if (bbg->n_balias >= _na) break;
                      IR_t * _bn = s2->proc_table[_q].proc_entry_node; int _bgg = 0; while (_bn && (_bn->op == IR_SUCCEED || _bn->op == IR_FAIL || _bn->op == IR_GOTO) && _bn->γ.node && _bgg++ < 65536) _bn = _bn->γ.node;
                      char _ab[300]; snprintf(_ab, sizeof _ab, "LBL__%s", asm_sym_name(s2->proc_table[_q].name + 5)); bbg->balias_node[bbg->n_balias] = _bn; bbg->balias_name[bbg->n_balias] = strdup(_ab); if (bbg->balias_name[bbg->n_balias]) bbg->n_balias++; } } }
            fn = emit_chain(bbg->entry, NULL, "pat_flat");
            if (fn) { extern int emit_label_lookup_offset(const char *); extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); int _mfb = g_last_flat_frame_bytes;
              for (int _q = 0; _q < s2->proc_count; _q++) { const char * _ln = s2->proc_table[_q].name; if (!_ln || strncmp(_ln, "LBL__", 5) != 0) continue;
                char _ab[300]; snprintf(_ab, sizeof _ab, "LBL__%s", asm_sym_name(_ln + 5)); int _off = emit_label_lookup_offset(_ab);
                if (_off < 0) { static int _lw = -1; if (_lw < 0) { const char * e = getenv("SCRIP_M3_UNIFY_DIAG"); _lw = (e && *e == '1') ? 1 : 0; } if (_lw) fprintf(stderr, "[M3-UNIFY] %s: body label %s not defined in main chain\n", _ln, _ab); continue; }
                rt_proc_set_fn(_ln, (bb_box_fn)((char *)fn + _off)); if (_mfb > 0) rt_proc_set_frame_bytes(_ln, _mfb);
                m3_seal_entry_cells(_ln + 5, (void *)fn, 0); } }
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: emit_chain returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            int _nparams = bbg->nparams, _zframe_graph = bbg->zframe_graph, _icn_cells_graph = bbg->icn_cells_graph;
            ir_delete_all(s2);
            void *mf = NULL;
            if (_nparams >= 1) { extern void rt_main_args_stage(char **, int); rt_main_args_stage(g_prog_argv, g_prog_argc); }
            { extern void bbprof_start(void); bbprof_start(); }
            { extern void rt_gcheap_warmup(void); rt_gcheap_warmup(); }
            if (_zframe_graph && !_icn_cells_graph) {
                { extern int g_plw_floor_bypass; g_plw_floor_bypass = 1; }
                icn_zf_main_call((void *)fn, mf, (void *)icn_zf_exit_γ, (void *)icn_zf_exit_ω);
            } else
            { extern void rt_outer_call(bb_box_fn, void *, long); extern int g_plw_floor_bypass; int _bypass = is_prolog && _zframe_graph; if (_bypass) g_plw_floor_bypass = 1;  { extern void rtcc_load_all(void); extern unsigned char g_rtcc_on; if (g_rtcc_on) rtcc_load_all(); }    { extern void rt_outer_call_delta0(bb_box_fn, void *, long); if (is_icon) rt_outer_call_delta0(fn, mf, 0); else rt_outer_call(fn, mf, 0); }    if (_bypass) g_plw_floor_bypass = 0; }
            goto run_done;
        }
        {
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern int g_frame_active;
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            rt_proc_reset();
            void *m3_gva_arena = (void *)0;
            if (is_pascal) {
                extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                IR_graph_t *_mg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : (IR_graph_t *)0;
                if (_mg) gva_collect_graph(_mg);
                for (int _pi = 0; _pi < s2->proc_count; _pi++) { int _pgi = s2->proc_table[_pi].bb_idx; if (_pgi >= 0 && _pgi < s2->bbp.count && s2->bbp.table[_pgi] && s2->bbp.table[_pgi] != _mg) gva_collect_graph(s2->bbp.table[_pgi]); }
                int n_gva_m3; { const char *_gv = getenv("SCRIP_M3_GVA"); n_gva_m3 = _gv && *_gv && *_gv == (char)48 ? 0 : gva_count(); }
                if (n_gva_m3 > 0) {
                    { extern DESCR_t *rt_gva_island(int); m3_gva_arena = rt_gva_island(n_gva_m3); }
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via pinned island: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            drive_slots_all(s2); n2_fb_prepass_diag(s2); n2_xgraph_probe(s2); n2_fb_prepass_register(s2);
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
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
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; g_flat_frame_floor = 0; int _floor_hit = (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED)); if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG-B] pname=%s entry_op=%d hit=%d\n", pname ? pname : "(null)", _pg && _pg->entry ? (int)_pg->entry->op : -1, _floor_hit); if (_floor_hit) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } if (getenv("SCRIP_FLOOR_DIAG") && _floor_hit) fprintf(stderr, "[FLOOR-DIAG-B] -> g_flat_frame_floor=%d\n", g_flat_frame_floor); }
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); if (!emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx])) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]); }
                { if (is_icon || is_sno_bb) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }
                char _m3pfx[300]; snprintf(_m3pfx, sizeof _m3pfx, "proc_%s", pname);
                bb_box_fn pfn = emit_chain(s2->proc_table[_pi].proc_entry_node, NULL, _m3pfx);
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
                if (pfn) rt_proc_set_fn(pname, pfn);
                if (pfn) m3_seal_entry_cells(pname, (void *)pfn, 1);
                { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic); }
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }
            }
            g_frame_active = 0;
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = sbbg; }
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = sbbg->nparams; }
                bb_box_fn fn = emit_chain(sbbg->entry, NULL, "pat_flat");
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
                g_frame_active = 0;
                ir_delete_all(s2);
                if (fn) {
                    void *mf = NULL;
                    if (sbbg->nparams >= 1) { extern void rt_main_args_stage(char **, int); rt_main_args_stage(g_prog_argv, g_prog_argc); }
                    { extern void bbprof_start(void); bbprof_start(); }
                    { extern void rt_gcheap_warmup(void); rt_gcheap_warmup(); }
                    { extern void rt_outer_call(bb_box_fn, void *, long); extern void rt_outer_call_delta0(bb_box_fn, void *, long); if (is_icon) rt_outer_call_delta0(fn, mf, 0); else rt_outer_call(fn, mf, 0); }
                    { extern int g_gva_active; g_gva_active = 0; } goto run_done;
                }
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
    { extern void bbprof_report(void); bbprof_report(); }
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
