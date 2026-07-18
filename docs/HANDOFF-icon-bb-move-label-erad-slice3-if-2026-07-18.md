# HANDOFF — GOAL-ICON-BB: ICN-MOVE-LABEL-ERAD slice 3 — lower_if → committed nary DISJUNCTION; IR_INDIRECT_GOTO retired
Date: 2026-07-18 (Claude Fable, same-day follow-on to slices 1+2 at `00a027ca`)

## Landed this session

### Slice 3 — `lower_if` rewritten as a COMMITTED 2-way (1-way when else absent) IR_DISJUNCTION self-state box
GOAL-ICON-BB NEXT SESSION CURSOR (§6 of FINDING-2026-07-15-...-BB-SELF-STATE) executed. Zero new IR
kinds, zero template changes, zero emitter changes for the box itself — the committed deltas are wiring:

- **arm0 entry = C's entry** (C lowered succ=T_entry, fail=dj → C's ω-edges retag "φ"). C-fail lands the
  φ-glue whose alt_i++/dispatch IS the branch selection: 0→1 enters E; N=1 (no else) exhausts → ω.
- **T and E lower with succ=dj ("σ" retag → per-arm result copy into the box's own slot, option B verbatim)
  and fail=ω-OUT** — the plain escape is the commit: then-exhaust never falls into else, else-exhaust never
  advances; the φ-glue only ever fires from C-fail, so alt_i is exactly the taken-branch index β dispatches.
- **resume_j** = the arm's own β surface when generator (ab-in-arm scan, lower_alt verbatim), else the shared
  **IR_FAIL sentinel** — chain-BFS-filtered, so the pair row falls to its node_ω default: resuming an
  exhausted valueless branch ≡ fail outward. NOT the dj self-marker (self ≡ advance would leak then-resume
  into else).
- C is bounded by construction (interp.r Op_Mark/Op_Unmark parity): its resume surface never enters the pair
  table. C's abandoned suspensions remain ICN-BOUND-UNMARK ladder territory (unchanged from the old ml/ig
  model).

**One real bug found and fixed in-slice** (rung02_proc_fact 120→0, rung36_jcon_mathfunc): the dj operand walk
(emit.cpp chain BFS ~1623) queues ALL operands — an IR_RETURN pushed as a trailing result operand drives
BEFORE its value producer arrives on the γ-spine; bb_slot_get misses (IR_RETURN's drive has no nd_slot
fallback, unlike IR_MOVE_LABEL:1419's) and bb_return emits the &null descriptor. FIX: lower_if pushes NULL
instead of wiring-kind arm results (GOTO/SUCCEED/FAIL/RETURN/SUSPEND/CORET/COFAIL) — they carry no value
slot and never σ-land. NOTE the latent sibling hazard: lower_alt pushes arm results unfiltered, so an
alternation arm ending in `return` has the same early-drive exposure — REPRODUCED at HEAD (`absv: n > 0 | return -n` → &null) and
FIXED same session: shared icn_arm_result filter now used by BOTH lower_alt and lower_if; locked under proof
by corpus rung02_if_return_arm + rung13_alt_return_arm (both byte-match). The alternative shared-code fix
(nd_slot fallback in IR_RETURN's drive, MOVE_LABEL:1419 precedent) remains open for a crosscheck-gated
session.

### Slice 4 — IR_INDIRECT_GOTO retired (zero producers anywhere)
lower_if:891 was the ONLY producer in any lowerer. Pruned: emit.cpp case labels (×2, DISJUNCTION keeps the
0-operand legacy Prolog-gate arm → bb_indirect_goto template STAYS live for it), zeta_storage.c:176 case
label, and the three dead optimizer γ-protections (dead_pure.c / branch_chain.c / copy_prop.c). Kind stays
DECLARED in IR.h + scrip_ir.c (enum removal shifts opcode values); drive_unowned aborts LOUD if one ever
reappears.

## Census after this session
- Icon IR_MOVE_LABEL producers: **0** (kind stays declared for Prolog, lower_prolog.c:718, ival pinned).
- IR_INDIRECT_GOTO producers: **0 globally**; emit/optimizer/zls references pruned; template retained for the
  0-operand DISJUNCTION Prolog gate only.
- Icon IR_GOTO producers: **19 lines / ~14 sites** in lower_icon.c; 4–7 live GOTO nodes per typical graph.

## VERIFIED (full battery, this tree)
- Icon rung suite **243/15/32 ×3 modes**, FAIL name-sets **byte-identical** to pristine-HEAD baseline in both
  run and compile modes (comm empty; the 15 = the rung36_jcon_* set).
- Icon smoke 14/14 ×2; gates icn_no_stack OK, icn_one_reg_frame OK, semicolon prison PASS.
- SNOBOL4 smoke 7/7; Prolog smoke 5/5 ×2; Raku 288/288 ×2 (emit.cpp/optimizer/zls touched → sibling proof).
- Probes m3+m4 byte-identical: 4-way committed selection; if-value read-through (`x := if c then "a" else
  "b"`); generator arms both branches (`every write(if c then (1 to 3) else (7 to 9))`); nested alternation
  in arm (`(1 to 2) | (8 to 9)`); bounded generator condition (`if (1 to 3) > 2`); return-in-then recursion
  (fact).

## NEXT CURSOR — IR_GOTO survey (the α-entry-vs-auto-β-promotion protocol rung, unchanged from GOAL doc)
Every surviving Icon IR_GOTO is a protocol artifact, not control flow: the trampoline absorbs callers'
auto-β-promotion so a fresh edge enters a generator-kind box at α (the s95 stale-alt_i by-bug class). Sites:
dj α-entry tramps (lower_alt + lower_if, now uniform — unify first), STMT-BOUNDARY α-force tramps, break/next,
scan leave tramps, while/until glue, seed, SENT, body-less every. Eradication = give LOWER a way to say
"raw-α edge, do not promote" (edge-tag or lc_γ_to_α! helper) + emitter honor + per-site conversion, one
gated slice each. bb_goto emits only an α-def + pair-jmp and the emitter's 128-hop GOTO-chase already folds
them off the hot path, so this is IR-hygiene, not perf — sequence it after the higher-value bench lanes.

## Files changed
- src/lower/lower_icon.c        (lower_if: MOVE_LABEL/INDIRECT_GOTO → committed nary DISJUNCTION; wiring-kind
                                 result filter)
- src/emitter/emit.cpp          (IR_INDIRECT_GOTO case labels retired ×2)
- src/contracts/zeta_storage.c  (case label retired)
- src/optimizer/dead_pure.c, branch_chain.c, copy_prop.c (dead INDIRECT_GOTO protections removed)
- docs/HANDOFF-icon-bb-move-label-erad-slice3-if-2026-07-18.md (this file)
