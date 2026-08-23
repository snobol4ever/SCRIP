# JCON / ICON four-port study → Prolog-BB rung plan (Opus 4.8, 2026-05-29)

**Sources read in full this session:**
- `.github/jcon_irgen.icn` (= `jcon-master/tran/irgen.icn`, 1559 lines) — the 43 `ir_a_*`
  four-port wiring procedures.
- `jcon-master/tran/ir.icn` — the IR-node vocabulary `irgen.icn` emits (the primitive ops
  the four-port wiring decomposes into).
- `jcon-master/jcon/vClosure.java` — the Java runtime's `(retval, Resume())` closure: the
  object form of a four-port box.
- `gprolog-master/src/EnginePl/wam_inst.{c,h}` — `Pl_Create/Update/Delete_Choice_Point`
  and the CP frame layout (`ALTB/CPB/EB/BB/HB/TRB/CSB/AB`).
- `.github/ARCH-ICON.md`, `corpus/.../jcon-ref/README.md`, `icon-references/NOTES.md`.

This study is **plan-only**. It produces no bytes and changes no behaviour. It exists to turn
the reading into the checkable rungs added under `PL-LOWER-REVAMP` in `GOAL-PROLOG-BB.md`.

---

## 1. The central correspondence

Icon and Prolog are the *same* suspension/backtracking abstraction expressed two ways. JCON's
`irgen.icn` is the most complete, battle-tested enumeration of how each control construct wires
the four ports (α start / β resume / γ succeed / ω fail). SCRIP's `lower_pl.c` already aims at
this model; the gaps named in the existing `PL-LOWER-REVAMP` note (monolithic `lower_pl_goal`,
β-by-heuristic, no determinacy flag) are exactly the places where JCON's discipline is missing.

| JCON / Icon (irgen.icn)            | Prolog (SCRIP)                          |
|------------------------------------|-------------------------------------------|
| `bounded` flag (resume port wired only when `/bounded`) | determinacy / CP-elision — whether a goal can offer more solutions |
| `ir.x.continue` / `ir.x.nextlabel` (loop resume entry) | callee `_redo` label; `bb_pl_call.cpp` β path |
| `vClosure(retval, Resume())`       | `pl_choice` CP record + `g_pl_bfr`        |
| `ir_a_Alt` (MoveLabel / IndirectGoto over alternatives) | `BB_CHOICE` clause iteration |
| `ir_a_Call` left-to-right arg eval with `L[i].ω → L[i-1].β` | conjunction backtracking across goals |
| `ir_a_ListConstructor` (`ir_MakeList` after arg sweep) | compound-term builder (`emit_build_compound_term`) |
| `ir_a_ProcBody` + `ir_make_sentinel` | per-predicate callee block sweep in `SM_BB_PL_INVOKE` |
| `Pl_Create/Update/Delete_Choice_Point` (gprolog) | CP push / retry / trust on `BB_CHOICE` |

**Key takeaway:** every Prolog-BB gap on the current NEXT list has a precise, already-debugged
structural template in one of these two reference implementations. The rungs below cite the exact
procedure to transliterate.

---

## 2. Findings worth keeping (with citations)

### F1 — `bounded` IS the determinacy flag (irgen.icn pervasive)
Every `ir_a_*` procedure guards its resume chunk with `/bounded & suspend ir_chunk(p.ir.resume, …)`.
When a construct is *bounded* (cannot be resumed for more values) JCON emits **no resume wiring at
all** — the box is deterministic and the β port is dead code. This is the single most reused idea in
the file. SCRIP's `lower_pl.c` has no equivalent flag (per the existing PL-LOWER-REVAMP note),
so it wires β unconditionally and then relies on a runtime "nearest resumable predecessor" heuristic.
Adopting an explicit `bounded`-style flag at lower time is the structural fix for the CAT-A-3
backtracking class AND the enabler for CP elision (WAM-CP-12).

