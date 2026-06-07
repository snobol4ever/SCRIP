# SNOBOL4-5STAGE-OWNED-BUILD.md — Claude-owned design + execution plan

**Mandate (Lon 2026-06-06):** Lon sets boundaries; Claude designs and builds, any way it wants, inside them.
No design questions back to Lon. Done = 100% PASS on all SNOBOL4 mode-4 suites.

**Boundaries (Lon's words):** 5-stage statements using BBs · build a new BB when you need a new piece ·
link them in LOWER any way you want, following the convention all other boxes are lowered and wired ·
read the manual (SPITBOL = semantic authority) · mode 4 only.

**Standing law:** all FACT RULES in GOAL-SNOBOL4-BB.md + RULES.md govern the HOW (template-only emission,
one medium, four Greek ports, no value stack, no C byrd-box calls, per-box ζ/RO storage, language-blind
templates). Sister docs read first: `.github/ARCH-SNOBOL4.md` (native pattern section, Lon-corrected
2026-06-01 — ADOPTED as base), one4all/archive/doc/IR_LOWER_SNOBOL4.md + M-G4-SHARED-CAPTURE.md
(convention lineage: ENMI four-port capture box), HANDOFF-2026-05-31 EUREKA.

## DECISIONS (made by Claude under the mandate; recorded so no session re-litigates)

**D1 — Drive = jmp-threaded INSTANCES (replaces the stale "via bb_broker.c" clause; broker is eradicated,
BROK-3 fence 0 HARD).** A built pattern is a graph of box-INSTANCE records; SEAL at element granularity,
WIRE at instance level (per ARCH-SNOBOL4.md). Instance record (GC-alloc, built by BB_PAT_BUILD/STITCH
boxes): `{ void* α_code; void* β_code; inst* γ_inst; inst* ω_inst; operand fields (sval/ival/child head);
per-instance RW state (saved δ, ARBNO depth/counters) }`. Element matchers get an INSTANCE-MODE twin
template family (own IR kinds per ONE-IR-ONE-LOGIC; same SPITBOL semantics; operands read `[π+off]`
instead of `_.op_*`/rip). A dedicated instance register π carries the current instance; port exits are
pure jmp-threading: γ-exit = `mov π,[π+γ_inst]; jmp [π+α_code]` · ω-exit = `mov π,[π+ω_inst];
jmp [π+β_code]`. Boundary instances at the scan site carry baked-code addresses (match-success label /
RETRY-ADVANCE label), so entering and leaving the built graph is the same two instructions. No broker, no
C-call boxes, no walker loop, no value stack. π register chosen at S3 start against ARCH-x86.md free-reg
map (candidate rbx; must not collide with Σ=r13 δ=r14 Δ=r15 ζ=r12).

**D2 — STITCH_SEQ/STITCH_ALT = runtime wire_seq/wire_alt, same port equations.** Fragment handle =
`{head_inst, γ_dangle (success-exit slot to patch), resume_inst (rightmost β-capable, may be NULL)}`.
CAT: patch a.γ_dangle→b.head; b.head.ω_inst→a.resume (nearest-left-resume, exactly lower.c:57-63 one
layer down). ALT: tiny JOIN instance funnels arm γ-exits (mirrors the IR_PAT_ALT join node); arm_j.ω→
arm_{j+1}.head; last→outer ω. DT_P value = head instance pointer in descr `.p`. Baseline wires even
invariant subtrees at instance level (correctness first); blob freeze is S7.

**D3 — Capture commit (fixes M4-CAPTURE-COND).** COND (`.`): pending ring in the statement ζ frame;
SAVE shell records {ring-mark, δ}; WRITE shell at element-γ APPENDS {nv-ref, sδ, eδ} (no NV_SET);
β-retreat through the capture truncates the ring to its mark; a CAPTURE_COMMIT BB at overall match-γ
flushes the ring (one rt_cap_commit call). IMM (`$`) stays element-γ direct write. `rt_cap_assign_cursor`
keeps is_imm and routes accordingly.

**D4 — Shim policy.** rt_scan/rt_scan_lit/exec_stmt-defer stay as per-shape fallback until S6; every rung
strictly GROWS the native share; m4-only corpus count is a non-decreasing HARD gate per commit. S6 deletes
the shims when the native chain covers the corpus.

## THE LADDER (S-rungs; each = probe-first vs sbl oracle, one commit, m4 gates only)

- [ ] **S1 SUBJECT-EVERYWHERE** — SUBJECT BB on all routes: literal + computed subjects evaluate through
  the value chain into ζ, then Σ/δ/Δ load; drop the named-var-only guard in `flat_drive_scan_native`.
  Kills probe-B BOMB class for invariant patterns. (ph.1)
- [ ] **S2 OPERAND-VARIANCE** — PB-RB-5: `LEN(N)`/`SPAN(cvar)`/`ANY(expr)` read operands late from ζ
  filled by the pre-match value chain; same element boxes; retire by-name string fetch for these shapes. (ph.2a)
- [ ] **S3 INSTANCES+BUILDERS** — D1+D2: instance record, π convention, instance-mode element twins,
  BB_PAT_BUILD + STITCH_SEQ/ALT + REF_INVARIANT boxes, DT_P assign/fetch, pattern-rhs arm in
  flat_drive_assign (kills 053 FATAL), defer box consumes DT_P natively with REAL β re-entry
  (kills exec_stmt landmine + star-var family). (ph.2b)
- [ ] **S4 CH18-CORRECTNESS** — BB_MATCH drives baked AND built graphs; then: one-shot β audit
  (SPAN twins β→ω; ANY/NOTANY/BREAK/LEN/TAB/RTAB/POS/RPOS), FENCE semantics (M4-FENCE cluster),
  ARBNO child-β re-entry. (ph.3)
- [ ] **S5 REPLACEMENT+SUBSTITUTION** — PB-RB-7: REPLACEMENT BB (value chain → ζ, can fail) +
  SUBSTITUTION BB (lvalue check → ω for literal/number subject; splice Σ[0:s]+repl+Σ[e:]; assign back)
  + CAPTURE_COMMIT (D3). Smoke: `S 'b' = 'X'` → `aXc` in m4. (ph.4+5)
- [ ] **S6 CONV** — retire IR_SCAN, rt_scan, rt_scan_lit, exec_stmt defer arm; everything through the
  native chain; delete shim code. (= PB-RB-CONV)
- [ ] **S7 OPT+SWEEP** — invariant blob freeze (PB-RB-OPT) + the non-pattern m4 clusters to 100%:
  M4-DATA (ARRAY/TABLE/DATA), M4-DEFINE, M4-BUILTIN, M4-BEAUTY, remaining M4-CRASH buckets.

**Completion test (= the goal's):** smoke m4 7/7 · pat-rung M4 19/19 no-SKIP · test_mode4_only_corpus_snobol4.sh
280/280 · beauty subsystems m4 17/17. Then Lon rewrites Snocone.

## ⛔ RULES OF ENGAGEMENT (Lon 2026-06-06 — the game rules; violations = rejection)

1. **BBs and XAs ONLY, through TEMPLATES** (emit_core dispatch). No instruction emission anywhere else.
2. **One BB template MAY serve multiple IR kinds — but usually does not** (ONE-IR-ONE-LOGIC holds; N→1 is
   the exception, 1→1 the norm, 1→N never).
3. **NO calls from emitted code into the SCRIP runtime without EXPRESS PERMISSION** — per-symbol,
   Lon-stamped, recorded in the ledger below. The logic lives in BBs; the runtime is not an escape hatch.

### Permission ledger (rule 3: symbol · who needs it · status)
- **nv get/set** (named-variable read/write from emitted code) — S2 operand fetch (`SPAN(cvar)`),
  S3 DT_P assign/fetch, S5 capture-commit stores + substitution write-back. STATUS: **REQUESTED**.
- **raw allocator** (GC_MALLOC-equivalent, bytes in/ptr out, no logic) — S5 substitution splice only
  (new subject string, unbounded size). NOT needed for S3 builds (see D5). STATUS: **REQUESTED**.
- Grandfathered shim calls (`rt_scan*`, `rt_defer_match`, `rt_cap_assign_cursor`): die at their S-rungs;
  zero new uses.

### D5 (forced by rule 3) — instance allocation WITHOUT runtime calls
A build expression's instance count is COMPILE-TIME-KNOWN (the structure is static; only operand values
are dynamic). Builders allocate instances from a compile-time-sized ζ-frame arena (`bb_slot_claim` region),
rebuilt in place on each execution — mirroring SPITBOL's rebuild-per-statement for variable patterns. No
malloc, no runtime call. Escape edge (pattern built in a per-activation frame, stored to a global, used
after return): probe at S3; if live, ledger-request the allocator for that one shape.

### RULE 4 — BB DUMP VISIBILITY (Lon 2026-06-06)
Lon sees the BB dump of the program under work, every session. `--dump-bb` EXISTS (driver scrip.c:1366;
one line per box: `[n] KIND α β γ ω operands`) and is READABLE at statement level but BLIND at pattern
level — IR_SCAN hides its pattern/subject/repl subgraphs (first-look probe 2026-06-06: SPAN statement =
opaque node [10]). GUI deferred (Lon: "do not do it now. Make steps"); FORCE-DIRECTED layout is a hard
requirement when built.

- [x] **S0-DUMP ✅ d312290 (text, FIRST RUNG — the instrument before the surgery)** — extend `--dump-bb`: recurse
  into IR_SCAN pattern/subject/repl subgraphs (indented, labeled `pat:`/`subj:`/`repl:`), same one-line
  four-port format; statement grouping headers (stmt #, label, goto targets); later S3 extension: dump
  built instance graphs (π cells) the same way. Gate: dump of the SPAN probe shows every pattern element
  with all four ports.
- [x] **DUMP-V2 ✅ (Lon 2026-06-07, line-format revamp)** — ONE BB per line:
  `[n] OP  γ=Nα  ω=Nβ  ops:[..] payload`. Operator strips the `IR_` prefix; α/β COLUMNS DROPPED (dying
  fields — lower_sno verified zero α/β usage); instruction number = node `idx` (dense allocation order,
  deterministic — never pointer-derived, the bbN lesson); ports print TARGET idx + entry letter, `·` for
  NULL; `operands[]` printed as idx refs (IR_SCAN excluded — its type-punned graphs render via the
  preserved `pat:`/`subj:`/`repl:` recursion); literal/var payloads kept. LETTER SOURCE (design call,
  stated for veto): bare `IR_t*` ports carry no per-edge entry selector yet — letters render the PORT-LAW
  convention (γ⇒target-α, ω⇒target-β); when IRD-4 retypes γ/ω to `IR_ref_t` the dump reads `ref.sz`
  instead — a one-line swap, recorded here so IRD-4 picks it up. Steps: (1) `IR_`-strip ✅ (2) γ/ω
  renderer idx+letter ✅ (3) `ops:[..]` SCAN-guarded ✅ (4) S0 headers+recursion preserved ✅
  (5) print_port/α/β columns deleted ✅ (6) gate: POS+SPAN+capture probe dumps every element, both ports
  lettered, capture operand visible ✅; m4 battery at floors ✅.
- [ ] **V2-GUI (DEFERRED — on Lon's word only).** Steps, not work: (a) `--dump-bb-json` machine mode off
  the SAME walker as S0-DUMP (nodes: id/kind/ops; edges: port-typed α/β/γ/ω); (b) Python renderer:
  networkx spring_layout (FORCE-DIRECTED, required) → SVG/PNG; node glyph = box with 4 REAL port
  vertices/sides (α=W in, β=S in, γ=E out, ω=N out), edges colored per port; (c) per-statement cluster
  hulls; (d) optional interactive HTML (d3-force) later. No code before Lon says go.
