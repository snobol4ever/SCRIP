#include <stdio.h>
#include "rt/rt_arena.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core.h"
#include "keywords.h"
#include "sil_macros.h"
#include "../parser/snobol4/scrip_cc.h"
#include "IR.h"
#include "stage2.h"
extern int exec_stmt(const char  *subj_name,
                          DESCR_t     *subj_var,
                          DESCR_t      pat,
                          DESCR_t     *repl,
                          int          has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
typedef DESCR_t (*eval_chain_fn)(void *zeta, int entry);
extern void          *lower_snobol4(const tree_t *prog);
extern eval_chain_fn  emit_chain(void *entry, void *out, const char *prefix);
extern void           rt_chain_enter(eval_chain_fn fn);
extern int            emit_jmp_entry_for_chain(IR_graph_t *g);
extern void           emit_jmp_entry_clear(void);
extern void           ast_tree_free_dyn(tree_t *p);
extern void           IR_free_dyn(void *g);
extern size_t         bb_pool_mark(void);
extern void           bb_pool_release(size_t mark);
#define EVAL_TMP_MARKED "EVAL$"
#define EVAL_TMP_LEGACY "ZZEVALZZ"
#define EVAL_TMP eval_tmp_name()
typedef struct { char *key; eval_chain_fn fn; } eval_cache_ent_t;
static eval_cache_ent_t *g_eval_cache = NULL;
static int               g_eval_cache_n = 0;
static int               g_eval_cache_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ s116 MON-CAP -- THE EVAL TEMP IS A COMPILER MINT AND MUST SAY SO AT MINT TIME.  core.c's mon_name_is_internal (ONE AUTHORITY for both VALUE fire-points) recognises a compiler-minted name by
 * an identifier HEAD followed by '$' -- the shape the SNOBOL4 lowerer already mints for itself (PAT$n, PAT$n$A<i>, PATV$k, PATTMP$n, EXPR$n, IGT$n, SNO$*).  "ZZEVALZZ" carried NO marker, so every
 * EVAL-bearing program emitted VALUE events the oracle has no counterpart for and desynced the trace: measured on probe/eval/ev_min_arith.sno (top-level EVAL, no DEFINE), the oracle emits 3 events
 * and scrip emitted 5 -- two spurious `VALUE ZZEVALZZ` (the INT result, then the STRING(0) restore of the saved value) -- diverging at step 2 of 3.  s112's own note at core.c:293 prescribes exactly
 * this remedy: "The durable fix is a mint-time marker on compiler names; until then, keep it here."  THIS IS THAT FIX, AND IT IS NOT A SECOND RULE: no monitor-side predicate, no ZZEVALZZ special
 * case, nothing to keep in sync -- the name simply joins the convention the filter already reads, so the mint and the filter cannot drift the way a name-specific suppression would.
 * ⛔ THE MARKER IS SAFE FROM THE PARSER BECAUSE THIS NAME NEVER REACHES IT: eval_build_chain parses only the user's expression text "(%s)" and injects this name as an AST TT_VAR node
 * (var->v.sval), so '$' is never read as SPITBOL's indirect-reference operator.  ONE AUTHORITY: every NV_GET_fn/NV_SET_fn site reads this function, so the saved/restore pair cannot straddle two
 * spellings.  Killswitch SCRIP_EVAL_TMP_MARK=0 restores the legacy name for A/B. */
static const char *eval_tmp_name(void)
    { static int m = -1; if (m < 0) { const char *e = getenv("SCRIP_EVAL_TMP_MARK"); m = (e && e[0] == '0') ? 0 : 1; } return m ? EVAL_TMP_MARKED : EVAL_TMP_LEGACY; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned long eval_cache_hash(const char *s) { unsigned long h = 1469598103934665603UL; while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211UL; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_cache_get(const char *s) {
    if (g_eval_cache_cap == 0) return NULL;
    unsigned long m = (unsigned long)g_eval_cache_cap - 1;
    for (unsigned long i = eval_cache_hash(s) & m, p = 0; p < (unsigned long)g_eval_cache_cap; p++, i = (i + 1) & m)
        if (!g_eval_cache[i].key) return NULL; else if (strcmp(g_eval_cache[i].key, s) == 0) return g_eval_cache[i].fn;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_insert_raw(eval_cache_ent_t *tab, int cap, char *key, eval_chain_fn fn) {
    unsigned long m = (unsigned long)cap - 1;
    unsigned long i = eval_cache_hash(key) & m;
    while (tab[i].key) i = (i + 1) & m;
    tab[i].key = key; tab[i].fn = fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_put(const char *s, eval_chain_fn fn) {
    if (g_eval_cache_cap == 0 || (g_eval_cache_n + 1) * 2 > g_eval_cache_cap) {
        int ncap = g_eval_cache_cap ? g_eval_cache_cap * 2 : 16;
        eval_cache_ent_t *ntab = (eval_cache_ent_t *)rt_ws_alloc((size_t)ncap * sizeof(eval_cache_ent_t));
        if (!ntab) return;
        for (int k = 0; k < g_eval_cache_cap; k++) if (g_eval_cache[k].key) eval_cache_insert_raw(ntab, ncap, g_eval_cache[k].key, g_eval_cache[k].fn);
        g_eval_cache = ntab; g_eval_cache_cap = ncap;
    }
    char *key = rt_ws_strdup(s);
    if (!key) return;
    eval_cache_insert_raw(g_eval_cache, g_eval_cache_cap, key, fn);
    g_eval_cache_n++;
}
/* rt_chain_enter — the s59 DEFER shape for EVAL/CODE (Lon: "no CALL/RET — a JUMP and a JUMP BACK").  Resolve
 * happened in C (cache/compile/registry lookup); this shim WIRES outside-γ→rcx and outside-ω→rdx to one shared
 * landing and JUMPS.  R12-ERAD s67: the r12 anchor + `mov r12,rsp` wholesale reclaim are DELETED — the s65
 * shared epilogue already converted chain exits to ABSOLUTE unwind on BOTH edges (measured in the live blob:
 * γ and ω each `mov [rsp+K-24/-16],rax; lea rsp,[rsp+K]; jmp *rax`, interior failures funnel through the
 * blob-local epilogue first), so the landing always arrives at exactly the pre-jmp rsp and r12 rides through
 * as an ordinary callee-save.  The chain is a NEW ACTIVATION: it self-allocates its own frame (K_total baked
 * at emit time from its region) and is one-shot.  The trailing ret is the C builtin boundary, not the
 * transfer.  ⚠ EXACTLY FIVE PUSHES — the SysV 16-byte stack alignment is load-bearing through the jmp: rsp is
 * 8 mod 16 at entry, +40 bytes of pushes makes it 0 mod 16, and the blob's `sub rsp,K_total` (K_total
 * 16-aligned) carries that alignment into the activation.  A sixth push (the first cut saved ___) left every
 * C callee reached FROM the chain on a misaligned stack and SEGV'd in libc's SSE printf path — measured, gdb,
 * rsp=...be8.  ___ needs no save here: it is the align-save register the chains manage themselves
 * (x86_align_enter/leave).  PROC-CONV converted the last call-regime citizen (LBL__ pseudo-procs,
 * rt_goto_transfer arm 4) to this same transfer; the donated-frame shim rt_callregime_run is deleted. */
__asm__(
".text\n"
".globl rt_chain_enter\n"
"rt_chain_enter:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  leaq 1f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
/* RTCC RC-4 INBOUND LOAD (RC-0(d) edge class 1): block→{rsi,rdi,r8,r9,r10,r11} before jmp into generated. */
/* BLOCK-CANONICAL LAW: a C→generated crossing LOADS the cache.  Gated on g_rtcc_on; GOT-indirect (PIC-safe). */
/* rax/rcx/rdx carry the live wires (fn ptr + return address) and MUST NOT be touched here.                   */
/* rsi/rdi (arg-tier) are loaded because they carry no live wire at this site.                                 */
/* Load order: r11=block ptr; rsi/rdi/r8/r9; r10 overwritten last (GOT scratch → r10 slot value).             */
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 2f\n"
"  movq rtccb@GOTPCREL(%rip), %r10\n"
"  movq 24(%r10), %rsi\n"
"  movq 32(%r10), %rdi\n"
"  movq 64(%r10), %r11\n"
"  movq 40(%r10), %r8\n"
"  movq 48(%r10), %r9\n"
"  movq 56(%r10), %r10\n"
"2:\n"
"  jmp *%rax\n"
"1:\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
/* ⛔⭐⭐ rt_chain_enter_v (s113) — THE VALUE-RETURNING TWIN, AND THE REASON IT HAS TO EXIST.
 * rt_chain_enter above implements Lon's s59 ruling verbatim ("no CALL/RET — a JUMP and a JUMP BACK"):
 * it wires γ→rcx, ω→rdx and `jmp`s, pushing NO return address.  That is correct for every citizen that
 * RUNS TO TERMINATION — rt_goto_transfer's CODE fragments and LBL__ pseudo-procs end at `:(END)` and exit
 * through the driver, so they never take the jump back and the missing way home is invisible.  EVAL is the
 * ONE citizen that must hand a value back, and it is the one that was broken.
 * ROOT CAUSE (s113, gdb-measured, not inferred): CARVE-KILL (ef9a7d2c/1ba33ea6) deleted xa_flat_prologue —
 * the producer half of the jmp-entry protocol.  The ONLY surviving site that stores the {γ,ω} wire header
 * is emit.cpp's flat_lcl_proc arm (~:2741/:2746).  A chain armed flat_jmp_entry=1 / flat_lcl_proc=0 (exactly
 * what emit_jmp_entry_for_chain produces) therefore gets NO prologue, NO wire header, GLUE-O suppressed by
 * its own flat_jmp_entry conjunct — and an epilogue that `ret`s.  Measured on `EVAL('1 + 2')`: flags
 * flat_jmp_entry=1 flat_frame_bytes=112 at emit_chain, yet the emitted chain is
 *   0x00 jmp α · 0x05 jmp ω · α: sub rsp,16 ×3 (its own FORTH spine) … add rsp,48 · mov eax,2 · RET
 * — no `sub rsp,112`, no `mov [rsp+kt-24],rcx`.  The `ret` pops rt_chain_enter's saved %r15 and jumps to it:
 * rip=_rtld_global, frame #1 = 0x41ad68.  That is the whole "omega_driver signature" of s103/s110/s112.
 * ⛔ THE CHAIN IS ALREADY A C-ABI CITIZEN — it ends in `ret` and returns DT in eax:rdx (eval_chain_fn is
 * literally DESCR_t(*)(void*,int)).  So the two halves of the protocol disagree, and the cheap correct move
 * is to give the ret-ending chain the return address it is already asking for, WITHOUT touching the stub the
 * terminating citizens use and WITHOUT moving one byte of generated code (MD5 radius 0 by construction).
 * ⚠ ALIGNMENT IS LOAD-BEARING AND IS WHY THIS IS `sub 8 + push` AND NOT A BARE 6TH PUSH.  The chain assumes
 * rsp ≡ 0 (mod 16) at its entry — its interior `call *%rax` sites are aligned off exactly that.  Five pushes
 * (40B) take the 8-mod-16 C entry to 0 mod 16; a 6th push alone would leave 8 mod 16 and SEGV in libc's SSE
 * printf path (measured at s59, rsp=...be8, recorded in rt_chain_enter's header — do not re-derive it).
 * `sub $8` + `push` moves 16 bytes: the landing sits at [rsp] for the chain's own `ret` AND entry stays
 * 0 mod 16.  The landing drops the 8-byte pad before unwinding the callee-saves.
 * rcx/rdx are still wired identically, so a chain that jumps back instead of returning is unaffected here. */
__asm__(
".text\n"
".globl rt_chain_enter_v\n"
"rt_chain_enter_v:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  leaq 3f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 4f\n"
"  movq rtccb@GOTPCREL(%rip), %r10\n"
"  movq 24(%r10), %rsi\n"
"  movq 32(%r10), %rdi\n"
"  movq 64(%r10), %r11\n"
"  movq 40(%r10), %r8\n"
"  movq 48(%r10), %r9\n"
"  movq 56(%r10), %r10\n"
"4:\n"
"  subq $8, %rsp\n"
"  pushq %rcx\n"
"  jmp *%rax\n"
"3:\n"
"  addq $8, %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
void rt_chain_enter_v(eval_chain_fn fn);
void rt_chain_enter(eval_chain_fn fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Deferred-expression thunks (`. *F(X)`) minted by a RUNTIME compile.  lower_snobol4 mints one EXPR$N proc per
 * deferred operand and files it in g_stage2; the driver does the register+emit walk for the main program, and
 * this is that same two-phase walk for the fragment path, over just the procs the fragment added [pc0, count).
 * Without it the dcap pump's rt_proc_call_open on EXPR$N finds no body and the conditional assignment is a
 * no-op.  dyn_scope=1 on every thunk, so the pump's open lands the dyn prologue and the NRETURN'd NAME rides
 * the rt_g_want_name the pump re-arms. */
int g_rt_fragment_emit = 0;   /* D-18b (s161): TRUE while the RUNTIME fragment compiler is emitting -- bb_call_proc_staged's TINY arms consult it and DECLINE, because a fragment's callees (EXPR$/PAT$ thunks and cross-chain mains) carry no <name>_α staging label for the alpha$ cell to seal against (measured: alpha$EXPR$0F1 / alpha$PAT$0 MISS under SCRIP_SEAL_DIAG); declined sites fall to the slim/legacy call = the rt fn-pointer machinery main programs already use for every thunk call (m1 witness family green). */
static void eval_thunks_emit_from(int pc0)
{
    extern void rt_proc_register(const char *name, const char **pnames, int nparams);
    extern void rt_proc_set_fn(const char *name, eval_chain_fn fn);
    extern void rt_proc_set_generator(const char *name, int is_gen);
    extern void rt_proc_set_variadic(const char *name, int is_var);
    extern void rt_proc_set_dyn_scope(const char *name, int v);
    extern void rt_proc_set_result_name(const char *name, const char *rname);
    extern void ir_drive_slot_assign(IR_graph_t *g);
    extern int g_gen_proc_active;
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        rt_proc_register(pname, (const char **)0, 0);
        rt_proc_set_generator(pname, g_stage2.proc_table[pi].is_generator);
        rt_proc_set_variadic(pname, g_stage2.proc_table[pi].is_variadic);
        rt_proc_set_rest_kind(pname, g_stage2.proc_table[pi].rest_kind);
        rt_proc_set_named_rest(pname, g_stage2.proc_table[pi].named_rest);
        rt_proc_set_dyn_scope(pname, g_stage2.proc_table[pi].dyn_scope);
        if (g_stage2.proc_table[pi].result_name) rt_proc_set_result_name(pname, g_stage2.proc_table[pi].result_name);
    }
    IR_graph_t *cfg_sv = g_emit_cfg;
    int fa = g_frame_active; g_frame_active = 1;
    int ga = g_gen_proc_active;
    g_rt_fragment_emit = 1;
    int b1c = 1; { const char *_be = getenv("SCRIP_B1C_PARITY"); if (_be && *_be == '0') b1c = 0; }   /* B1c (s168, HQ): fragment loop to DRIVER EMIT-CONTEXT PARITY — FINDING-2026-08-19-s168; DEFAULT ON since s169, killswitch SCRIP_B1C_PARITY=0. MEASURED s169 (b1c-flip seat) ON A PRISTINE BUILD AT f44be5f1, DRIVER AND .so FROM ONE COMMIT: 6-suite scorecard A/B, 1024 programs x 2 modes x 2 arms -- 9 movers, EVERY ONE crash->better, ZERO regressions (not one PASS left PASS in either mode); m3 PASS 956->962, m4 PASS 944->944 unchanged; mode-4 .s md5 blast radius 0 movers / 527 comparable, so the default arm's emitted code is byte-identical by measurement, not just by construction. `=0` restores the pre-flip behaviour verbatim (BASELINE-ARM law). Residue owned elsewhere: m4 still SEGVs at the seam (queue row b1c-m4-seam, FINDING s168 R1) and three retreat witnesses run clean but answer match where the oracle retreats to nomatch (queue row b1c-retreat, R2) -- both are crash->non-crash moves here, neither is a regression. */
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        ir_drive_slot_assign(g_stage2.bbp.table[idx]);
        g_emit_cfg = g_stage2.bbp.table[idx];
        g_gen_proc_active = g_stage2.proc_table[pi].is_generator;
        if (b1c) { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }   /* B1c parity: driver m3 loop scrip.c:1664 */
        if (b1c) { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = g_stage2.bbp.table[idx]; g_flat_frame_floor = 0;
            if (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED)) {
                for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
                    int _mx = g_stage2.proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]); break; } } }   /* B1c parity: SN4-FLAT-PROC floor, scrip.c:1671; cleared by emit_jmp_entry_clear */
        { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
          int _isp = emit_jmp_entry_for_patproc(pname, g_stage2.bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, g_stage2.proc_table[pi].dyn_scope, g_stage2.proc_table[pi].is_generator, g_stage2.bbp.table[idx]);
          if (b1c) g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, g_stage2.proc_table[pi].nparams)) ? g_stage2.proc_table[pi].nparams : -1; }   /* B1c parity: PL-DC arming, scrip.c:1674 */
        eval_chain_fn pfn = emit_chain(g_stage2.bbp.table[idx]->entry, NULL, "proc_flat");   /* B1c: ->entry is CORRECT here, bb_proc_entry RULED OUT by asm-diff (s165: emitting from proc_entry_node drops the thunk's entry prologue -- the 0x70 frame carve + rcx/rdx wire save -- and the body falls into slab zeros); fragment graphs are self-owned, never main-shared, so the s176 shared-graph rationale does not apply */
        if (pfn) rt_proc_set_fn(pname, pfn);
        { extern void bb_ab_seal_entry_cells(const char *, void *, int); if (pfn) bb_ab_seal_entry_cells(pname, (void *)pfn, 1); }   /* ⭐⭐⭐ D-18a (s161): THE MISSING LINE — the main driver seals alpha$<FN> for every proc it emits (scrip.c R-1 s94 loop); this fragment loop registered and set fns but never sealed, so alpha$EXPR$<thunk> stayed rt_ab_undef_fn_stub and every deferred call inside an EVAL-built pattern raised error 22 at match time (B1b witness; beauty's grammar is built of exactly these). Label pool is live here — emit_jmp_entry_clear below is the same boundary the driver seals across. */
        if (b1c && pfn) { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }   /* B1c parity: scrip.c:1680 — teardown/unwind reads the frame geometry */
        if (b1c && pfn) { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); rt_proc_set_zstatic(pname, g_last_flat_zstatic); }   /* B1c parity: PS-1b, scrip.c:1683 — DT_P carries real zstatic */
        if (b1c && pfn) { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }   /* B1c parity: PS-3, scrip.c:1684 — ζ suspension footprint for DT_P targets */
        if (b1c && pfn) { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); if (g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *)((char *)pfn + g_last_dc_off)); }   /* B1c parity: PL-DC seal, scrip.c:1685 */
        emit_jmp_entry_clear();
    }
    g_rt_fragment_emit = 0;
    g_gen_proc_active = ga; g_frame_active = fa; g_emit_cfg = cfg_sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_build_chain(const char *s)
{
    if (!s || !*s) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    size_t n = strlen(s);
    char *src = (char *)rt_ws_alloc(n + 4);
    if (!src) return NULL;
    snprintf(src, n + 4, "(%s)", s);
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *e = parse_expr_pat_from_str(src);
    sno_error_quiet_end();
    if (!e) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_ws_strdup_c(cap); return NULL; }
    tree_t *var = ast_stmt_new(TT_VAR);
    var->v.sval = rt_ws_strdup(EVAL_TMP);
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 1));
/* ⭐ s115 MON-CAP -- THE EVAL CHAIN'S LABEL EVENT HAS NO SPITBOL COUNTERPART.  eval_build_chain stamps its SYNTHESIZED statement :stno 1, so the bb_statement monitor tap fires `LABEL
 * stno=INT=1` every time a chain executes -- an event the oracle never emits, because the oracle has no such statement.  EVERY EVAL-BEARING PROGRAM THEREFORE "DIVERGES" AT THE CHAIN'S FIRST
 * STATEMENT WHETHER OR NOT IT IS CORRECT: s114 measured ev_fn_literal reporting the identical step-5 divergence before AND after a real fix, so the monitor could not grade the fix that cured
 * it.  This is the exact disease the MON-RE GOTO-tap note at emit.cpp:1189 already records ("a LABEL at PURE WIRING nodes has no counterpart in SPITBOL's wire, so every program -- passing ones
 * included -- diverged within 3 steps and the monitor could not bracket ANY bug"), one node class over.  RULES.md: a monitor blind to the divergence CLASS must be extended before the hunt it
 * is blind to.  ONE AUTHORITY, NOT A SECOND RULE: the tap staging at emit.cpp:1202 already reads `op_stno > 0`, so a synthesized statement only has to stop CLAIMING to be source statement 1 --
 * no new suppression predicate, no monitor-side filter, nothing to keep in sync with the emitter.  0 is the honest value: this statement has no source counterpart.  Killswitch
 * SCRIP_MON_CHAIN_STNO=1 restores the old claim for A/B.  Beauty is EVAL-bearing, so this stands between the monitor and Milestone 1. */
    { static int _cs = -1; if (_cs < 0) { const char * e = getenv("SCRIP_MON_CHAIN_STNO"); _cs = (e && e[0] == '1') ? 1 : 0; }
      ast_push(st, ast_attr_int(":stno", _cs ? 1 : 0)); }
    ast_push(st, ast_attr_expr(":subj", var));
    ast_push(st, ast_attr_leaf(":eq", ""));
    ast_push(st, ast_attr_expr(":repl", e));
    tree_t *prog = ast_stmt_new(TT_PROGRAM);
    ast_push(prog, st);
    extern void sno_expr_salt_next(void);
    extern int sno_expr_mark(void);
    extern void sno_expr_thunks_build(int x0);
    extern int sno_pat_count(void); extern void sno_pat_thunks_build(int p0);
    sno_expr_salt_next();
    int xm = sno_expr_mark();
    int pat0 = sno_pat_count();
    int pc0 = g_stage2.proc_count;
    void *g = lower_snobol4(prog);
    if (!g) { ast_tree_free_dyn(prog); return NULL; }
    sno_expr_thunks_build(xm);
    if (sno_pat_count() > pat0) sno_pat_thunks_build(pat0);   /* BLOCKER-C (s144): EVAL expr may mint PAT$N — build proc thunks so eval_thunks_emit_from below emits+registers them (else SNO$MKPAT miss) */
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int fa = g_frame_active; g_frame_active = 1;
    emit_jmp_entry_for_chain((IR_graph_t *)g);
    g_rt_fragment_emit = 1;
    eval_chain_fn fn = emit_chain(((IR_graph_t *)g)->entry, NULL, "pat_flat");
    g_rt_fragment_emit = 0;
    emit_jmp_entry_clear();
    g_frame_active = fa; g_emit_cfg = cfg_sv;
    eval_thunks_emit_from(pc0);
    IR_free_dyn(g);
    ast_tree_free_dyn(prog);
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVAL-CHAIN-RET (this session): CLASS C chains (EVAL/CODE JIT fragments) exit via bb_glue_outer_γ/ω which
 * does mov rsp,___; pop ___; ret.  The ret lands in eval_string_transient (the caller of THIS function)
 * after "call eval_chain_run_capture" — skipping this function's NV_GET_fn(EVAL_TMP) call entirely.
 * The chain's γ return value is eax=DT_S, rdx=garbage — unusable as a DESCR.
 * FIX: eval_string_transient reads ZZEVALZZ directly after calling eval_chain_run_capture.
 * THIS function now only exists to correctly set up ___ as the eval_chain_run_capture frame base
 * so the chain's "mov rsp,___; pop ___; ret" correctly unwinds to eval_string_transient.
 * __attribute__((noinline)) ensures a real call frame with its own ___. */
