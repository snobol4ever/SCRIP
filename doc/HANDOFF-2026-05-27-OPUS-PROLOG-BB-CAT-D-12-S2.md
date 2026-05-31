# HANDOFF 2026-05-27 (Opus 4.7) — CAT-D-12-S2: functor/3 + arg/3 + =../2 mode-4 templates

## Summary

CAT-D-12-S2 LANDED. Mode-4 templates for `functor/3`, `arg/3`, `=../2` in `bb_builtin.cpp`,
plus eight effect helpers in `bb_exec.c`. **rung09_builtins_builtins mode-4 is byte-exact
matched to oracle** (`foo 2\nb\n[foo,a,b]\nyes\nyes\nno\nno`), closing the last gap that
made rung09 a three-mode disagreement under V-5's pipeline.

The work follows S1's restoration of three-mode agreement (GATE-2 132/132) by filling the
mode-4 emission slot for the three structural builtins that S1's BB_BUILTIN exec arms in
`bb_exec.c` (lines 2977 / 3012 / 3026) had been correctly implementing in modes 2/3 all along.

100% template emission preserved. Zero `seg_byte`/`SL_B`/`sl_emit_one`/`emit_standard_blob`
outside `*_templates/` and `emit_core.c`. Every emitted byte originates from a `s_2asm` /
`s_1asm` / `s_L2asm` call inside the templates; helpers do only effect work (trail mark,
term construction, unify, deref).

## Gates at handoff HEAD

| Gate | Before S2 | After S2 | Note |
|------|----------:|---------:|------|
| GATE-1 smoke | 5/5 | 5/5 | held |
| GATE-2 crosscheck (3-mode) | 132/132 | 132/132 | held |
| GATE-3 mode-2 (`--interp`) | 89/107 | 89/107 | held |
| GATE-3 mode-3 (`--run`) | 89/107 | 89/107 | held |
| GATE-4 mode-4 micro-rung | 4/4 | 4/4 | held |
| Full mode-4 corpus | 21/89 (PLAN.md baseline) | **22/89** | **+1: rung09_builtins_builtins** |
| Icon / Raku / Snocone / Rebus / SNOBOL4 smoke | 5 / 5 / 8 / 4 / 5 | 5 / 5 / 8 / 4 / 5 | all held |
| FACT RULE grep | 0 | 0 | held |
| `.S_ERR` in rung09 emit | 0 | 0 | held |
| `PL_BUILTIN: unknown 'functor'/'arg'/'=..'` stubs in r09 emit | 3 | **0** | retired |

SNOBOL4 smoke shows 5/6 (`beauty_omega --interp vs oracle` FAIL); confirmed pre-existing
Milestone-2 territory, not introduced by this change.

## Code changes

### `src/lower/bb_exec.c` (+144 lines)

Inserted a CAT-D-12-S2 block immediately after `rt_pl_term_cmp_terms` (between the CAT-D-9b
and `pl_format_float` blocks). Three static port-arm functions + eight extern entry points:

- `static int functor_common(Term *t0, Term *t1, Term *t2)` — direct port of the BB_BUILTIN
  exec arm at line 2977: decompose if `t0` bound (read functor/arity, unify into Name/Arity),
  construct otherwise (Name+Arity given, build fresh-var compound, unify into t0). Trail-marked,
  unwound on fail. Same float/atom/int Name handling as the original arm.
- `int rt_pl_functor(k0,i0,s0, k1,i1,s1, k2,i2,s2)` — 9-scalar entry for scalar arg paths.
- `int rt_pl_functor_term(t0, k1,i1,s1, k2,i2,s2)` — accepts prebuilt `Term*` for the term
  position (a0). Used when `bb_pl_builtin.cpp` detects `BB_PL_STRUCT` literal at a0.

- `static int arg_common(Term *tN, Term *tT, Term *tA)` — port of arm at line 3012. tN must
  be TERM_INT (1-based), tT must be TERM_COMPOUND, unify tA with `tT->compound.args[tN-1]`.
- `int rt_pl_arg(...)` — 9-scalar.
- `int rt_pl_arg_term(k0,i0,s0, t1, k2,i2,s2)` — a1 (Term position) is compound literal.