### F2 — Resume is an explicit per-node port, never a heuristic (irgen.icn, all procs)
JCON wires β by *name*: e.g. in `ir_a_Binop` the right operand's failure goes to
`p.left.ir.resume` (line 504), and the box's own resume goes to `p.right.ir.resume`. There is no
search for "the last resumable thing"; every edge is explicit and local. This is the direct
remedy for PL-LOWER-REVAMP gap (2).

### F3 — `ir_a_Call` is the conjunction-backtracking template (irgen.icn 360-403)
Arguments are evaluated left to right; each `L[i].ir.failure` routes to `L[i-1].ir.resume`
(line 394), and `L[1].ir.failure` routes to the box's ω (line 396). The call itself parks a
resume label via `ir_ResumeValue` (line 389) so the *callee* can be re-driven. This is exactly
Prolog conjunction `(A, B, C)`: on B's failure, redrive A. The `BB_PL_SEQ` row of the existing
port table matches this, but the explicit `ir_ResumeValue` "park a redo label" step is what
`bb_pl_call.cpp`'s β path must mirror in the binary arm.

### F4 — `ir_a_ListConstructor` is the compound-term builder template (irgen.icn 1313-1354)
Evaluate each element into `args[i]` (each with its own resume chain), then at the trailing
sentinel emit a single `ir_MakeList(target, args)` (line 1346). The sentinel pattern
(`ir_make_sentinel`, line 1492) wraps the list with a head/tail NoOp so the first/last edges are
uniform. This is the structural shape for porting `emit_build_compound_term` to raw bytes
(the deferred priority #2): build args, then one `rt_pl_compound_build_n` call.

### F5 — `ir_a_ProcBody` + sentinel is the callee-block sweep (irgen.icn 774-804)
A procedure body is a sentinel-wrapped statement list; each element wires
`L[i].ir.success → L[i+1].ir.start` and `L[i].ir.failure → L[i+1].ir.start` (lines 795-798,
note: BOTH success and failure advance — proc-body statements are run for effect). The
`SM_BB_PL_INVOKE` callee-block loop (priority #1) is this sweep with Prolog success/fail semantics
(failure backtracks rather than advancing).

### F6 — gprolog CP frame is the ground truth for retry/trust (wam_inst.h 90-104, wam_inst.c 1407-1455)
`CREATE_CHOICE_COMMON_PART` saves `ALTB`(next clause), `CPB`(continuation), `EB`(env),
`BB`(parent CP), `HB`(heap top), `TRB`(trail top), `CSB` + args. `UPDATE` (retry) does
**`Pl_Untrail(TRB(cur_B))` first**, restores `H/CP/E`, then sets a new `ALTB`. `DELETE` (trust)
untrails, restores, and pops `B = BB(cur_B)`. SCRIP's `pl_choice` already documents this exact
mapping (`pl_runtime.h:47-68`: `parent==BB, trail_mark==TRB, env==EB, resume==ALTB,
saved_args==AB, stamp==STAMP`). The untrail-before-retry ordering is the invariant the
`BB_CHOICE` binary arm must preserve.

### F7 — `vClosure` confirms the box-as-object model (vClosure.java 8-16)
A `vClosure` is `(retval, Resume())`. γ = `retval`; β = `Resume()`; ω = `Resume()` returning a
fail sentinel. This is reassurance that the CP-record-as-suspended-box design in SCRIP is the
same design JCON ships — nothing exotic is required.

---

## 3. What is explicitly NOT proposed
- No co-expression / `create E` work (Prolog has no analog; irgen.icn `ir_a_Create` is Icon-only).
- No string-scanning (`ir_a_Scan`) — Prolog has no `?` subject/pos machinery.
- No change to the runtime CP model — gprolog reading *corroborates* the existing `pl_choice`
  layout; it does not call for revising it.

---

## 4. Rung set (added to GOAL-PROLOG-BB.md under PL-LOWER-REVAMP as PLR-J-*)
See the goal file. Sequenced so each rung is independently verifiable against mode-2 (`--run`,
the correctness reference) and lands no bytes outside `*_templates/` (FACT rule).
