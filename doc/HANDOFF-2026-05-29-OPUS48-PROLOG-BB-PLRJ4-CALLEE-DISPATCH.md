# HANDOFF — PLR-J-4: native multi-predicate dispatch (callee-block sweep + call protocol)

**Date:** 2026-05-29
**Model:** Opus 4.8
**Goal:** GOAL-PROLOG-BB.md — PLR-J-4
**SCRIP parent:** `1aa0b3c5`

## Summary

Landed PLR-J-4a (the `bb_pl_call.cpp` MEDIUM_BINARY call protocol) and PLR-J-4b
(the callee-block sweep in the `SM_BB_PL_INVOKE` MEDIUM_BINARY arm) in lockstep,
plus a small piece of cross-block-linkage infrastructure. **Multi-predicate
single-clause Prolog programs now run natively in mode-3 (`--run`)** — previously
the DEFER GUARD aborted any program with more than one user predicate.

## What changed (4 files, +237/-10)

1. **`src/emitter/emit_core.c` / `.h` — `emit_label_intern(name)` (NEW, pure infra,
   zero x86 bytes).** Name-keyed lookup over the session label arena: same name →
   same `bb_label_t*`. This is the cross-block linkage primitive. The BB_PL_CALL
   site (`bb_pl_call.cpp`) and the callee-block definition site (`SM_BB_PL_INVOKE`
   sweep) both call `emit_label_intern(".Lplpred_<name>_<arity>")` and so share ONE
   `bb_label_t*`, which `bb_label_define` resolves by pointer identity. Without it,
   the two sites would `emit_label_alloc` distinct objects and the forward `call`
   would never resolve (`bb_emit_end` would abort on the unresolved patch).

2. **`src/emitter/BB_templates/bb_pl_call.cpp` — PLR-J-4a.** Replaced the
   double-jump BINARY stub (`jmp ω; jmp ω`) with the full call protocol, a byte
   twin of the MEDIUM_TEXT arm: build each caller-side arg Term (`build_arg_bin`,
   BB_PL_STRUCT → `emit_build_compound_term_bin` from PLR-J-3) and push; allocate
   + push callee env via `pl_bb_env_save_push`; align; bind callee slots from the
   stashed Term*s via `pl_bb_bind_arg`; `call .Lplpred_<name>_<arity>` (cross-block
   forward ref via `emit_label_intern`); test `rt_last_ok` → γ/ω with env install +
   `rt_pl_cp_save_caller_env`; β redo path (`pl_cp_current`, reinstall `cp->env`
   [+24], `call <label>_redo`, reinstall `cp->saved_args` [+40]). Internal labels
   (`_fail5`, β-entry, `_nosol`) hand-patched; γ/ω + the two cross-block calls go
   through the `bb_bin_t` descriptor. `pl_choice` offsets verified against the
   struct: `env`=24, `saved_args`=40.

3. **`src/emitter/SM_templates/sm_bb_switch.cpp` — PLR-J-4b + entry-β fix.**
   - Removed the M3-PL-NOINTERP-1a DEFER GUARD (`others>0` bail).
   - Added the callee-block sweep into the SAME scratch buffer the entry walk uses:
     for every other user predicate, `bb_label_define(emit_label_intern(blbl))`,
     `pl_emit_callee_block_body` (flat walk, medium-agnostic → bytes in BINARY),
     γ/ω `rt_set_last_ok`+`ret` epilogue, and a `<label>_redo: jmp β` entry. A
     leading `jmp .Lplcallees_end` skips the block bodies on entry fall-through.
   - **Entry-β definition (latent fix):** the entry passed `&plβ` to `walk_bb_flat`
     but never defined it (true in TEXT too). Any entry body with a resumable node
     (BB_PL_CALL/CHOICE/ALT) left `.Lplent_β` unresolved → `bb_emit_end` abort.
     Now `plβ` is defined as `jmp plω` (program-level no-more-solutions → fail).
   - **MULTI-CLAUSE GUARD (PLR-J-5 boundary):** `bb_pl_choice.cpp` BINARY is still a
     double-jump stub, so a multi-clause predicate (entry `t==BB_CHOICE`) would walk
     to an unresolved choice label. Until PLR-J-5, detect any BB_CHOICE-headed
     predicate (entry or callee) and bail HONESTLY (`g_sm_native_unsupported`) so
     `--run` aborts cleanly rather than crashing the emitter.

## Verification

3-mode agreement probes (flat arithmetic, the PLR-J-4 scope), all AGREE m2==m3:
- `a:-b. b:-c. c:-write(chained),nl.` → `chained`
- `dbl(X,Y):-Y is X*2. go:-dbl(5,A),dbl(A,B),write(B),nl.` → `20`
- `calc(X,Y,R):-R is X+Y,write(R),nl. go:-calc(11,22,_).` → `33`
- `echo2(X,Y):-write(X),nl,write(Y),nl. go:-echo2(11,22).` → `11/22`

Multi-clause (`count/1`) aborts honestly (`native emit unimplemented`), no crash.

**Pre-existing bug found (NOT PLR-J-4, orthogonal):** mode-3 native `is/2` with a
NESTED arithmetic expression (`R is 3*10+4`) gives `6` not `34` — confirmed at the
baseline commit `1aa0b3c5` with my changes stashed, single predicate, no arg
passing. The native `is` arm evaluates only part of a nested expression tree. This
is a separate rung (CAT-D / WAM `is` nested-expr fix), independent of dispatch.

## Gates (all byte-identical to baseline `1aa0b3c5`, ZERO regressions)

- GATE-1 smoke: 5/5
- GATE-2 crosscheck: 11 PASS / 121 (unchanged — its multi-mode programs are
  multi-clause [PLR-J-5 guard] or hit the nested-is bug)
- GATE-3 mode-2: 104/107 (byte-identical — mode-2 untouched)
- GATE-4 mode-4 minimal: 4/4
- GATE-SWI: 57/57 (100%)
- FACT: 0 violations (arm2 = 12 baseline; all bytes inside `*_templates/`;
  `emit_label_intern` is pure label bookkeeping, zero x86 bytes)
- siblings: icon 5/5, raku 5/5, snobol4 13/13

## NEXT

- **PLR-J-5** — `BB_CHOICE` BINARY arm (`bb_pl_choice.cpp`) as `ir_a_Alt`
  transliteration + gprolog retry/trust ordering (untrail-before-retry, trust pops
  CP). This removes the MULTI-CLAUSE GUARD and unblocks recursive/multi-clause
  predicates (`count/1` etc.) — the bulk of the 121 open mode-3 crosscheck failures.
  Depends on PLR-J-2 (explicit resume ✅) and PLR-J-4 (callee blocks to backtrack
  into ✅).
- **Orthogonal:** mode-3 native nested-`is` expression evaluation fix (separate
  rung). The GATE-2 crosscheck count won't move much until BOTH PLR-J-5 and the
  nested-`is` fix land, since most crosscheck programs need one or the other.
