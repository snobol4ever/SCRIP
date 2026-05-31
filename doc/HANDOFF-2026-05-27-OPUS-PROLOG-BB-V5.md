# HANDOFF 2026-05-27 (Opus 4.7) — GOAL-PROLOG-BB V-5: retire AGW-1c

## Summary

V-5 LANDED (uncommitted at session end — committed in this handoff). Prolog `--run` no longer
routes through `sm_interp_run` + the C walker; it now compiles to flat x86 via `sm_codegen_text`,
forks an external `as` + `gcc` pipeline, and `execv`s the resulting binary in a child process.
The AGW-1c exception is retired from the code path; the corresponding comment block in
`scrip.c:422-432` is replaced.

V-5 immediately exposed real mode-3/mode-4 emitter gaps that the old `--interp ≡ --run` fake
parity was hiding. GATE-2 (crosscheck `--interp` vs `--run`) drops from a fake 132/0 to a real
36/96. This is **truth-telling, not regression** — the prior 132/0 was meaningless because both
sides walked the same C code. The new 96 failures are the genuine PL-DEBT-1 ledger.

Bonus: a one-line fix in `emit_sm.c` (add `BB_PL_STRUCT` to `pl_ir_kind_uses_sval`) eliminates
the `.S_ERR` undefined-symbol class of linker errors on programs that unify compound terms.

## Gates at handoff HEAD

| Gate | Before V-5 | After V-5 | Note |
|------|-----------:|----------:|------|
| GATE-1 smoke | 5/5 | 5/5 | held |
| GATE-2 crosscheck (`--interp` vs `--run`) | 132/0 (fake) | **36/96** (real) | new floor — see PL-DEBT-1 |
| GATE-3 rung suite (`--interp`) | 88/107 | 88/107 | held |
| GATE-4 mode-4 rung (`--compile --target=x86`) | 4/4 | 4/4 | held |
| Icon / Snocone / Raku / Rebus smoke | 5/5/5/4 | 5/5/5/4 | held |
| FACT RULE grep | 0 | 0 | held |

## Code changes

### `src/driver/scrip.c`
- Added `<sys/wait.h>`, `<errno.h>`, `<libgen.h>` includes.
- New static helpers (FACT-RULE compliant — no SM/BB walked at runtime; all bytes from templates):
  - `scrip_locate_rt_lib(out, cap)` — finds `libscrip_rt.so` via `$SCRIP_RT_LIB` env, else
    `/proc/self/exe` dirname + `out/`, `../out/`, or `lib/` siblings.
  - `scrip_spawn_wait(prog, argv)` — fork + execvp + waitpid wrapper; propagates exit/signal.
  - `scrip_run_via_x86_pipeline(s2, input_path)` — full pipeline:
    1. mkdtemp `/tmp/scrip_run_XXXXXX`, chdir into it (CWD-relative `bb_macros.s` lands here).
    2. Open `prog.s`, call `sm_codegen_text(&s2->sm, fas, input_path)`, close.
    3. `stage2_free_bb_after_emit(s2)` — BB graphs gone before child runs (per mode-4 contract).
    4. `as --64 -o prog.o prog.s` (spawn + wait).
    5. `gcc -no-pie -o prog.bin prog.o <rtlib> -Wl,-rpath,<rtdir> -lgc -lm -lstdc++` (spawn + wait).
    6. chdir back to original CWD; spawn `prog.bin` + wait; return child exit status.
- `scrip.c:422-432` Prolog `--run` branch rewritten: the AGW-1c comment block ("for Prolog,
  Mode 3 (--run) routes through sm_interp_run …") is REPLACED with the V-5 comment, and the
  `sm_run_with_recovery(&s2->sm, sm_interp_run)` call is replaced with
  `scrip_run_via_x86_pipeline(s2, input_path)` followed by `stage2_free_sm_bb(s2)`.

### `src/emitter/emit_sm.c`
- `pl_ir_kind_uses_sval` — added `BB_PL_STRUCT` case. Functor sval names (e.g. `"f"` from
  `f(X,a)`) are now pre-interned by `pl_pre_intern_pred_names`, so `bb_pl_intern_into` no longer
  falls back to `strtab_label`'s `.S_ERR` sentinel under compound-term unify. Verified by
  recompiling rung03_unify_unify.pl: `grep S_ERR` count drops from 2 to 0.

## What V-5 exposed (PL-DEBT-1, the real ledger)

The 96 GATE-2 failures break down into a small number of structural categories. Sampled diagnoses:

### CAT-A — `BB_PL_SEQ`-in-`BB_PL_ALT` α channel bug (largest single category)

**Symptom:** `( G1, G2, …, Gn ; H )` only emits `G1` then jumps to the alt's γ; `G2..Gn` are
silently dropped. Hits every program that uses `;` disjunction with a multi-goal left branch
(rung02_facts, rung05_backtrack via `member/fail ; true`, and many puzzle/builtin tests).

**Root cause:** `lower_pl.c:213` returns the conjunction's α as `*α_out = gα[0]` (the first
goal), not `*α_out = seq` (the BB_PL_SEQ wrapper). When the disjunction lowering at
`lower_pl.c:222-228` sets `nd->α = aα = gα[0]`, the alt's left branch becomes the first goal
directly, bypassing the SEQ wrapper. Mode-2 interp executes step-by-step via `bb_exec_node` so
the bug surfaces *less* — but it's actually a latent issue there too (single-step
`bb_exec_node(gα[0])` only runs one step of the conjunction in the alt case). Mode-4 flat-emit
exposes it sharply: `flat_drive_pl_alt → walk_bb_flat(gα[0], outer_γ, …)` dispatches to
`FILL(BB_PL_CALL, …)`, which emits only that one call with outer_γ as the success target.

