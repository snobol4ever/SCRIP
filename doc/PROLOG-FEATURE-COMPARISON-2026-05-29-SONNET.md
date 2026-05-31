# PROLOG-FEATURE-COMPARISON-2026-05-29-SONNET.md

Feature-by-feature comparison of SCRIP's Prolog engine against the two reference
implementations shipped in this session: **GNU Prolog (gprolog-master)** and
**SWI-Prolog (swipl-devel-master)**. Every claim below cites the exact reference
source line read. The purpose is to find where the reference engines do something
better or differently, and to convert each real divergence into a verifiable rung.

This doc grounds the new rung families **PL-TRAIL-COND**, **PL-INDEX-L2**, and
**PL-CP-FRAME** added to GOAL-PROLOG-BB.md.

---

## Method

Read in full: `gprolog-master/src/EnginePl/wam_inst.h` (WAM choice-point frame,
trail tags, conditional-trail macros), `gprolog-master/src/Pl2Wam/indexing.pl`
(two-level first-arg indexing), `swipl-devel-master/src/pl-incl.h` (Trail / GTrail
macros, Mark/Undo), `swipl-devel-master/src/pl-index.c` (hash + deep indexing).
Read on our side: `src/frontend/prolog/prolog_unify.c` (trail + bind + unify),
`src/frontend/prolog/term.h` + `prolog_atom.c` (term model), `src/runtime/interp/
pl_runtime.{c,h}` (choice-point frame), and the WAM-CP-8 first-arg-indexing handoff.

---

## Feature 1 — Trailing discipline (the headline finding)

### What the references do

Both reference engines trail a binding **conditionally**, not unconditionally.