- `static int univ_common(Term *t0, Term *t1)` — port of arm at line 3026. Decompose if t0
  bound: build `[Functor|Args]` via cons-cells with `ATOM_DOT`. Construct otherwise: walk
  cons list (capped at 64 elements per the upstream arm), reconstitute compound, unify into t0.
- `int rt_pl_univ(...)` — 6-scalar.
- `int rt_pl_univ_term(t0, k1,i1,s1)` — a0 (Term) compound.
- `int rt_pl_univ_term_list(k0,i0,s0, t1)` — a1 (List literal) compound.
- `int rt_pl_univ_term_term(t0, t1)` — both compound.

### `src/emitter/BB_templates/bb_builtin.cpp` (+197 lines)

Inserted three dispatcher blocks immediately before the unknown-fn stub on what was line 534.
Each block detects whether the canonical term-position arg is `BB_PL_STRUCT` (i.e. a literal
compound at the call site) and routes accordingly:

- **functor/3** — a0 is the term position. If `BB_PL_STRUCT`, emit `sub rsp,16` + the
  `emit_build_compound_term(a0)` walker, move result to `rdi`, pack a1/a2 into the remaining
  6 SysV registers (esi=k1, rdx=i1, rcx=s1, r8d=k2, r9=i2, [rsp+0]=s2), call
  `rt_pl_functor_term@PLT`. Otherwise emit `sub rsp,32` + the full 9-scalar call:
  edi=k0, rsi=i0, rdx=s0, ecx=k1, r8=i1, r9=s1, [rsp+0]=k2, [rsp+8]=i2, [rsp+16]=s2 — `call
  rt_pl_functor@PLT`.

- **arg/3** — a1 is the term position. Two paths analogous to functor's; the `_term` variant
  carries the compound `Term*` in `rcx`.

- **=../2** — both a0 and a1 can be compound literals (a0 = term, a1 = list literal). Four
  paths: both-scalar (`sub rsp,8`, 6 regs, call `rt_pl_univ@PLT`); compound-a0 only (`sub
  rsp,8`, build a0 first, call `rt_pl_univ_term@PLT`); compound-a1 only (`sub rsp,8`, build
  a1 first, call `rt_pl_univ_term_list@PLT`); both-compound (`sub rsp,16` — stash t0 across
  the t1 build via `qword ptr [rsp+0]`, then `rt_pl_univ_term_term@PLT`).

Every path closes with the standard CAT-D-* triplet:
```
test eax, eax
je   <ω>
jmp  <γ>
<β:> jmp <ω>
```

## Verification of `rung09_builtins_builtins`

`/home/claude/corpus/programs/prolog/rung09_builtins_builtins.pl` exercises all three predicates
in their decompose form, plus the four CAT-D-10 type-test ITE branches that depend on
functor/3-style introspection succeeding.

| Mode | Before S2 | After S2 |
|------|-----------|----------|
| `--interp` (mode 2) | `foo 2\nb\n[foo,a,b]\nyes\nyes\nno\nno` | (unchanged) |
| `--run` (mode 3) | `foo 2\nb\n[foo,a,b]\nyes\nyes\nno\nno` | (unchanged) |
| `--compile --target=x86` (mode 4) | `_ _\n_\n_\nyes\nyes\nno\nno` | **`foo 2\nb\n[foo,a,b]\nyes\nyes\nno\nno`** |

The before-S2 mode-4 output reflected `PL_BUILTIN: unknown` stubs emitting `succ_back` for the
three builtins — control reached `write(Name)` with Name still unbound (printed as `_`), and
the chained `functor(foo(a,b), N, A)`, `arg(2, foo(a,b), Arg)`, `=..` calls all no-op'd.

## What S2 exposed (not a regression — pre-existing PL-DEBT)

While writing the scalar-path test `T = foo(x,y,z), arg(1, T, A)`, I discovered that the
`BB_PL_VAR` scalar path of every term-position arg is unreachable in the current corpus, for
a reason upstream of CAT-D-12:

**Root cause:** `rt_pl_node_to_term` in `src/runtime/rt/rt.c:965-980` switches on `kind` and
has no case for `BB_PL_STRUCT`. The compound kind falls into `default: return
term_new_int(ival)`, which means **every `T = foo(...)` unify in mode-4 binds the env slot to
`term_new_int(arity)` instead of a real compound**. By the time `arg/3` reads the slot back
through `rt_pl_node_to_term(BB_PL_VAR, slot)`, it derefs to a TERM_INT (tag=3), not
TERM_COMPOUND (tag=2).

This is **CAT-B from V-5 handoff** ("Compound-term unify binds nothing"). The fix is in
`bb_unify.cpp`: when the RHS of a `BB_UNIFY` is `BB_PL_STRUCT`, the emitter must route through
`emit_build_compound_term` (CAT-D-9b's post-order walker) to construct a real Term tree before
the unify call — not through `rt_pl_node_to_term`. Alternatively, `rt_pl_node_to_term` itself
could add a `BB_PL_STRUCT` case that reads child args, but the child args aren't passed
through the flat (k,i,s) triple — they live on the BB graph. The emit-side fix in `bb_unify.cpp`
is the natural place.

**Once CAT-B lands, my three scalar helpers (`rt_pl_functor` / `rt_pl_arg` / `rt_pl_univ`)
light up automatically.** No further template change needed — they already accept the canonical
(k,i,s) triple, and the `BB_PL_VAR` slot will then deref to a proper TERM_COMPOUND.

For the current corpus, every passing program reaches the new functor/arg/=.. templates
through the compound-literal path (`_term` variants), so the scalar dead-code does not affect
gates. Documented in the GOAL file under the S2 section.

## RULES.md / convention compliance

- **NO SM/BB WALKING AT RUNTIME IN MODES 3/4:** Held. The new helpers do trail/unify/term
  construction only; they never walk a `BB_t*` or `SM_Program*`. Compound construction at emit
  time uses `emit_build_compound_term` (CAT-D-9b walker that emits asm; not a runtime walker).
- **TEMPLATE-ONLY EMISSION (FACT RULE):** Held. `grep -rnE 'seg_byte\(SEG_CODE|SL_B\(|sl_emit_one|emit_standard_blob' src/`
  excluding `*_templates/` and `emit_core.c` returns 0.
- **NO C BYRD-BOX FUNCTIONS:** Held. Zero `DESCR_t foo(void*, int entry)` introduced.
- **Four ports = Greek letters only:** Held. `_.lbl_α / _.lbl_β / _.lbl_γ / _.lbl_ω` are the only
  port symbols in the new template emissions.

## Files touched (committed by this handoff)

**SCRIP repo:**
- `src/lower/bb_exec.c` — +144 lines (CAT-D-12-S2 block).
- `src/emitter/BB_templates/bb_builtin.cpp` — +197 lines (three dispatcher blocks).

**.github repo:**
- `PLAN.md` — Prolog BB row bumped (CAT-D-12-S2 ✅, gate deltas, next step = CAT-B).
- `GOAL-PROLOG-BB.md` — full CAT-D-12-S2 section inserted above CAT-D-12-S1.
- `doc/HANDOFF-2026-05-27-OPUS-PROLOG-BB-CAT-D-12-S2.md` — this file (or sibling location in SCRIP/doc/).

## Recommended next session priority

1. **CAT-B** — compound-term unify fix in `bb_unify.cpp`. This is the gate that unlocks
   roughly half the currently-FAILing 67 mode-4 rungs (everything that does `T = compound, …`
   downstream — rung02_facts, rung03_unify, rung05_backtrack, rung06_lists at minimum).
   Likely a 20-50 line surface in `bb_unify.cpp` to detect `BB_PL_STRUCT` RHS and route
   through `emit_build_compound_term`. Mode-2/3 unchanged (those paths use `pl_node_to_term`
   in `bb_exec.c`, which DOES handle BB_PL_STRUCT correctly — only mode-4 is broken).
2. **CAT-D-11** — sort/2 + msort/2. Stand-alone RT helper job, straightforward.
3. **CAT-A-3** — BB_PL_CALL + BB_CHOICE β-resume. Still blocked on Lon directive on design.