**Fix (proposed, NOT applied):** `lower_pl.c:213` → `*α_out = seq;`. Verify interp first
(`bb_exec_node(seq)` returns `seq->α = gα[0]`, which the outer `bb_exec_once` loop then drives —
but `BB_PL_ALT` uses *direct* `bb_exec_node`, not the outer loop, so its handling needs to
mirror what `flat_drive_pl_alt` does or fall through to a small loop). Pair the fix with a
gate (rung02_facts under `--interp` and `--run` both producing `brown\njones\nsmith`).
Estimated impact on GATE-2: +20-40 PASS.

### CAT-B — Compound-term unify binds nothing (rung03 class)

**Symptom:** `f(X, a) = f(b, Y), write(X), write(' '), write(Y)` prints `_ _` instead of `b a`.

**Root cause:** `bb_unify.cpp`'s `build_term_text` calls `rt_pl_node_to_term(kind, ival, sval,
dval)` with only the operand's *own* fields — for a `BB_PL_STRUCT`, that's `kind=58, ival=2,
sval="f"`. The struct's argument nodes (hanging off α + γ-chain per `BB.h:84`) are never built or
attached. The runtime constructs a 2-arity compound with no arg terms, so the unify trivially
succeeds without binding X/Y.

**Fix sketch:** `build_term_text` (or a sibling) needs to recursively materialize compound
terms: walk `nd->α` + `nd->α->γ` + … for `ival`=arity args, build each via `rt_pl_node_to_term`,
push on stack, then call a new runtime helper `rt_pl_compound_build(functor_label, arity, args)`
that pops them in order. Effect-helper class (matches `rt_pl_arith` precedent — no port logic).
Estimated impact: +10-20 PASS (every program that does compound unification).

### CAT-C — List/cons walking + `member/2` segfault (rung05 class)

**Symptom:** `member(X, [a,b,c]), write(X), fail ; true` segfaults in the child process.

**Root cause (suspected, not confirmed):** lists lower as nested `BB_PL_STRUCT` cons-cells
(`lower_pl.c:80`). Same compound-term gap as CAT-B, plus `member/2` walks the cons chain via
recursive `BB_PL_CALL`. The recursive call's arg-passing (env push/save/restore in
`bb_pl_call.cpp`) may not unify the head/tail slots correctly when the arg is a cons compound
with unbound tail variable. Needs `gdb` on a child binary to localize.

### CAT-D — Other (builtin coverage, format/sort/findall, etc.)

Builtins like `findall/3`, `sort/2`, `format/2`, `atom_codes/2`, etc. live in `bb_exec.c`'s
`BB_BUILTIN` arm but the corresponding flat-emit `bb_builtin.cpp` template only covers
`write/1`, `nl/0`, `is/2`, and a handful of others. Per `BB_BUILTIN` invocation under flat-emit,
the unrecognized arms emit a stub-comment + jmp γ (which is why these programs succeed without
producing the expected output — same shape as the missing-conjunction-tail in CAT-A but a
different cause).

## Recommended next-session priority

1. **CAT-A fix first** (single-line `lower_pl.c:213` change + interp `BB_PL_ALT` re-verification).
   Biggest GATE-2 lift per LOC, and a cleaner foundation for CAT-B/C/D investigations.
2. **CAT-B compound-build runtime helper.** This is also a prerequisite for several rung-ladder
   builtins that need to construct return terms (rung28 catch/throw, rung25 term_to_atom output).
3. **V-6 audit** (RULES.md exception removal + `pl_bb_dcg` mode-2-only assertion) — straightforward
   after V-5 + CAT-A land.

## RULES.md update needed

The "Exception: Prolog `--run` via `sm_interp_run` until bb_pl_*.cpp templates land" sanction on
the "NO SM/BB WALKING AT RUNTIME IN MODES 3/4" rule should be deleted. V-5 closes the exception.
Deferred to next commit pending PR review — flagging here so it's not forgotten.

## Files touched (committed by this handoff)

- `src/driver/scrip.c` — V-5 helper + Prolog `--run` rewire.
- `src/emitter/emit_sm.c` — `BB_PL_STRUCT` in `pl_ir_kind_uses_sval` (S_ERR fix).
- `.github/PLAN.md` — V-5 marked done; next step bumped.
- `.github/GOAL-PROLOG-BB.md` — V-5 ✅; PL-DEBT-1 ledger seeded with CAT-A/B/C/D.
- `doc/HANDOFF-2026-05-27-OPUS-PROLOG-BB-V5.md` — this file.
