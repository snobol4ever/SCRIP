# HANDOFF 2026-05-29 Opus 4.8 — PROLOG-BB PLR-K-3/4/5/6

**Commits:** SCRIP `f6223d74` · corpus `4a7d2dd` · .github (watermark + this handoff)
**Parent:** SCRIP `28a720f2`, corpus `0f692c3`

## Summary

Four mode-3-native MEDIUM_BINARY rungs continuing the PLR-K ladder.
**GATE-2 crosscheck 47 → 61 PASS (+14).** GATE-3 mode-2 104/107 → 108/111 (rung40 enrolled).
All other gates byte-identical. FACT arm1=0 / arm2=12.

| Gate | Result |
|---|---|
| GATE-1 smoke | 5/5 |
| GATE-2 crosscheck | 61 PASS / 75 FAIL / 1 ORACLE_MISS |
| GATE-3 mode-2 | 108/111 (3 pre-existing fails: rung15 then_reassert, rung27 succ_or_zero, rung30 dcg_pushback) |
| GATE-4 mode-4 minimal | 4/4 |
| GATE-SWI | 57/57 (100%) |
| siblings | icon 5/5, raku 5/5, snobol4 13/13 |
| FACT | arm1=0, arm2=12 |

## PLR-K-3 — numbervars/3 + write/1 compound BINARY arm (+5, rung20)

- New `rt_pl_numbervars_term(void *t0, long start, int k2, long i2, const char *s2)` in `bb_exec.c`
  (after `rt_pl_char_type`), declared in `bb_exec.h`. Transliterates the mode-2 numbervars oracle:
  walk the built term depth-first, bind each unbound TERM_VAR to a fresh `$VAR(N)` compound, then
  unify End with the final counter under a trail mark. Returns 1/0.
- BINARY arm in `bb_builtin.cpp`: `numbervars/3` is `PL_BI_CHAIN` (arg0=α term, arg1=α->γ start int,
  arg2=α->γ->γ End var). Build arg0 via `emit_build_compound_term_bin` (TERM_VARs alias the live env
  slots → binding shows in a later `write`), keep in rdi, load start/End-triplet, call the helper.
- **Discovered + fixed blocker:** the `write/1` BINARY arm handled only `BB_ATOM`/`BB_PL_VAR`. Every
  compound/int/float arg fell through to a no-op → `write(f(a,b))` printed EMPTY in mode-3 even for a
  ground literal. This masked numbervars (End was right; only the compound `write` was empty). Added
  `rt_pl_write_term_ptr(void *t)` in `rt.c` (renders any built `Term*` via the shared `pl_write`) +
  an `else` branch in the write arm that builds the term and renders it. rung20 ×5 now 3-mode AGREE.

## PLR-K-4 — writeq/1 + write_canonical/1 + print/1 BINARY arm (+5, rung22)

- New `rt_pl_writeq_term_ptr` / `rt_pl_write_canonical_term_ptr` in `rt.c` (route a built `Term*`
  through `pl_writeq` / `pl_write_canonical`, the mode-2 oracle's own writers), declared in `rt.h`.
- New writeq/write_canonical BINARY arm: build the arg uniformly via `emit_build_compound_term_bin`,
  call the matching quoting writer, always-succeed γ tail. `print` added to the write arm condition
  (it was bundled with write/writeln in MEDIUM_TEXT but the BINARY arm only matched write/writeln;
  nl-suffix stays gated on `writeln` so print gets no spurious newline).
- **General builder strengthening:** added a `BB_ARITH` branch to `emit_build_compound_term_bin`. An
  arith functor used in TERM position is a compound (e.g. `write_canonical(1+2)` → `+(1,2)`); operands
  are on α/β (NOT γ-chained like BB_PL_STRUCT). Mirrors `pl_node_to_term`'s BB_ARITH case. Benefits
  any BINARY path materializing operator terms. rung22 ×5 now 3-mode AGREE.

## PLR-K-5 — compound-arg type-test BINARY arm (+4, NEW rung40)

- Replaced the J-1/J-3 honest-abort (`if (a0->t == BB_PL_STRUCT) return double-jump-stub`) in the
  type-test BINARY arm with the compound path: build via `emit_build_compound_term_bin` (now covers
  BB_PL_STRUCT + BB_ARITH from PLR-K-4), pass as rsi, fn ptr in rdi, call the existing
  `rt_pl_type_test_term` helper the MEDIUM_TEXT arm already used.
- `is_list([1,2,3])` / `compound(f(a))` / `ground(g(a,X))` / `callable(f(x))` now 3-mode AGREE
  (mode-4 TEXT arm already existed → mode-3 now at parity).
- **No prior corpus coverage existed for compound type-tests.** Added corpus
  `rung40_typetest_compound_{is_list,compound,ground,callable}` (.expected from the mode-2 reference;
  all 3 modes agree). Extended the GATE-3 rung-suite glob in `scripts/test_prolog_rung_suite.sh` to
  `rung4[0-9]` so the new rung enrolls in mode-2 (108/111).

## PLR-K-6 — retract/1 + retractall/1: ARCHITECTURAL BOUNDARY, honest-abort (no +N)

**Finding:** clause REMOVAL cannot be delivered in mode-3 native against the current
statically-compiled clause model. The `BB_CHOICE` dispatcher (`bb_pl_choice.cpp`) bakes the clause
count as a compile-time constant (`cmp edi, n`, `n = _.pl_choice_n`) and emits each clause as a fixed
flat code block. A runtime `zc->nbodies--` is INVISIBLE to the emitted enumerator — a later
`color(X)` still tries all originally-emitted clauses. This is the same boundary `PL-RT-ASSERTZ`
faces; it needs a runtime-mutable clause store the native dispatcher consults (multi-session
substrate, NOT a template arm).

**Action:** per the NO-MODE-FALLBACK rule, the retract/retractall BINARY arm sets
`g_sm_native_unsupported` → honest NATIVE-ABORT (rc 134, named diagnostic). This replaces the prior
double-jump stub, which silently no-op'd the removal and reported the clause as gone (and segfaulted
the recursive `retract_loop` case). The 5 rung14 tests now correctly count as NATIVE-ABORT, not
silent wrong-answers; GATE-2 stays 61 (no false progress). **Mode-2 oracle untouched** (rung14 m2 5/5).