GNU Prolog, `wam_inst.h:472`:
```
#define Word_Needs_Trailing(adr)  ((adr) < HB1 || (Is_A_Local_Adr(adr) && (adr) < B))
```
`Bind_UV/Bind_OV/Bind_MV` (`wam_inst.h:477-510`) each guard `Trail_*` with
`if (Word_Needs_Trailing(adr))`. The rule: a variable only needs trailing if it
is **older than the current choice point** (heap address below `HB1`, the youngest
CP's heap boundary, or a local cell below `B`). A variable created *after* the
current CP will be discarded wholesale on backtrack (the heap segment above HB is
reclaimed), so recording it on the trail is pure waste.

SWI-Prolog, `pl-incl.h:2194`:
```
#define GTrail(p)  do { if ( p < LD->mark_bar ) (tTop++)->address = p; } while(0)
```
Identical idea: global-stack bindings are trailed only when the cell is below
`LD->mark_bar` (the current mark boundary). `LTrail` (local stack) is
unconditional because local frames have different reclamation.

### What we do

`prolog_unify.c` `bind()`:
```
static void bind(Term *var, Term *val, Trail *trail) {
    if (var->var_slot != -1)
        trail_push(trail, var);     /* UNCONDITIONAL */
    var->ref = val;
    var->tag = TERM_REF;
}
```
Every binding to a slotted variable is pushed on the trail, regardless of whether
the variable is younger or older than the current choice point. Our terms
(`term.h`) carry no creation age, so the conditional cannot even be expressed today.

### Why it matters

In any backtracking-heavy or deterministic-but-CP-bearing predicate, the trail
grows with **every** binding rather than only the bindings that outlive a CP. The
trail is GC-managed (`GC_realloc` doubling in `trail_push`), so unconditional
trailing inflates trail memory and the `trail_unwind` loop walks more entries on
every backtrack. For deep recursion under a live CP this is O(total bindings)
instead of O(bindings older than the CP).

### Feasibility in our model

We already have `g_pl_cp_stamp` (a monotonic CP counter, `pl_runtime.c:33`) and the
`pl_choice` frame carries `stamp`. Adding a creation-stamp field to `Term` (set in
`term_new_var`) lets `bind()` compare `var->birth_stamp` against the youngest live
CP's `stamp`: if the var was born *after* the youngest CP, skip the trail push.
This is the boxed-model analogue of `adr < HB1`. Bounded, additive, mode-2 first.

→ **Rung family PL-TRAIL-COND.**

### ⛔ ATTEMPTED 2026-05-29 (Sonnet 4.6) — VERIFIED UNSOUND IN OUR MODEL, REVERTED

PL-TRAIL-COND-1 was implemented exactly as designed (Term `birth_stamp`, monotonic
`g_pl_var_stamp`, HB register `g_pl_hb_stamp` snapshotted into `pl_choice` on push and
restored on pop/truncate, `bind()` skipping the trail push when
`var->birth_stamp > g_pl_hb_stamp`). It **broke backtracking**: GATE-3 dropped 104→102,
with `rung05_backtrack_backtrack` (recursive `member/2` over `[a,b,c]`) yielding only `a`
and `rung11_findall_findall_arith` collecting only `[1]` instead of `[1,4,9]`. Reverted in
full; back to 104/107.

**Root cause (the architectural finding):** the gprolog/SWI conditional-trail optimization
presupposes a SECOND binding-undo mechanism — heap-segment reclamation. In a real WAM, on
backtrack the heap pointer H is reset to the CP's HB, which *physically discards* every cell
(and thus every binding) allocated after the CP, so those bindings need not be on the trail.
SCRIP's boxed GC model has **no such second mechanism**: `trail_unwind(mark)` is the SOLE way a
binding is undone, and vars are never reclaimed on backtrack (GC owns lifetime). Therefore
EVERY binding to a mutable var must be trailed — skipping any one leaves it bound across the
backtrack, which is exactly the bindings inside a recursive clause body that must reset for the
next alternative. The `birth_stamp > HB` test correctly identifies "young" vars, but in our
model young ≠ reclaimed-on-backtrack, so the skip is unsound.

**Precondition for ever revisiting:** PL-TRAIL-COND is only viable AFTER a heap-reclamation
track exists (a per-CP allocation region that is bulk-discarded on backtrack, replacing GC for
term cells under a CP). That is a large substrate change with its own risks and is NOT currently
motivated. **PL-TRAIL-COND is therefore CLOSED as won't-fix-as-designed.** The win it targets
(smaller trail) is real but unreachable without abandoning the GC boxed model.

This is a clean negative result: the optimization was tried, measured, proven unsound, and the
exact missing precondition identified. Recorded so no future session re-attempts it blind.

---


## Feature 2 — First-argument indexing depth

### What the references do

GNU Prolog, `indexing.pl` (Level 1 + Level 2): clauses are split into groups, then
each non-var group emits `switch_on_term(LabVar,LabAtm,LabInt,LabLst,LabStc)` to
dispatch by principal type, and within a type, `switch_on_atom(N, [(atm,Lab)...])`
/ `switch_on_integer(...)` which are **hash tables** from key to clause-group label
(`indexing.pl:60-78`). Selecting among N atom-keyed clauses is O(1), not O(N).

SWI-Prolog, `pl-index.c`: Fibonacci-hash buckets (`hashIndex`, line 177),
**multi-argument** indexing (`find_multi_argument_hash`, line 115) and **deep /
JIT** indexing (`MSG_INDEX_DEEP`, line 381) — it will build an index on whichever
argument(s) best discriminate, and index into sub-terms, all created lazily on
first call.

### What we do

WAM-CP-8 (handoff `HANDOFF-2026-05-29-OPUS-PROLOG-BB-WAM-CP-8-FIRST-ARG-INDEXING.md`):
a single first-argument **class-tagged key** (`PL_IDX_CLS_ATOM/INT/FLT/CMP`), and a
**linear filter scan** over `bodies[]` counting clauses whose `idx_key` matches the
caller's key or is `PL_IDX_VAR`. When exactly one survives and it is single-solution,
the CP is elided (the LCO enabler). This is Level-1 only and the survivor count is
O(N) in the number of clauses.

### Gap

We have the correctness win (CP elision) but not the asymptotic win. For a predicate
with hundreds of facts sharing a class (the classic Prolog fact-database), our linear
filter is O(N) per call where gprolog/SWI are O(1) via a hash bucket. We also index
only the first argument; SWI picks the most discriminating argument.

→ **Rung family PL-INDEX-L2** (hash-bucket Level-2 first; multi-arg deferred).

---

## Feature 3 — Choice-point frame completeness

### What the references do

GNU Prolog choice-point frame is 8 saved words (`wam_inst.h:96-104`): `ALTB`
(alternative code ptr), `CPB` (continuation), `BCIB` (cut info), `EB` (env top),
`BB` (previous B), `HB` (heap top — drives `Word_Needs_Trailing` and on-backtrack
heap reclamation), `TRB` (trail top), `CSB` (constraint stack), plus saved args `AB`.

### What we do

`pl_runtime.h:57` `pl_choice`: `type, parent(=BB), trail_mark(=TRB), env(=EB),
resume(=ALTB), saved_args(=AB), cursor, stamp, saved_cut_barrier`. We deliberately
deferred **HB** (heap mark), **CPB**, **BCIB**, **CSB** — recorded as "deferred to
the later tagged-word track" in the header comment (`pl_runtime.h:48-49`).

### Gap

The missing field that actually blocks an optimization is **HB** (the heap/birth
boundary). Feature 1's conditional trailing needs exactly this: the youngest CP's
notion of "what counts as old". In our boxed model HB becomes "the CP's `stamp`",
so PL-TRAIL-COND effectively *is* the minimal HB port. CPB/BCIB/CSB are not on any
current critical path (no constraint solver; cut already works via
`saved_cut_barrier`). So this feature folds into PL-TRAIL-COND rather than needing
its own large rung; noted here for completeness and flagged as PL-CP-FRAME-0
(documentation/struct-field reservation) so a future reader knows HB is the only
deferred field with a known consumer.

---

## Features verified ALIGNED (no action)

- **Term/deref model.** Our boxed `Term*` + `TERM_REF` deref chain (`term.h:30`) is
  the SWI-style cell-with-reference model, not gprolog's raw WamWord stack. Both are
  valid; ours integrates with the shared AST + GC. `term_deref` loop matches SWI
  `deRef`. No change warranted — this is a deliberate substrate choice.
- **Cut via barrier.** Our `g_pl_cut_barrier` + `pl_cp_truncate` (`pl_runtime.c:60`)
  mirrors gprolog `Assign_B(BB(B))` / cut-to-barrier. Aligned.
- **catch/throw.** Scratch-trail trial unification of the catcher
  (`pl_runtime.c:298-301`) before committing is the correct ISO discipline (don't
  bind on a non-matching catcher). Aligned with both references' ball/catcher model.

---

## Recommended landing order

1. **PL-TRAIL-COND-1** (smallest real win, mode-2, corroborated by BOTH references) —
   add var birth-stamp + conditional trail. Verify trail-size shrinks on a
   backtracking probe with output byte-identical to baseline.
2. **PL-INDEX-L2-1** — replace the linear first-arg filter with a hash bucket built
   at lower time; same dispatch semantics, O(1) selection.
3. **PL-CP-FRAME-0** — reserve/document the HB field (folded; bookkeeping only).

All three are mode-2 interpreter logic (zero emitted bytes, FACT unchanged) and are
verified against mode-2 as the correctness reference per the goal's discipline.
