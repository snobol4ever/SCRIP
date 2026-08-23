# WAM-CP-6 Step B — LCO Frame-Reuse Design (2026-05-29, Opus 4.7)

**Status:** DESIGN SETTLED — ready to implement. No code changed in this doc;
this is the read-only groundwork that resolves the two open design questions
left by Step A (`860d1163`) so the next session implements directly.

**Predecessor:** Step A LCO-DETECT (`860d1163`) — eligibility detection landed,
gated on `SCRIP_LCO_TRACE=1`, no semantic change. This doc specifies the
semantic change for Step B.

---

## The problem (recap)

Mode-2 Prolog calls recurse on the **C stack**: `bb_exec_node`'s `BB_PL_CALL`
case calls `bb_exec_once(_bcfg)` *internally* (bb_exec.c ~line 3331). Each
Prolog call deepens the C stack by one full `bb_exec_once` frame + the
`bb_exec_node` frame above it. `count(1e6)` → ~1e6 nested C frames → SIGSEGV
(the historical SEGFAULT-CLUSTER).

LCO removes this: when a call is in **tail position** and is **deterministic**,
the caller's frame is dead the instant the callee succeeds, so the callee can
**reuse** the caller's frame instead of stacking a new one.

---

## Key structural finding (this is why the design is cheap)

**The clause-body driver loop is already flat.** `BB_PL_SEQ` (bb_exec.c:3074)
just returns `bb->α`; the single `bb_exec_once` `while(cur)` loop
(bb_exec.c:4319) then walks the entire goal γ-chain of the clause body —
including the tail-position `BB_PL_CALL` — *without any nested `bb_exec_once`
call*. The ONLY nested `bb_exec_once` in a clause body is the one **inside** the
`BB_PL_CALL` handler, descending into the *callee's* graph.

Therefore the trampoline does not need to unwind a deep interpreter call
chain. It needs to do exactly one thing: when a tail+det `BB_PL_CALL` is about
to recurse into `_bcfg`, instead **return control to the current
`bb_exec_once` driver loop with `cur` redirected to `_bcfg->entry`**. The
current driver frame is reused; no new C frame; the callee runs in the same
loop.

---

## RESOLVED — Design Question #1: mechanism (NO new node)

**Decision: use a driver-recognized redirect sentinel. Do NOT add a
`BB_PL_TAIL_CALL` BB op.**

Rationale (the audit that settled it):
- A new BB op forces a `case` in **five** dispatch sites
  (`emit_bb.c`, `emit_core.c`, `emit_sm.c`, `bb_exec.c`,
  `emit_per_kind_audit.c`) plus the FACT-clean mode-4 template obligation, plus
  the per-kind audit gate. High churn, broad regression surface.
- The redirect-sentinel approach adds ZERO enum entries and touches exactly two
  functions in one file (`bb_exec.c`): the `BB_PL_CALL` handler and the
  `bb_exec_once` driver loop. (And `bb_exec_resume`'s identical loop, if tail
  calls can occur on a resume path — see "Resume path" below.)
- LCO is a mode-2 runtime optimization; it does not need a compile-time node.
  The tail-position fact (`bb->γ == NULL`) is already on the existing
  `BB_PL_CALL` node — no lowering change needed.

### Sentinel protocol
Add two file-scope globals in bb_exec.c:
```c
static BB_graph_t *g_pl_tail_redirect_cfg  = NULL;  /* non-NULL ⇒ driver must trampoline */
static BB_t       *g_pl_tail_redirect_entry = NULL;  /* _bcfg->entry to resume at          */
```
In the `BB_PL_CALL` handler, when `eligible` (tail + det) is detected on the
fresh-call path, AFTER binding args into the callee env and setting
`g_pl_env = callee_env`:
- DO NOT call `bb_exec_once(_bcfg)`.
- DO NOT allocate a `PlCallSt` or set `bb->state = 1` (a tail call has no resume
  — that is the whole point of "deterministic").
- Set `g_pl_tail_redirect_cfg = _bcfg; g_pl_tail_redirect_entry = _bcfg->entry;`
- `return NULL;` (or a dedicated sentinel) so the driver loop's "next == NULL"
  branch is reached — BUT the driver must check the redirect FIRST.

In `bb_exec_once` (and `bb_exec_resume`), at the top of each loop iteration
AFTER `next = bb_exec_node(cur);`:
```c
if (g_pl_tail_redirect_cfg) {
    BB_graph_t *tgt = g_pl_tail_redirect_cfg;
    BB_t       *te  = g_pl_tail_redirect_entry;
    g_pl_tail_redirect_cfg = NULL;
    g_pl_tail_redirect_entry = NULL;
    g_current_cfg = tgt;          /* AG ring follows the new graph (see note) */
    bb_reset(tgt);                /* fresh per-node state for the reused frame */
    cur = te;
    continue;                     /* reuse THIS C frame — no recursion */
}
```
Net effect: the same `bb_exec_once` C frame now drives the callee's graph.
Stack stays flat across arbitrarily deep tail recursion.