__attribute__((noinline))
static void eval_chain_enter_only(eval_chain_fn fn) {
    static int _rv = -1; if (_rv < 0) { const char * e = getenv("SCRIP_EVAL_RET"); _rv = (e && *e == '0') ? 0 : 1; }
    if (_rv) { rt_chain_enter_v(fn); return; }
    rt_chain_enter(fn);
    /* chain ret lands in eval_string_transient; this line is never reached */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ B2c ROOT CAUSE (s172, queue row beauty-m3-zls): AN EVAL THAT RETURNS A PATTERN HANDS BACK A VALUE LIVING INSIDE THE CHAIN'S OWN JIT BLOB, so releasing that blob when the EVAL returns is a
 * USE-AFTER-FREE, not a memory economy.  The old line read `mark < EVAL_RETAIN_BUDGET`, and `mark` is the WHOLE pool frontier -- the program's own compiled code included -- so the predicate
 * answered "is this program's code smaller than 2MB?", never "how much has EVAL retained?".  MEASURED on beauty: its m3 blobs seal at 0x202000 = 2,105,344 bytes, 8,192 PAST the 2MB budget, so
 * EVERY EVAL in beauty-m3 took the release arm; the pattern built by semantic.inc's `shift = EVAL("p . thx . *Shift('" t "', thx)")` was dangling the instant it was assigned, and the first
 * Shift/Reduce match jumped to pool_top (0x7fffee202000: page-aligned, re-mprotect'ed RW by bb_pool_release, hence SIGSEGV on instruction FETCH, with rax == rip == the DTP's fn field).  m4
 * escaped only because its code lives in the ELF and not in the pool, so mark stayed near 0 and the same chains were retained -- the mode asymmetry was an ACCOUNTING artifact, not a dirty quad
 * (the `zls_g_region` frame the s170 census read is a stale return address in dirty stack that gdb prints as frame #1 -- a fingerprint of the dirty spine, never a caller).  DEFAULT IS NOW
 * RETAIN: the chain is cached and its blob kept for the life of the process, which is exactly what every program under 2MB already got.  Killswitch `SCRIP_EVAL_RETAIN=<bytes>` sets the budget
 * for A/B -- =2097152 restores the historic 2MB cliff verbatim, =0 releases every chain (the class in one command: witness corpus/probe/b2c/b2c_eval_pat_release.sno, 7 lines, PASS at default,
 * SIGSEGV at =0).  Pool exhaustion stays bounded and now fails HONESTLY: bb_alloc returns NULL at the 64MB reserve, eval_build_chain returns NULL, and the EVAL fails instead of jumping into
 * freed pages. */
static size_t eval_retain_budget(void) { static long v = -1; if (v < 0) { const char *e = getenv("SCRIP_EVAL_RETAIN"); v = (e && *e) ? atol(e) : -1; } return v < 0 ? ~(size_t)0 : (size_t)v; }
static int eval_chain_run_guarded(eval_chain_fn fn) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    static int _ef = -1; if (_ef < 0) { const char *e = getenv("SCRIP_EVAL_FAILS"); _ef = (e && *e == '0') ? 0 : 1; }
    if (!_ef) { eval_chain_enter_only(fn); return 1; }
    int my = g_core_errjmp_n++; long esv = g_error; g_error = -1;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; g_error = esv; return 0; }
    eval_chain_enter_only(fn);
    g_core_errjmp_n = my; g_error = esv; return 1;
}   /* ⭐⭐⭐ CN-EVAL-FAILS (this seat, queue row `cn-oracle-rulings`; Lon s168 "use SPITBOL as an Oracle") -- MANUAL v3.7 p.131, "EVAL fails if evaluation of its argument fails", MADE TRUE FOR THE RUN HALF. SCRIP already honoured p.131 for the COMPILE half: a fragment that will not parse returns a NULL chain and the EVAL takes :F (measured, EVAL('3 +') fails on both engines at HEAD). What it did NOT survive was an error raised while the fragment RAN -- core_runtime_error printed and exit(1)'d, killing the whole program where the oracle merely fails the statement and runs on. Measured on live x64 sbl: EVAL('&NEVERSET') -> :F &ERRTYPE 251, EVAL('UNDEFINEDFN(3)') -> :F &ERRTYPE 22, EVAL('1 / 0') -> :F &ERRTYPE 14, and 'reached-end' prints in every case. ⛔ WHY g_error IS BORROWED AND NOT A NEW FLAG: core_runtime_error's conversion arm is gated on g_error, the arm this boundary needs, and the FACT RULE forbids minting a global to say the same thing twice. It is SAVED AND RESTORED on both exits (esv) and on the catch path, so the borrow is invisible to any frame outside this one, and -1 is chosen deliberately -- the conversion arm decrements only `g_error > 0`, so an unlimited grant neither spends a credit nor perturbs a program that set the cell itself. The push/pop discipline (`my` snapshot, restore to `my` not a decrement) is by_name_dispatch.c:1483's, so a longjmp from a DEEPER frame that skipped its own pop cannot strand this one. ⛔ THE POOL AND THE CACHE ARE DELIBERATELY UNTOUCHED BY FAILURE: a chain that failed at RUN time is still a VALID compiled artifact -- it failed on program state, not on its own code -- so it caches and releases exactly as a successful one does, which keeps memory behaviour byte-identical to HEAD and keeps a failing EVAL in a loop from growing the pool. Releasing the blob on the failure path was the tempting alternative and is the s172 B2c use-after-free wearing a different hat. Killswitch SCRIP_EVAL_FAILS=0 restores the abort for A/B. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_string_transient(const char *s) {
    if (!s || !*s) return NULVCL;
    eval_chain_fn cached = eval_cache_get(s);
    if (cached) {
        DESCR_t saved = NV_GET_fn(EVAL_TMP);
        int ok = eval_chain_run_guarded(cached);
        DESCR_t result = ok ? NV_GET_fn(EVAL_TMP) : FAILDESCR;
        NV_SET_fn(EVAL_TMP, saved);
        return result;
    }
    size_t mark = bb_pool_mark();
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) { bb_pool_release(mark); return FAILDESCR; }
    DESCR_t saved = NV_GET_fn(EVAL_TMP);
    int ok = eval_chain_run_guarded(fn);
    DESCR_t result = ok ? NV_GET_fn(EVAL_TMP) : FAILDESCR;
    NV_SET_fn(EVAL_TMP, saved);
    if (mark < eval_retain_budget()) eval_cache_put(s, fn);
    else bb_pool_release(mark);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_node(tree_t *e)
{
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_node: AST-walk evaluator deleted; nothing interprets tree_t at runtime\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_expr(const char *src)
{
    if (!src || !*src) return NULVCL;
    tree_t *tree = parse_expr_pat_from_str(src);
    if (!tree) return FAILDESCR;
    return eval_node(tree);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVAL/CODE (manual Ch.9, directive lifted 2026-07-08).  Runtime label registry: fragment labels registered by
 * code(); resolution order in rt_goto_transfer is (1) `$X` indirect deref, (2) END, (3) this registry — so a
 * fragment label OVERRIDES a same-named main label per the manual, (4) the main program's LBL__ pseudo-procs
 * (exported by lower_sno_stage2 when the program uses CODE), (5) a variable holding a CODE value (the lexer
 * folds direct-goto `:<C>` onto the plain-name form, so `C` here may be the variable), (6) fault.  A transfer
 * RUNS the target nested on a fresh 64KB frame (GC-visible so DESCR temporaries in it stay rooted); SNOBOL4
 * gotos never resume their source, so the target running to termination cascades clean returns back up every
 * crossing — the process exits through the driver as always.  Honest slice-1 caveats: one frame is allocated
 * per crossing and never freed, and each crossing nests one C-stack level, so a loop that ping-pongs across
 * the main/fragment boundary (label-to-label, not within one graph) grows both without bound — fine for the
 * manual's shapes (a handful of crossings), a real rung for a frame-recycling tail-transfer later. */