**Findings recorded for the future mutable-store work:**
1. A single-clause predicate lowers to a BARE `BB_PL_SEQ` body (no `BB_CHOICE`, no `zc->bodies[]`).
   `lower_pl_predicate`: `choice->n == 1` → `lower_pl_clause_body` directly.
2. Within a multi-clause `BB_CHOICE`, each clause body is itself `BB_PL_SEQ`-wrapped; the head-unify
   nodes (`BB_UNIFY`, α=slot var, β=head literal) live in `bb_pl_seq_state_t->goals[0..arity-1]`,
   NOT at `body->entry`. A correct structural head-matcher (descend SEQ → goals[] → unify β against
   the pattern args, no `bb_exec_once`) was built and proved correct (`retract(age(bob,X))`→X=25,
   `retract(ghost(x))`→fail) but removed as dead code once the arm became an abort, since it can
   only mutate the runtime `zc` the native enumerator ignores.
3. `snprintf` FAULTED in deep mode-3 native context (SSE varargs alignment on a frame this call site
   doesn't 16-align for that libc variant); `fprintf` in the same function was fine. A manual
   digit-build sidesteps it. Note for future native helpers that format strings.

## Files touched

**SCRIP** (`f6223d74`):
- `src/lower/bb_exec.c` — `rt_pl_numbervars_term` effect helper (+30).
- `src/lower/bb_exec.h` — numbervars decl (+1).
- `src/runtime/rt/rt.c` — `rt_pl_write_term_ptr` / `rt_pl_writeq_term_ptr` /
  `rt_pl_write_canonical_term_ptr` (+32).
- `src/runtime/rt/rt.h` — three writer decls (+3).
- `src/emitter/BB_templates/bb_builtin.cpp` — numbervars/writeq/write_canonical BINARY arms; print
  in write arm; compound type-test path; `BB_ARITH` in `emit_build_compound_term_bin`; retract
  honest-abort (+156/-6).
- `scripts/test_prolog_rung_suite.sh` — glob extended to `rung4[0-9]`.

**corpus** (`4a7d2dd`):
- `programs/prolog/rung40_typetest_compound_*` — 4 new programs (.pl/.expected/.s).
- `programs/prolog/rung22_write_canonical_*.s` — regenerated mode-4 assembly.

**.github:** GOAL-PROLOG-BB.md watermark + this handoff reference.

## NEXT

- **string_io (rung24)**, **format/2 compound (rung19 remainder)**, term_to_atom/term_string mode-4
  emit, atom_number mode-4 — tractable BINARY/TEXT arms.
- **findall/3** last (own protocol — `nd->ival` is `bb_pl_findall_state_t*`, not arity).
- **PL-RT-ASSERTZ mutable clause store** (multi-session) — the real home for both assertz and retract
  in mode-3/4. The native `BB_CHOICE` dispatcher must read clause count/bodies from a runtime
  structure instead of the compile-time `_.pl_choice_n`. The structural head-matcher logic and the
  two clause-shape findings above are the starting point.
- writeq/write_canonical **mode-4 (MEDIUM_TEXT) arm** still absent (they produce empty under
  `--compile --target=x86`); mechanical follow-on: add the `BB_ARITH` branch to the TEXT builder
  `emit_build_compound_term` + two TEXT arms calling the quoting writers @PLT.