### `g_current_cfg` note
`g_current_cfg` (bb_exec.c:61) backs the AG value-history ring used by Icon-
style arithmetic/comparison nodes (`ag_ring_peek`). Prolog bodies thread state
via `g_pl_env`/trail and the Prolog BB_ARITH path (bb_exec.c:3442) uses
`pl_node_to_term`, NOT the ring — so Prolog is largely ring-independent. Still,
set `g_current_cfg = tgt` in the redirect for correctness of any ring-using
node reachable in a body, and because the driver already saves/restores it
around the outermost loop.

---

## RESOLVED — Design Question #2: arg-binding aliasing

**The hazard:** the normal (non-LCO) call binds caller args into the callee env
by `unify(callee_param_var, caller_term, &g_pl_trail)` — aliasing through
TERM_REF chains so bindings propagate back to the caller (bb_exec.c ~3319-3325).
Under LCO the caller frame goes away, so we must ensure the callee's bindings do
not dangle against freed caller cells.

**Why it is actually safe here (no copy needed in the common case):**
Our `Term` cells are **GC-allocated individually** (Boehm GC + the
UNCOLLECTABLE sidecar fix from `5bf88205`), NOT slots in a stack frame that gets
truncated. When the caller's `bb_exec_once` frame is reused, the caller's
`g_pl_env` array (a `calloc`'d `Term*` vector) is the only thing that would be
"lost" — but:
1. The arg `unify` already ran BEFORE the redirect, recording bindings on the
   **trail** (a global array), not in the caller env array.
2. The callee env holds its own fresh `term_new_var(ai)` cells; those are
   GC-roots-reachable through `g_pl_env` (now = callee_env) and the trail.
3. The caller's env array becoming unreferenced is exactly what we want — it is
   the frame we are reclaiming. Boehm GC collects it when no longer reachable.

So unlike SWIPL (whose stack-allocated frames force an explicit
`copyFrameArguments`), our GC model means the deref'd arg terms survive
naturally as long as the trail or callee env references them. **No explicit copy
is required for the first cut.**

**The one caveat to verify during implementation:** the caller env array
(`saved_env` in the current code) must not be `free()`d on the tail path. The
normal path frees `callee_env` on failure and stashes it in `PlCallSt` on
success; the tail path must simply NOT free anything and NOT restore
`g_pl_env = saved_env` (we are abandoning the caller frame, so `g_pl_env`
stays = callee_env). Confirm no `free(saved_env)` sneaks in.

---

## Eligibility gate for Step B (narrower than Step A's detector)

Step A logs eligibility for ALL calls. Step B should ACT only on the safe
subset first, expanding later:

**Phase B1 (this rung): act on `eligible=1` for SINGLETON-clause callees only.**
These are the cases the Step-A trace showed as `det=1` today (no clause-
selection CHOICE CP). Detection: `bb->γ == NULL` AND
`g_pl_bfr == lco_entry_bfr` AND `!bb_body_has_live_choice(_bcfg)` AND the callee
predicate has exactly one clause (so no BB_CHOICE wrapper — check
`_bcfg->entry->t != BB_CHOICE`). This is provably safe: no choice point exists
to resume, the call is genuinely det.

**Phase B2 (after WAM-CP-8): extend to multi-clause callees** whose first-arg
index selects exactly one clause (CP elided by indexing). This is where
`count/1` becomes eligible and the 1e6 benchmark unlocks. Depends on WAM-CP-8.

Doing B1 first means the regression surface is tiny (only singleton predicates
in tail position change behavior, and they change from "recurse on C stack" to
"loop in place" — observationally identical output, just no stack growth).

---

## Test plan for Step B

1. **Byte-identical gates** (must hold): GATE-1, GATE-2, GATE-3 m2/m3, GATE-4,
   GATE-SWI m2/m3, FACT 0, sibling smokes. The optimization must not change ANY
   observable output.
2. **New stack-depth gate:** a singleton-tail-chain that would blow the C stack
   without LCO. E.g.:
   ```prolog
   loop(0) :- !.            % base (has a cut — but the recursive clause is what matters)
   loop(N) :- N>0, M is N-1, loop(M).
   ```
   With B1 limited to singleton callees this won't trigger (loop/1 is
   multi-clause). For B1, use a genuine singleton chain that recurses through a
   helper, OR validate B1 purely on the byte-identical gates + the Step-A trace
   showing the singleton calls now take the redirect path (add a
   `SCRIP_LCO_TRACE=2` "ACTED" log line).
3. **Valgrind/ASan spot check** on the singleton-chain test to confirm no
   use-after-free of the abandoned caller env (validates Design Question #2's
   "no copy needed" claim).

---

## Files Step B will touch
- `src/lower/bb_exec.c` ONLY: BB_PL_CALL handler (tail-path branch) +
  bb_exec_once driver loop (redirect check) + bb_exec_resume driver loop
  (same redirect check, if reachable). Two globals. ~40-60 lines.
- No enum change, no emitter change, no lowering change, no FACT obligation.

## Why this is the right scope boundary
Step B as specified (Phase B1, singleton callees) is a single-session,
low-regression change that delivers the mechanism. The big win (`count/1` 1e6)
waits on WAM-CP-8 indexing (Phase B2) — but the mechanism proven in B1 is
exactly what B2 reuses, so B1 is not throwaway. This matches the SWIPL-study
dependency graph: #6 LCO mechanism is independent of #7 indexing, but #6's
*coverage* of the common case depends on #7.