typedef struct { char *key; eval_chain_fn fn; } lbl_ent_t;
static lbl_ent_t *g_lbl_tab = NULL;
static int        g_lbl_n = 0;
static int        g_lbl_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_label_set_fn(const char *name, void *fn) {
    if (!name || !*name) return;
    for (int i = 0; i < g_lbl_n; i++) if (!strcmp(g_lbl_tab[i].key, name)) { g_lbl_tab[i].fn = (eval_chain_fn)fn; return; }
    if (g_lbl_n >= g_lbl_cap) {
        int ncap = g_lbl_cap ? g_lbl_cap * 2 : 16;
        lbl_ent_t *nt = (lbl_ent_t *)rt_ws_realloc(g_lbl_tab, (size_t)ncap * sizeof(lbl_ent_t));
        if (!nt) return;
        g_lbl_tab = nt; g_lbl_cap = ncap;
    }
    g_lbl_tab[g_lbl_n].key = rt_ws_strdup(name);
    g_lbl_tab[g_lbl_n].fn  = (eval_chain_fn)fn;
    g_lbl_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn rt_label_get_fn(const char *name) {
    if (!name || !*name) return NULL;
    for (int i = 0; i < g_lbl_n; i++) if (!strcmp(g_lbl_tab[i].key, name)) return g_lbl_tab[i].fn;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GOTO_FRAME_BYTES (64 * 1024)
void rt_goto_transfer(const char *name)
{
    if (!name || !*name) return;
    if (name[0] == '$') {
        DESCR_t iv = NV_GET_fn(name + 1);
        const char *inm = VARVAL_fn(iv);
        if (!inm || !*inm) { fprintf(stderr, "[SNO] transfer to undefined label: $%s (indirect name is null)\n", name + 1); exit(1); }
        rt_goto_transfer(inm);
        return;
    }
    if (!strcmp(name, "END")) return;
    eval_chain_fn fn = rt_label_get_fn(name);
    if (fn) { rt_chain_enter(fn); return; }
    {
        extern void *rt_proc_get_fn(const char *);
        char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", name);
        fn = (eval_chain_fn)rt_proc_get_fn(lname);
        if (fn) { rt_chain_enter(fn); return; }
    }
    {
        DESCR_t d = NV_GET_fn(name);
        if (d.v == DT_C && d.slen == 3) { rt_chain_enter((eval_chain_fn)d.ptr); return; }
    }
    fprintf(stderr, "[SNO] transfer to undefined label: %s\n", name); exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    extern IR_graph_t *sno_lower_fragment_at(const tree_t *prog, int entry_idx);
    extern const char *sno_stmt_label(const tree_t *s);
    extern int g_frame_active;
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *prog = sno_parse_string_ast(src, NULL);
    sno_error_quiet_end();
    if (!prog || prog->n == 0) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_ws_strdup_c(cap); return FAILDESCR; }
    /* BLOCKER-C (s144): a runtime CODE fragment that contains a pattern match lowers through sno_lower_fragment_at,
     * whose sno_pat_collect walk MINTS new PAT$N entries (continuing g_sno_npat past the main-program set) but —
     * unlike lower_sno_stage2 — never turns them into emitted+registered proc_table blobs.  At match time the
     * fragment body calls SNO$MKPAT("PAT$N") → rt_proc_get_fn → NULL → "compiled pattern blob not registered".
     * Capture the pattern + proc watermarks BEFORE lowering; after every fragment body is lowered+emitted, build
     * proc_table thunks for the newly-collected patterns (sno_pat_thunks_build) and emit+register them through the
     * same eval_thunks_emit_from path the EVAL arm uses.  Mirrors main's sno_pat_thunks_build(0) + driver loop. */
    extern int sno_pat_count(void); extern void sno_pat_thunks_build(int p0);
    int pat0 = sno_pat_count();
    int proc0 = g_stage2.proc_count;
    eval_chain_fn first = NULL;
    int k = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *c = prog->c[i];
        if (!c || c->t != TT_STMT) continue;
        const char *lbl = sno_stmt_label(c);
        if (k == 0 || (lbl && lbl[0])) {
            IR_graph_t *g = sno_lower_fragment_at(prog, k);
            if (!g) return FAILDESCR;
            extern IR_graph_t *g_emit_cfg;
            IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = g;
            int fa = g_frame_active; g_frame_active = 1;
            emit_jmp_entry_for_chain(g);
            eval_chain_fn fn = emit_chain(g->entry, NULL, "code_flat");
            emit_jmp_entry_clear();
            g_frame_active = fa; g_emit_cfg = cfg_sv;
            if (!fn) return FAILDESCR;
            if (k == 0) first = fn;
            if (lbl && lbl[0]) rt_label_set_fn(lbl, (void *)fn);
        }
        k++;
    }
    if (sno_pat_count() > pat0) { sno_pat_thunks_build(pat0); eval_thunks_emit_from(proc0); }
    if (!first) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_C;
    d.slen = 3;
    d.ptr  = (void *)first;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t EXPVAL_fn(DESCR_t expr_d)
{
    if (expr_d.v == DT_E) {
        if (expr_d.slen == 3) {
            eval_chain_fn fn = (eval_chain_fn)expr_d.ptr;
            if (!fn) return FAILDESCR;
            DESCR_t saved = NV_GET_fn(EVAL_TMP);
            rt_chain_enter(fn);
            DESCR_t result = NV_GET_fn(EVAL_TMP);
            NV_SET_fn(EVAL_TMP, saved);
            return result;
        }
        if (expr_d.slen == 1) {
            extern DESCR_t sm_eval_subexpr(int entry_pc);
            int entry_pc = (int)expr_d.i;
            return sm_eval_subexpr(entry_pc);
        }
        if (expr_d.slen == 2) {
            fprintf(stderr, "[SMX] FATAL: eval_code DT_E thunk path used the global value stack, "
                            "which is removed. This SM-era code path is not on Byrd Boxes. "
                            "Aborting (by design).\n");
            abort();
        }
        if (!expr_d.ptr) return FAILDESCR;
        const char *save_Σ = Σ;
        int         save_Ω = Ω;
        int         save_Δ = Δ;
        NAME_ctx_t eval_ctx;
        NAME_ctx_enter(&eval_ctx);
        DESCR_t result = eval_node((tree_t *)expr_d.ptr);
        NAME_ctx_leave();
        Σ = save_Σ;
        Ω = save_Ω;
        Δ = save_Δ;
        return result;
    }
    if (expr_d.v == DT_C) {
        if (expr_d.slen == 3) {
            eval_chain_fn fn = (eval_chain_fn)expr_d.ptr;
            if (!fn) return FAILDESCR;
            rt_chain_enter(fn);
            return NULVCL;
        }
        return NULVCL;
    }
    const char *s = VARVAL_fn(expr_d);
    if (!s || !*s) return NULVCL;
    return eval_expr(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t CONVE_fn(DESCR_t str_d)
{
    const char *s = VARVAL_fn(str_d);
    if (!s || !*s) return FAILDESCR;
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_E;
    d.slen = 3;
    d.ptr  = (void *)fn;
    return d;
}
