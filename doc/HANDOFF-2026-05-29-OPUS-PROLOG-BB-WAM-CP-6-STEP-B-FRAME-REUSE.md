# HANDOFF — WAM-CP-6 Step B: LCO Frame-Reuse (Phase B1)

**Date:** 2026-05-29
**Author:** Opus 4.8
**Goal:** GOAL-PROLOG-BB.md
**Predecessor:** Step A LCO-DETECT (`860d1163`) + Step B design doc (`ce99d578`)
**Files touched:** `src/lower/bb_exec.c` ONLY (+94 lines). No enum, emitter, lowering, or FACT change.

---

## What landed

WAM-CP-6 Step B Phase B1 — actual **frame-reuse** for tail-position deterministic
singleton-callee `BB_PL_CALL`. Where Step A only *detected* the SWIPL `I_DEPART`
eligibility (audit, gated `SCRIP_LCO_TRACE=1`), Step B *acts*: the eligible tail
call no longer recurses into `bb_exec_once(_bcfg)` (one C frame per Prolog call →
the historical SEGFAULT-CLUSTER). Instead it binds args into a fresh callee env,
trips a redirect sentinel, and returns NULL; the `bb_exec_once` / `bb_exec_resume`
driver loop reuses its own C frame and redirects `cur` to the callee graph entry.
C stack stays flat across arbitrarily deep singleton tail recursion.

This is exactly the mechanism the design doc settled: a **driver-recognized
redirect sentinel**, NOT a new `BB_PL_TAIL_CALL` op (zero enum/emitter/FACT churn).

## Mechanism (as built)

Two file-scope globals in `bb_exec.c` (after `g_current_cfg`):
```c
static BB_graph_t * g_pl_tail_redirect_cfg   = NULL;
static BB_t       * g_pl_tail_redirect_entry = NULL;
```

**BB_PL_CALL fresh path:** when the Phase-B1 gate holds, bind args into a fresh
`callee_env` (same `term_new_var` + `unify` as the normal path — bindings record
on the global trail BEFORE the redirect), set `g_pl_env = callee_env`,
`bb_reset(_bcfg)`, set the two sentinel globals, `bb->state = 0`, `return NULL`.
No `PlCallSt`, no `bb->state = 1`: a deterministic tail call has no resume.

**Driver loops (`bb_exec_once` AND `bb_exec_resume`):** at the top of each
iteration, immediately after `next = bb_exec_node(cur)` and BEFORE the
`if (!next)` terminal branch, check the sentinel: if set, clear it, point
`g_current_cfg = bbg = tgt`, recompute `safety`, `cur = entry`, `continue` —
reusing this C frame.

## Phase-B1 eligibility gate (all four must hold, all statically decidable at fresh-call entry)

1. **Tail position:** `bb->γ == NULL` (already encoded by AG lowering at
   `lower_pl_clause_body:596` for the rightmost statement).
2. **No live CP on the spine:** `g_pl_bfr == NULL`. THIS WAS THE KEY FIX during
   implementation (see below). A non-NULL `g_pl_bfr` means some enclosing
   construct (a multi-clause `BB_CHOICE` we are inside, or a disjunction) has a
   pending alternative backtracking may re-enter.
3. **Singleton callee:** `_bcfg->entry->t != BB_CHOICE`. A single-clause
   predicate's graph entry is the `BB_PL_SEQ` body wrapper; a multi-clause
   predicate's is `BB_CHOICE`. Clause selection on a singleton pushes no CP.
4. **Statically CP-free body (except tail call):** new helper
   `bb_body_cp_free_except_tail(_bcfg)` returns 1 iff the body contains no
   `BB_CHOICE`/`BB_PL_ALT` and no NON-tail `BB_PL_CALL` (γ != NULL). A tail
   `BB_PL_CALL` is itself the next LCO candidate and does not disqualify.

## Implementation journey (two regressions found and fixed — recorded for archaeology)

The first cut used only gates (1) + (3). It regressed **GATE-3 104→103** and
**GATE-SWI 57→56**:

- **rung11_findall_findall_filter:** `[2,4]` → `[2]`. A singleton callee whose
  BODY contained a choice point (multi-solution generator) was flattened, but
  `findall` needs to backtrack into it for more solutions. **Fix:** added gate (4)
  — require the body to be statically CP-free except for a tail call.

- **test_list / memberchk (SWI):** `memberchk(f(X,a),[f(x,b),f(y,a)])` must
  backtrack from the failed `f(x,b)` unify into `f(y,a)` to bind `X=y`. The tail
  recursive `member(X,T)` in clause 2 of `member/2` is a singleton-body tail
  call, but it runs WHILE `member`'s own multi-clause `BB_CHOICE` has a live CP.
  Flattening stranded that CP. **Fix:** added gate (2) — refuse the redirect
  whenever `g_pl_bfr != NULL`.

After both fixes: ALL gates byte-identical to the `860d1163` baseline.

## Gates (all byte-identical to baseline, ZERO regressions)

| Gate | Result |
|---|---|
| GATE-1 smoke | 5/5 |
| GATE-2 crosscheck | 132/0 (5 ORACLE_MISS) |
| GATE-3 mode-2 | 104/107 |
| GATE-4 mode-4 minimal | 4/4 |
| GATE-SWI plunit | 57/57 (100%) |
| FACT grep | 0 |
| smoke icon / raku / snobol4 | 5/5 · 5/5 · 13/13 |

## Mechanism proof

`SCRIP_LCO_TRACE=2` adds an `[LCO] ACTED <name>/<arity> frame-reuse redirect`
line each time the redirect fires (default OFF). On the canonical singleton chain
`greet :- hello.  hello :- world.  world :- write(ok),nl.`:
```
[LCO] ACTED hello/0 frame-reuse redirect
[LCO] ACTED world/0 frame-reuse redirect
ok
```
Both tail calls reused the driver C frame; `ok` prints correctly.

## Safety (Design Question #2 — no copy needed)

The tail path NEVER calls `free()`. The abandoned caller env is a GC-allocated
`Term*` vector reclaimed by Boehm GC when unreachable; arg bindings are recorded
on the global trail BEFORE the redirect, and the callee env holds its own fresh
`term_new_var` cells (GC-reachable through `g_pl_env`). Unlike SWIPL's
stack-allocated frames, no `copyFrameArguments` is required. (Valgrind spot-check
deferred — valgrind not installed in this container; the no-free invariant is the
analytic guarantee.)

## NEXT

**Phase B2 — pairs with WAM-CP-8 (first-arg indexing).** The benchmark target
`count(0). count(N):-N>0,N1 is N-1,count(N1).` to 1e6 is STILL not eligible: its
recursive call is multi-clause (`_bcfg->entry->t == BB_CHOICE`) AND runs with a
live clause-selection CP (`g_pl_bfr != NULL`), so it fails gates (2) and (3). This
is the predicted dependency: WAM-CP-8 must elide the clause-selection CP (so
`g_pl_bfr` stays NULL) and dispatch directly to one clause (so no `BB_CHOICE`
wrapper is entered) before B2 can flatten `count/1`. The B1 mechanism proven here
is exactly what B2 reuses — extend the gate, not the mechanism.

Concretely for B2: once WAM-CP-8 indexing lands, the gate's `g_pl_bfr == NULL` and
`entry->t != BB_CHOICE` conditions should be replaced by "indexing selected
exactly one clause and elided the CP" — at which point `count(1e6)` runs in O(1) C
stack.

**Alternative independent tracks:** WAM-CP-7 (unify specialization), WAM-CP-13
(mode-4 corpus), PL-RT-ASSERTZ.
