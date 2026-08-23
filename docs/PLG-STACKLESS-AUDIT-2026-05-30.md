# PLG-STACKLESS-AUDIT-2026-05-30.md — PLG-0 value-stack / snapshot inventory

**Rung:** PLG-0 (GOAL-PROLOG-BB.md → PLG ladder). Doc-only, no code touched.
**Author:** Claude Opus 4.8, 2026-05-31 (dated -05-30 per the rung's filename contract).
**SCRIP HEAD at audit:** `cf6b7f6` (post Ground-Zero `lower.c` regrow + SMX excision).

---

## 0. The audit is being written AFTER a Ground-Zero rebuild the rung predates

The PLG rungs were authored 2026-05-30 against the THEN-live engine (`sm_interp_run` →
`SM_BB_SWITCH` → `pl_bb_dcg` → `bb_exec_once`, watermark `1882bc6b`). Between that and this
audit the trunk was rebuilt:

- **The Stack Machine was excised.** `src/driver/scrip.c` now aborts for any non-Icon, non-SNOBOL4
  language in `--run` and `--run`: `[SMX] FATAL: Stack Machine excised … This language has not
  yet crossed onto Byrd Boxes.` Verified live: `./scrip --run corpus/programs/prolog/hello.pl`
  → rc 134 (SIGABRT) at `scrip.c:393`.
- **The 3183-line `lower.c` tangle was deleted** (blob `d2d8c8e1`) and replaced by the unified
  four-port `lower.c` (the former `lower2.c`). Prolog lowering (`lower_pl.c`, the file the PLG
  rungs name in nearly every line) NO LONGER EXISTS. Its successor is the `ROLE_GOAL` arm of the
  one shared lowerer: `lower_goal()` in `src/lower/lower.c`.
- **The mode-3/4 native + x86 emit paths were excised too** (`--compile --target=x86 removed`,
  `--run` native SM gone).

**Consequence for PLG-0's verdict columns:** every mechanism the rung asks me to classify as
(a) remove / (b) keep-trail / (c) keep-cursor / (d) keep-ARBNO-array STILL EXISTS IN THE TREE —
but as **compiled-but-unreachable** code. It is linked into `libscrip_rt.so` (the Makefile still
lists `bb_exec.c` and every `bb_*.cpp` template) yet NOTHING reaches it at runtime, because the
SMX gate aborts before any Prolog BB is built or executed. So the honest verdict for the whole
Prolog value-stack apparatus is not "remove it surgically" (PLG-7's framing) but **"it is already
dead by excision; the live question is what to re-grow, and in what shape, as Prolog crosses onto
the new four-port `lower.c` (PLG-1 onward)."** This audit therefore does double duty: it inventories
the dead mechanisms AND records, for each, whether the re-grown engine should resurrect it.

---

## 1. Mechanism inventory (with file:line, reachability, and verdict)

Reachability key: **DEAD-X** = compiled, linked, but unreachable behind the SMX gate (no live
caller). **LIVE-ICN** = on the Icon/SNOBOL4 path that still runs. **GONE** = not in the tree.

### M1 — `bb_node_state_t` snapshot/restore (THE value stack the rung targets)

- **Struct:** `src/include/IR.h:200-213`. Fields: `value` (DESCR_t), `counter` (int64), `state`
  (int), `resolve_cs`, `ch_cur`, `ch_mark`, `ch_saved_env`, `ch_last_body`, `ch_last_act`,
  `ch_cp`, `ch_cut_barrier`, `ch_body_snaps`, `ch_nbodies`. The `ch_*` fields are explicitly the
  BB_CHOICE clause-dispatch state (cursor, trail mark, saved env, cp spine, cut barrier) — i.e.
  this struct copies a whole graph's per-node mutable state in and out.
- **Impl:** `bb_snapshot_state` / `bb_restore_state` at `src/lower/scrip_ir.c:207` / `:241`. The
  snapshot recurses into `zc->bodies[b]` (`:234`, `:260`) — it deep-copies every clause body's
  node state too. This recursion-over-shared-graph IS the copy-in/copy-out the rung calls "the
  value stack."
- **Call sites (`src/lower/bb_exec.c`):**
  | line | enclosing fn / case | path | verdict |
  |------|---------------------|------|---------|
  | 918, 937 | `rt_pl_aggregate_all_term` | Prolog (`aggregate_all/3` helper) | **DEAD-X → REMOVE** in re-grow |
  | 1589 | `bb_exec_node` `case IR_CALL` | **Icon proc call** | **LIVE-ICN → KEEP / SEPARATE** |
  | 3381, 3392, 3414, 3429 | `bb_exec_node` `case IR_GOAL` | Prolog user-pred call | **DEAD-X → REMOVE** in re-grow |
- **Verdict:** (a) REMOVE for Prolog. The four-port reference (`bench/test_icon.c`,
  `bench/test_sno_1.c`, `archive/.../prolog_emit.c`) gives each activation its OWN flat frame, so
  there is nothing shared to snapshot. **CRITICAL CAVEAT for PLG-7:** the `IR_CALL` site (1589) is
  the LIVE ICON path and is OUT OF SCOPE for the Prolog deletion — the rung's own PLG-7 note
  ("Audit Icon/SNOBOL4 sites first — those are SEPARATE") is correct and now has line numbers.
  Deleting the struct outright would break Icon; the Prolog re-grow must not reuse it, and PLG-7's
  "delete the fields" can only happen after the Icon `IR_CALL` site is independently migrated off
  it (separate goal, NOT a Prolog rung).

### M2 — `resolve_choice` CP ledger + `g_resolve_bfr` register

- **Struct:** `src/runtime/interp/resolve_runtime.h:38-49`. Fields: `type`
  (`RESOLVE_CP_CLAUSE/DISJ/RETRY`), `parent` (spine link), `trail_mark`, `env`, `resume`,
  `saved_args`, `cursor`, `stamp`, `saved_cut_flag`, `saved_cut_barrier`.
- **Register + helpers:** `g_resolve_bfr`, `g_resolve_cut_barrier` (`resolve_runtime.c:25-26`);
  `resolve_cp_push/current/...` (`:28+`). Parent-linked record, NOT a contiguous stack, NOT a
  value stack.
- **Reachability:** DEAD-X (only reached through the excised Prolog `bb_exec` path).
- **Verdict:** (c) KEEP THE IDEA. This is the irreducible "which suspended alternative is live"
  ledger — the `_cs`/`pl_choice` analogue the rung's KEEP-list names. The re-grown engine still
  needs a parent-linked CP record carrying `(trail_mark, cursor/resume, saved_args, parent)`.
  `stamp` is the deferred HB port (PL-TRAIL-COND / PL-CP-FRAME-0). Fields `saved_cut_flag` /
  `saved_cut_barrier` are cut bookkeeping and stay. **But** the field set should be re-derived
  from the four-port reference at re-grow time, not copied wholesale — `env` here is a `Term**`
  snapshot that, in the flat per-activation model, becomes the activation's own frame pointer
  rather than a saved/restored slot vector.

### M3 — `PlCallSt` per-call activation record (`bb_exec.c:3310`)

- `typedef struct { Term **callee_env; Term **saved_env; int trail_mark; int nslots;
  bb_node_state_t *act; } PlCallSt;` — built per BB_PL_CALL in `case IR_GOAL`.
- **Reachability:** DEAD-X.
- **Verdict:** SPLIT. `callee_env`/`saved_env`/`nslots`/`trail_mark` are the legitimate
  per-activation frame + trail-mark the `.c` references keep (a `pl_foo_2_r` C frame's locals).
  The embedded `bb_node_state_t *act` is M1 (the value stack) and is the part that goes. In the
  re-grow, `PlCallSt` collapses to roughly `{ frame, saved_frame, trail_mark, cursor }` with NO
  `act` snapshot — matching `archive/.../prolog_emit.c`'s "only surviving dynamic state is `_cs` +
  trail mark."

### M4 — `g_resolve_trail` (the binding-undo trail)

- **Type:** `Trail` (`src/frontend/prolog/prolog_runtime.h:8`). `trail_mark`/`trail_unwind`/`unify`
  /`bind` discipline. ~30 `trail_mark(&g_resolve_trail)` sites across `bb_exec.c`.
- **Reachability:** DEAD-X (Prolog) — though the SAME trail primitives are used live by the
  resolve helpers and by Icon string scanning via `g_resolve_trail` is Prolog-only; Icon uses its
  own. (Two trails exist; do not conflate.)
- **Verdict:** (b) KEEP. Every `.c` reference keeps a trail; it is the binding-undo log, not a
  value stack. The re-grown Prolog engine keeps exactly this.

### M5 — `rt_pl_*` effect helpers + `bb_*.cpp` Prolog templates

- `rt_pl_aggregate_all_term`, `rt_pl_findall`, `rt_pl_is_eval`, `rt_pl_catch`, etc. in `bb_exec.c`;
  referenced by emitter templates `bb_builtin.cpp` (`:46,654,673`), `bb_catch.cpp`, `bb_unify.cpp`.
- **Reachability:** DEAD-X. The templates are mode-4 (x86 emit) artifacts; x86 emit is excised, so
  the templates never run. The helpers' only live-looking callers are those dead templates.
- **Verdict:** KEEP-AS-REFERENCE, do not delete yet. These are faithful transliterations of the
  mode-2 builtin semantics (findall/aggregate/catch/is). When PLG-10 re-grows the
  EVAL/findall/assert analogues onto the explicit indexed deferred-frame array, these bodies are
  the semantic oracle to transliterate FROM — same role the archived emitter plays for control
  flow. Premature deletion would discard correctness knowledge. (They cost only link size while
  dead.)

### M6 — `g_vstack` (the SM value stack — already half-excised)

- `src/runtime/rt/rt.c:63` `static DESCR_t g_vstack[VSTACK_CAP]`; `rt_push_*`/`rt_pop_*` in
  `rt.h`. BUT `rt.c:14-17` shows the push/pop are now BOMB stubs: `"This box must be rebuilt
  stackless (per-box slot, no g_vstack)."` and `[SMX] FATAL: SM value stack push after excision`
  (`:94`).
- **Reachability:** DEAD-X — and actively guarded: any reach is a loud fatal, not silent.
- **Verdict:** (a) ALREADY REMOVED in spirit. This is the value stack the whole Ground-Zero
  directive targeted, and it is already a tripwire. The Prolog re-grow must never call
  `rt_push_*`/`rt_pop_*`; if a re-grown box does, the existing bomb catches it. Nothing to do here
  except never resurrect it.

---

## 2. Cross-reference against the four canonical references (per the rung)

- **Proebsting PDF (`bench/Simple Translation of Goal Directed Evaluation.pdf`)** — four chunks
  `α/β/γ/ω` per operator, threaded by `goto`; start/resume synthesized, succeed/fail inherited; no
  value stack, no recursion. The new `lower.c` IS this model (see its header comment): ports are
  pointer assignments, `bounded` leaves collapse β→ω. M1 (snapshot) has NO analogue in the paper
  → confirms REMOVE.
- **`bench/test_icon.c`** — every box value is a flat statically-declared slot (`int x5_V`,
  `int mult_V`); one C activation, no stack. Confirms M3's frame-of-locals and M1's removal.
- **`bench/test_sno_1.c`** — the ONE unbounded-repetition construct (ARBNO) uses an explicit
  indexed frame array `_1[64]`/`ζ`. This is verdict (d): KEEP the explicit indexed
  deferred-frame array for genuinely-repeating constructs (PLG-10's findall/assert/DCG analogue).
  Cross-check: the current tree has `bb_arbno_state_t` (`IR.h:215`) — the SNOBOL/Icon realization
  of exactly this; the Prolog re-grow's PLG-10 should mirror its shape, not M1's snapshot.
- **`archive/frontend/prolog/prolog_emit.c`** — predicate → C fn with flat `α/β/γ/ω` body; only
  surviving dynamic state across backtracking is `_cs` (resume cursor int) + `trail_mark`; NO
  per-node snapshot, NO value stack; recursion uses the C stack with a fresh frame per activation.
  This is the target shape and corroborates: KEEP M2(cursor/CP-record) + M4(trail) + M3(frame),
  REMOVE M1(snapshot).

---

## 3. Verdict table (the rung's required output)

| # | Mechanism | Reachability | Class | Verdict |
|---|-----------|--------------|-------|---------|
| M1 | `bb_node_state_t` snapshot/restore — Prolog sites (`bb_exec.c` 918/937/3381/3392/3414/3429; `scrip_ir.c` 207/241) | DEAD-X | (a) true value stack | **REMOVE** in re-grow; do not reuse |
| M1′ | same struct — Icon `IR_CALL` site (`bb_exec.c:1589`) | LIVE-ICN | (a)-shaped but Icon | **SEPARATE — out of Prolog scope**; PLG-7 must not delete the struct until this migrates independently |
| M2 | `resolve_choice` CP ledger + `g_resolve_bfr` | DEAD-X | (c) CP cursor/ledger | **KEEP THE IDEA**; re-derive fields from the 4-port refs |
| M3 | `PlCallSt` activation record | DEAD-X | mixed | **SPLIT**: keep frame+mark+cursor, drop embedded `act` (=M1) |
| M4 | `g_resolve_trail` + trail_mark/unwind/unify | DEAD-X | (b) trail | **KEEP** |
| M5 | `rt_pl_*` helpers + `bb_*.cpp` Prolog templates | DEAD-X | semantic oracle | **KEEP AS REFERENCE** (don't delete; transliterate from at PLG-10) |
| M6 | `g_vstack` + rt_push/pop | DEAD-X (bombed) | (a) value stack | **ALREADY REMOVED** (tripwire in place); never resurrect |

---

## 4. Implication for the PLG ladder ordering

PLG-7 ("delete `bb_node_state_t`") is, post-excision, NOT the act of removing live code — the
Prolog snapshot sites are already dead. The real PLG-7 deliverable becomes: when the Prolog engine
is re-grown (PLG-1..6) on the four-port `lower.c`, it must be re-grown WITHOUT M1, so that the dead
Prolog snapshot sites can be deleted as part of removing the whole excised Prolog `bb_exec` path —
AND the deletion must leave the LIVE Icon `IR_CALL:1589` snapshot intact (or migrate it under a
separate Icon goal). The struct definition in `IR.h` cannot be deleted until that Icon site is
handled; that is a cross-language dependency the original PLG-7 text did not have line numbers for
and now does.

PLG-1 (HELLO WORLD, stackless, mode-2) cannot use the old `--run`→`sm_interp_run`→`bb_exec_once`
path at all (it aborts at the SMX gate). It must instead route Prolog through the new
`lower_program.c` `lower()` (which today handles only `LANG_SNO`/`LANG_ICN`) into `lower_goal()`,
then `bb_exec_once(main_graph)` directly — exactly as the SNOBOL4/Icon arms already do in
`scrip.c`'s `mode_interp` branch. That wiring is the true content of PLG-1 and is the recommended
next rung.

## 5. Gate

Doc-only rung. No source touched. All current gates unchanged because nothing executed:
- `./scrip --run corpus/programs/prolog/hello.pl` → rc 134 SMX abort (unchanged, by design).
- `make scrip` + `make libscrip_rt` build green at `cf6b7f6` (verified this session).
- FACT grep unchanged (no template touched).
