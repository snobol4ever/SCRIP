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
- **nv get** (`rt_nv_cstr`, named-variable read from emitted code, `const char*(const char*)`) —
  S2 operand fetch (`SPAN(cvar)` immediate scan, via `bb_match_span_var`), stored single-var cset
  construction-time evaluate-and-bake (`P=ANY(var)`/SPAN/BREAK/BREAKX/NOTANY, via `bb_pattern_unary_s`).
  STATUS: **STAMPED** (Lon 2026-06-14).
- **nv set** (named-variable write from emitted code) — S3 DT_P assign/fetch (covered by the already-stamped
  `rt_gvar_assign_pat`), S5 capture-commit stores + substitution write-back. STATUS: **REQUESTED**.
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
- [x] **DUMP-V2 ✅ + V2b CORRECTION (Lon 2026-06-07; first cut VETOED live — nested/indented output was wrong)** —
  FLAT LINEAR COLUMNAR listing: ONE BB per line `[n] OP  γ=Nα  ω=Nβ  ops:[..] payload`, column 0, ZERO
  indentation, ZERO `pat:`/`subj:`/`repl:` labels. EVERY slot 0..n-1 prints — NULL slots render `[n] ·`,
  never skipped. Operator strips `IR_`; α/β columns DELETED; instruction number = node `idx` (dense,
  deterministic); ports = TARGET idx + entry letter (PORT-LAW convention until IRD-4 `IR_ref_t.sz` — one-line
  swap recorded here); `operands[]` as idx refs (SCAN-guarded). Sub-graphs still produced by the lowering
  (SCAN pat/subj/repl via `IR_alloc` lower_sno.c:753) print as SIBLING flat tables — a stopgap: the
  architecture is ONE linear slot sequence, reached when builder slots (ILIT/POS/ILIT/LEN…) inline into the
  one graph; the sub-graph tables then vanish on their own. ⛔ KNOWN RESIDUE: ARBNO inner graphs hang off a
  lower-internal `az` sidecar punned into EXEC.counter (lower_sno.c:902-911) — NOT reachable from contracts
  without a layering violation, so they remain undumped; dies with the linear-builder lowering (same S0
  residue note). Gate: POS+SPAN+capture probe dumps 24/24 slots across 3 flat tables, both ports lettered ✅;
  m4 battery at floors ✅.
- [ ] **V2-GUI (DEFERRED — on Lon's word only).** Steps, not work: (a) `--dump-bb-json` machine mode off
  the SAME walker as S0-DUMP (nodes: id/kind/ops; edges: port-typed α/β/γ/ω); (b) Python renderer:
  networkx spring_layout (FORCE-DIRECTED, required) → SVG/PNG; node glyph = box with 4 REAL port
  vertices/sides (α=W in, β=S in, γ=E out, ω=N out), edges colored per port; (c) per-statement cluster
  hulls; (d) optional interactive HTML (d3-force) later. No code before Lon says go.

## D6 — DT_P = EXECUTABLE CODE SEGMENTS (Lon 2026-06-07 session 2; ⛔ SUPERSEDED BY D7 2026-06-08 — kept for lineage)

Builders build THE REAL THING: BB_PATTERN_* boxes copy position-independent PROTOTYPE BLOBS (.rodata, emitted
at compile time by element templates: code + co-located operand/RW slots, rip-internal only, CALL-FREE) into the
RWX pattern pool (pat_pool.c, B1), fill operand slots, leave the two outbound ports as patch sites. STITCH = patch
linkage (runtime wire_seq/wire_alt on rel32/abs64 sites). DT_P value = head block DTP_t {entry, out_γ, out_ω}
written in-segment (dtp.h). Match: jmp entry, exits jmp through out slots. NO IR_t at runtime, NO instance records,
NO interpreter near a pattern. **TRANSIENT-OVERHEAD RULE (Lon):** fragment handles DTP_FRAG_t {entry, γ_site,
ω_site} are ζ-resident during the building statement and DISCARDED after stitch — builder parts exist solely to
build the bigger pattern; only the head block survives. Like strings: intermediates are garbage after concat.
Constraints this imposes (healthy): prototypes call-free (capture/NV work forced to statement boundary = D3);
rebuild-per-evaluation (SPITBOL's own behavior for variable patterns); collection deferred (bump alloc, reset
groundwork in pat_pool_reset; true reclamation B11).

## D7 — GENERIC RT BUILD/STITCH + SELF-DESCRIBING PROTOS (Lon 2026-06-08 PIVOT; SUPERSEDES D6)

**The mess D7 fixes (Lon "Yikes!"):** D6 builders stitch by patching ABSOLUTE jump addresses into prototype
blobs at their runtime-varying pool locations, using HARDCODED offsets (`+8/+16/+24`, β@+32) written by hand in
every element template AND every stitch box (`bb_pattern_cat` patches `[r12+sa+8]`, `[r12+sb+16]`, … literally).
That is the same hand-counted-offset disease the `bb_bin_t`-ABOLISHED FACT RULE outlaws — relocated from
compile-time byte-counting into runtime offset arithmetic. One proto layout change silently rots every offset in
every box (the B6 β-derivation note, the B7b abort_site-won't-propagate bug, the B9 4th-slot note are all ONE rot:
"offset +24 isn't carried through CAT"). It also forced the build asm to be re-expressed per medium, producing 1101
`ins*` text-only passthrough calls that have NO binary encoding → **mode-3 emits nothing for the builders → 053
PASSES m4 but FAILS m3 (and m2). That single line is the parity break.**

**THE CORRECTION — three Lon directives, ONE end state:** (1) standardized patch technique, generic at the build
site; (2) the map comes from the concat-string BEGIN/END MARKERS (the `bb_emit_x86` marker-walk — positions
DISCOVERED, never hardcoded); (3) all building+patching lives in the RT as functions. These are one change:

- **Self-describing protos.** Each element template emits its PIC proto blob ONCE; the patch-site offsets
  (entry, β, γ_site, ω_site, abort_site, operand slots) are DISCOVERED at compile time by the marker-walk and
  written into a per-proto `DTP_PROTO_DESC` emitted alongside the blob. ZERO hardcoded offsets anywhere.
- **Generic RT build.** `rt_pattern_build(const void *proto, uint32_t len, const DTP_PROTO_DESC *desc, long op_i,
  const char *op_s)` copies the proto into the pool, fills operand slots via `desc`, returns a `DTP_FRAG_t` whose
  entry/γ_site/ω_site = `pool + desc->{entry,gamma,omega}_off`. ONE function, every element kind.
- **Generic RT stitch.** `rt_pattern_stitch_cat(DTP_FRAG_t *out, const DTP_FRAG_t *l, const DTP_FRAG_t *r)` and
  `_alt(...)` patch via the frag's discovered sites — `*l->γ_site = r->entry`, `*r->ω_site = l->β`, etc. — NO
  literal offsets. The stitch is layout-agnostic; protos can change shape freely.
- **Templates collapse.** A builder box becomes: marshal `proto/len/desc/operands` → `call rt_pattern_*` → store
  frag → wire 4 ports. The proto blob + desc are DATA (legit text/binary-different, not instructions); the only
  asm left is `call` + port jumps — both have real dual-medium `x86()` encoders. So the `ins*` text-only forms in
  the pattern builders DELETE (the asm that used them is gone), and modes 3/4 are byte-identical logic again.

**WHY THIS IS THE PARITY FIX, not just cleanup:** the RT is MODE-AGNOSTIC. The mode-2 interpreter calls
`rt_pattern_build`/`rt_pattern_stitch_*` DIRECTLY; modes 3/4 emit a `call` to the same symbols. ONE implementation,
THREE modes, parity by construction. 053 goes green in m2 AND m3 AND m4 from the same code. **m2==m3==m4 becomes
the gate** (co-equal HARD), replacing the m4-only stance.

**LEDGER STAMP (RULE 3 — Lon granted by the "put the patching in the RT" directive, 2026-06-08):** emitted code MAY
`call` `rt_pattern_build`, `rt_pattern_stitch_cat`, `rt_pattern_stitch_alt`, `rt_gvar_assign_pat` (already stamped).
These are the generic build/stitch family; the per-element/per-stitch INLINE asm builders are retired into them.

**SUBSTRATE KEPT:** `pat_pool.c` (RWX bump arena) + `dtp.h` `DTP_t`{entry,out_γ,out_ω} head. `DTP_FRAG_t` gains
nothing structurally (still {entry,γ_site,ω_site}); the descriptor `DTP_PROTO_DESC` is NEW (compile-time-filled).
Element PROTO BLOBS are kept as PIC data (their hand-built α/β bodies stay — they are the matching logic), but
their COPY/FILL/PATCH machinery moves out of template asm into `rt_pattern_build`/`_stitch`.

## D7 CONVERSION LADDER (Lon "continue" 2026-06-08; box-by-box — frag HELD at 3-field/24B so converted+unconverted boxes interoperate)

**VERIFIED LIVE THIS SESSION (do not re-derive):**
- `rt_pattern_build` / `rt_pattern_stitch_cat` / `rt_pattern_stitch_alt` LANDED + linked (out-pointer sig; 3-field frag {entry@+0, γ_site@+8, ω_site@+16} = the existing emitted ζ-layout). β-first-class DEFERRED — frag stays 3-field so unconverted boxes still read it; stitch derives β = `(uint8_t*)ω_site + 8` exactly as the old boxes. B9 first-class-β rides a future COORDINATED frag-widening (all boxes at once).
- **BUILDER PATH IS WIRED FOR VARIABLE SUBJECTS ONLY.** `X P . V` (juxtaposition, X a var) routes native + works m4. `'lit' ? P` (literal subject) is the unrouted rt_scan path → EMPTY IN ALL MODES — NOT a builder bug (it is S1's job). Never probe builders with a literal subject.
- `053_pat_alt_commit` = `P=('a'|'b'|'c'); X='b'; X P . V :S(YES)F(NO)` → .ref `b`; CURRENTLY m4 PASS / m3 FAIL / m2 FAIL. Ready-made proof case (already in pat-rung). m3/m2 fail ONLY because its 3 boxes are `ins*` text-only (no binary encoding) and the interpreter has no build arm. sbl re-confirmed: `('a'|'b'|'c')` on `cat` → `c`.
- `P=LEN(3)` VALUE-assign is EMPTY IN ALL MODES incl m4 — the LEN-value LOWERING is unrouted (separate gap, D7-RB-3, NOT a template bug). B4's "073→hel" was anonymous-scan `S ? LEN(3)`, a different path.

### D7-RB-1 — 053 PARITY (convert LIT + ALT + DTP_ASSIGN + the m2 interpreter arm; goal m2==m3==m4 on 053)
PROOF = pat-rung `053` → m3 PASS + m2 PASS (m4 stays PASS), plus `050`/`051` (alt-two/three) hold.
PER-BOX MECHANISM (mirror across the three emitter templates):
0. **PREREQUISITE — ADD THE MISSING x86() ENCODERS (this is WHY `ins*` exists; the missing encoder IS the bug per the FACT RULE).** The marshal sequence needs dual-medium encoders `x86_asm.h` LACKS today (B2 finding: silently dropped, so the boxes used `ins2` instead): `lea reg,[reg+disp32]` (for `lea rdi,[r12+off]`, `lea rcx,[rax+disp]`) and a label-diff IMMEDIATE operand (for `mov edx, .Lx_e-.Lx_s` — or derive len another way). ALREADY present (used without `ins2`, so reuse freely): `mov reg,imm`, `lea reg,[rip+label]` (the `x86("lea",r,"[rip + __]",addr,label)` form), `mov reg,reg`, `add/sub/cmp reg,reg`, `call SYM` (pass the fn address like `da_fn()` in bb_dtp_assign), stack-align idiom `push rbx; mov rbx,rsp; and rsp,-16; …; mov rsp,rbx; pop rbx`. ADD the 1–2 missing forms, byte-verify each vs `as`, THEN every box body below converts with ZERO `ins*`. This single encoder set unblocks the entire 1101-site `ins*` purge ring-wide — convert-by-deletion follows mechanically once the encoders exist.
1. KEEP each proto's α/β matching body AS-IS; just LABEL every site: `.Lx_s`(start) `.Lx_a`(α) `.Lx_g`(out_γ slot) `.Lx_w`(out_ω slot) `.Lx_op1`(operand, if any) `.Lx_e`(end); β already labelled.
2. Emit a `DTP_PROTO_DESC` data blob `.Lx_desc`: `.long .Lx_a-.Lx_s` · `.long β-.Lx_s` · `.long .Lx_g-.Lx_s` · `.long .Lx_w-.Lx_s` · `.long -1`(abort) · `.long .Lx_op1-.Lx_s` (or -1) · `.long -1`(op2; LIT uses op2 for len). The ASSEMBLER discovers all offsets — zero C/template byte-counting, immune to layout shift (the entire point of D7).
3. DELETE the inline copy/fill/patch asm (the `ins*` block) and replace with marshal+call (out-pointer ABI):
   - element build (LIT): `lea rdi,[r12+off]`(out frag) · `lea rsi,[rip+.Lx_s]` · `mov edx,.Lx_e-.Lx_s` · `lea rcx,[rip+.Lx_desc]` · `mov r8,<len>` · `mov r9,<lit strtab ptr>` · align rsp · `call rt_pattern_build` · `jmp γ`. (unary_i later: r8=n, r9=0.)
   - ALT stitch: `lea rdi,[r12+out]` · `lea rsi,[r12+left]` · `lea rdx,[r12+right]` · align · `call rt_pattern_stitch_alt` · `jmp γ`.
   - DTP_ASSIGN: keep `rt_gvar_assign_pat` (stamped); its head-copy converts to a minimal call OR stays — the frag it reads is unchanged.
   ONLY asm left per box = marshal regs + `call` + `jmp γ`/ports — all have real dual-medium `x86()` encoders ⇒ m3≡m4, ZERO `ins*`.
4. INTERPRETER ARM (m2): `IR_interp.c` cases for IR_PATTERN_LIT / IR_PATTERN_ALT / IR_DTP_ASSIGN call the SAME `rt_pattern_build`/`rt_pattern_stitch_alt`/`rt_gvar_assign_pat` directly (operands from the IR node). This is what makes m2 match m3/m4 — the parity-by-shared-RT mechanism.
5. PROBE 053/050/051 vs .ref across m2/m3/m4 → `b`/`dog`/`banana`. FLIP the pat-rung gate to require PASS-M2==PASS-M3==PASS-M4 (parity), not m4-only. Smoke stays 7/7/7.
FILES: `bb_pattern_lit.cpp`, `bb_pattern_alt.cpp`, `bb_dtp_assign.cpp`, `IR_interp.c` (3 cases), `emit_bb.c` (FILL marshals proto/desc/operand regs). The `x86("call","rt_*")` SYM-target form already exists. NO frag-layout change, NO Makefile change.
GATE: smoke 7/7/7 · pat-rung 053+050+051 m2==m3==m4 · corpus non-decreasing · fence HARD.
THEN D7-RB-2: convert unary_i/unary_s/nullary/arb/cat identically (each its own commit, frag held) — purges `ins*` box-by-box (1101→0 in pattern builders). D7-RB-3: the LEN-value lowering gap. FINAL: coordinated frag-widening → first-class β (closes B9), once every box is on the RT functions.




- [x] **B0 PATND-DELETE** ✅ 07698c7+27c797f — patnd.h/constructors/cache/type-puns deleted (−781); .p → struct _DTP_t*; 8 nullary primitives → NULL-head DT_P placeholders (startup NV init survives, use-time bombs); grep PATND == 0.
- [x] **B0b AST-WALK DELETE** ✅ b7a2717 — eval_node + interp_eval_pat → bombs (−471; Lon: nothing interprets tree_t at runtime).
- [x] **B1 SEGMENT SUBSTRATE** ✅ fa0ebcc — dtp.h (DTP_t head + DTP_FRAG_t handle) + pat_pool.c (4MB RWX arena, g_pat_pool_base/cur/end exported for pure-store bump alloc; rule-3 clean); init at all three bb_pool_init sites.
- [x] **B2 PROTO-LIT** ✅ — bb_pattern_lit.cpp lands the D6 builder: PIC call-free LIT blob (FIXED LAYOUT: +0 lit_ptr · +8 lit_len · +16 out_γ · +24 out_ω abs64 patch sites · **β entry at +32 = UNIVERSAL convention for all element protos** · α at assembler-resolved label-diff) emitted IN-LINE post-`jmp γ` (never executed in place; copied by `rep movsb`) — NOT .rodata-section-switched (avoids XA-framing interplay; copy semantics identical); metadata struct COLLAPSED into the fixed layout + gas label-diff immediates (`.Lpb_e-.Lpb_s` size, `.Lpb_a-.Lpb_s` entry) — no C-side byte counting, assembler discovers everything. One-shot β: reload len, `sub r14d`, jmp out_ω (bb_lit δ-restore semantics). Builder: bump-alloc g_pat_pool_cur (kw_anchor RIPSEAL idiom; store-back `mov [rip+sym],rdi`), copy, fill operands (strtab lit lea + len imm), DTP_FRAG_t {entry,γ_site,ω_site} → ζ `[r12+op_off+0/8/16]`. 26 IR_PATTERN_*/IR_DTP_ASSIGN kinds + knames (dump-readable, taxonomy per-primitive) + emit_core dispatch; family bomb-stubbed via bb_pattern_stub(msg) — BOMB messages = the work queue (B3/B4/B5/B6/B7/B8/B9/B10 tagged). lower_pattern_build (lower_sno.c, exported lower_internal.h): TT_QLIT→IR_PATTERN_LIT arm + NULL-fallback default; **UNROUTED — routing = B3's flat_drive_assign pattern-rhs arm** (rerouting now would pull live shapes off the passing shim path). PROOF: standalone #include harness (17th-run pattern; recipe in HANDOFF-2026-06-07-…-B2) → emitted text `as`-assembles rc=0, objdump-verified end-to-end (frag stores ζ+48/56/64, blob slots, β +32, FF25 indirect exits). ⛔ FINDING (ring-wide): x86() parsed forms SILENTLY DROP unrecognized operand shapes (empty string) — `lea r,[rax+imm]` / `lea r,[r13+rax]` not in repertoire → ins2; objdump-verify every new operand shape. ⛔ B3 PROBE NOTE: emitted store to g_pat_pool_cur relies on copy-reloc unification (-no-pie link) — sanity-probe lib-writer (pat_pool_init) vs exe-writer coherence at first live build. Gates at floors: smoke 7/7 · pat 18/0/1 · corpus 148/280 · fence HARD.
- [x] **B3 STITCH-ALT + DTP-ASSIGN + DEFER-RUN** ✅ — bb_pattern_alt.cpp (pairwise stitch: patch *left.ω_site=right.entry; copied JOIN thunk {+0 slot · _j `jmp [rip+slot]`} funnels both arm γ_sites; out frag {left.entry, &join.slot, right.ω_site}); bb_dtp_assign.cpp (in-segment head per D6: {entry · out_γ · out_ω} + gthunk/wthunk `jmp [rip+_s+8/16]`; patches frag dangles INTO the head so per-match patching = 2 head slots regardless of pattern size; calls rt_gvar_assign_pat(name, head) via push-rbx/and-rsp,-16 idiom). Routing: lower_sno_assign pattern-RHS arm (TT_ALT + sno_pattern_buildable whitelist, NULL→legacy fallback) → lower_pattern_build TT_ALT arm (left-assoc pairwise, kids chained γ-sequential, IR_PATTERN_ALT operands pushed); driver: descr_chain_arity LIT=0/ALT=2/DTP_ASSIGN=1 (slot-threaded RPN re-derives), walk_bb_flat FILL cases + bb_slot_alloc24 frag slots. Defer-run: rt_dtp_run (file-scope global asm, pattern_match.c) — SysV (h,Σ,δ,Δ), pushes rbx/r12-r15, patches h.out_γ/out_ω→local trampolines, r13/r14/r15=Σ/δ/Δ, `jmp *0(h)`; γ-tramp returns r14, ω-tramp -1; rt_defer_match DT_P arm replaces B0 BOMB (defer β stays one-shot→ω; true downstream-retreat re-entry = B10, landmine narrowed: 053 has no post-DT_P failing element). ⛔ FINDING (B2 latent, surfaced first live run): `x86("mov",r,"[rip + __]",addr,sym)` emits **lea** (address-of) — right for strtab literals, WRONG for pool-cursor load → builders corrupted .bss at &g_pat_pool_cur; fixed in all three builders with ins2 `mov rax, qword ptr [rip + g_pat_pool_cur]` (standalone B2 proof couldn't catch: never executed the copy live). ⛔ FINDING: emitted-exe path (main → rt_proc_reset/rt_frame only) never reaches pat_pool_init → first builder rep movsb rdi=0; fix = `__attribute__((constructor))` pat_pool_ctor in pat_pool.c (idempotent, zero new emitted→runtime symbols, all 3 existing call sites intact). GATE MET: **053 → `b` (sbl-matched) · pat-rung 19/19 no-SKIP** · smoke 7/7 · corpus **151**/280 (floor 148, +3) · beauty 1/17 · fence HARD.
- [x] **B4 UNARY_I protos** ✅ — LEN/POS/RPOS/TAB/RTAB (`P = LEN(3)`, 073 family). ONE kind-discriminated template bb_pattern_unary_i.cpp (driver sets op_kind string per bb_match_* precedent; five IR kinds stay distinct, dump-readable): universal slot layout HELD — +0 n (dword-stored into quad) · **+8 saved-δ scratch (TAB/RTAB β-restore)** · +16 out_γ · +24 out_ω · β@+32 — so B6's `base = ω_site−24, β = ω_site+8` derivation stays valid family-wide. α semantics per SPITBOL ch.14: LEN δ+n≤Δ guard; POS δ==n; RPOS δ==Δ−n; TAB n≥δ ∧ n≤Δ, save δ→+8, δ=n; RTAB Δ−n≥δ, save, δ=Δ−n. β: LEN `sub r14d,n`; POS/RPOS pass-through; TAB/RTAB reload +8. ⛔ SCRATCH LANDMINE (inscribe for B9): +8 is per-INSTANCE single-slot — ARBNO re-entering one instance multiple times within a match needs per-activation save discipline (D5 instance-count or stack); fine for B4-B8 single-outstanding-activation chains. Lowering: five lower_pattern_build arms (TT_ILIT args ONLY; TT_VAR → NULL → legacy, that's B10 *V territory); sno_pattern_buildable extended; route widened TT_ALT-only → any buildable non-TT_QLIT RHS. Driver: arity 0 ×5, walk FILL cases ×5 with op_kind. ⛔ FINDING: bb_match_atp was the ONLY template using emit_intern_str with NO strtab_label fallback (and lower_flat_set_intern_str has ZERO callers ring-wide — intern ALWAYS returns NULL; every label everywhere rides the .S-strtab fallback) → @cursor emitted `lea rdi,[rip + ??]`, dormant until B4's route made 074's scan emit the real ATP box; fixed with the dvar_label idiom; template audit confirms no other instance. PROBES: 073 → `hel` ✓, 074 → `2` ✓ (both sbl-matched), four synthetic oracle probes POS/RPOS/TAB/RTAB in-var all sbl-MATCH. GATE MET: smoke 7/7 · pat-rung 19/19 · corpus **154**/280 (floor 151, +3) · beauty 1/17 · fence HARD.
- [x] **B5 UNARY_S protos** ✅ — ANY/NOTANY/SPAN/BREAK/BREAKX via ONE kind-discriminated template bb_pattern_unary_s.cpp (precise parallel of B4's unary_i; driver sets op_kind string; five IR kinds distinct/dump-readable). Universal slot layout HELD — +0 cset_ptr (NUL-term strtab string, filled like bb_pattern_lit's lit-ptr) · +8 saved-δ scratch (BREAKX β-restore) · +16 out_γ · +24 out_ω · β@+32 — B6's `base = ω_site−24, β = ω_site+8` stays valid. Operand load uses `ins2 mov rax, qword ptr [rip + g_pat_pool_cur]` (B3 finding: `[rip+__]` emits lea, wrong for a value load). Matching = inline call-free membership scan (no strchr, per call-free-proto rule), leaving +8 free. α semantics per SPITBOL ch.18/19: ANY δ<Δ ∧ subj[δ]∈cset → δ++,γ else ω; NOTANY δ<Δ ∧ subj[δ]∉cset → δ++,γ else ω; SPAN run of ≥1 cset chars → δ=end,γ, 0 matched → ω; BREAK run of non-cset chars, stop before first cset char → δ=stop,γ, reach Δ with no break char → **fail (ω)**; BREAKX like BREAK on α, save stop→+8. β one-shot → out_ω for ANY/NOTANY/SPAN/BREAK; **BREAKX β re-generates** (restore +8, skip past break char, rescan to next; ω only when no further break char). Entire proto body in ins1/ins2 passthrough (mode-4 TEXT-only) — sidesteps the B2 silent-drop trap on `[r13+rax]`/`byte ptr` shapes. PROBES (builder route, named-var subject `S ? P` since literal-subject + non-lit-pattern is the rt_scan BOMB = S1's job): SPAN('ab')/aabbcc→`aabb` · ANY('abc')/cab→`c` · NOTANY('ab')/Xab→`X` · BREAK('X')/abcXdef→`abc` · BREAKX('X')/abcXdef→`abc` — all sbl-MATCH; fail-path probes BREAK-no-char/SPAN-zero/ANY-no-char all → `failed` sbl-MATCH. ⛔ DEFERRED (NOT B5 regressions, confirmed isolated): capture `. X` inside built DT_P corrupts the BREAK fail-path (B8 territory — capture-in-built-pattern unbuilt); a literal subsequent inside the built pattern (`BREAKX('X') . A 'Xc' . B`) needs STITCH-CAT (B6) before BREAKX β-regen can be exercised in the builder route. GATE MET: smoke 7/7 · pat-rung 19/19 no-SKIP · corpus **155**/280 (floor 154, +1) · beauty 1/17 · fence HARD.
- [x] **B6 STITCH-CAT** ✅ — nearest-left-resume β wiring (`P = 'a' LEN(2)`). **VERIFIED 2026-06-08 (Opus 4.8):** bb_pattern_cat.cpp lands as a PURE POINTER-PATCH stitch (NO join-thunk copy, NO g_pat_pool bump — CAT needs no shared success funnel, unlike ALT): `*left.γ_site=right.entry` · `*right.ω_site=left.β` (left.β = left.ω_site_value `+8` via `lea rcx,[rcx+8]`) · out frag {left.entry, right.γ_site, left.ω_site}. **AS-BUILT DELTAS vs recipe below:** (a) recipe (4) join-thunk proto DROPPED — CAT is patches-only; (b) offsets are the DTP_FRAG_t ζ layout {entry@+0, γ_site@+8, ω_site@+16}, NOT the proto-internal out_γ@+16/out_ω@+24 the recipe quoted (recipe conflated frag-slot vs blob-slot); (c) IR_PATTERN_CAT kind already existed (B2 taxonomy) — no IR.h change; (d) **sno_pattern_buildable SEQ/CAT must gate on `has_pat` (≥1 genuine pattern element via new recursive `sno_seq_is_pattern` helper), NOT unconditional `return 1`** — else all-string concat `OUTPUT='ab' 'cd'` misroutes to the builder → yields `PATTERN` (smoke `concat` regression caught 7/7→6/7; fixed → all-literal SEQ falls through to legacy IR_ASSIGN_CONCAT). lower_pattern_build TT_SEQ/TT_CAT arm = left-assoc pairwise IR_PATTERN_CAT chain (exact mirror of the TT_ALT arm, sequential). Touchpoints: bb_pattern_cat.cpp(new) · bb_templates.h · emit_core.c dispatch · emit_bb.c FILL+descr_chain_arity=2 · lower_sno.c(buildable+helper+build arm) · Makefile(RT_PIC_SRCS+per-obj rule). PROBES (named-var subject, builder route): 4 oracle targets all sbl-MATCH (`'a' LEN(2)`→abc · `LEN(1) 'b'`→ab · `SPAN('a') 'b'`→aaab · `'x' LEN(2)`→fail); 9-probe extended battery all sbl-MATCH incl **BREAKX β-regen** (`BREAKX('XY') 'Y'`/aXbYc→aXbY — the case B6 unblocks), **ALT-as-CAT-operand** (`('a'|'x') LEN(1)`→ab, nested stitch), 3-elem one-shot chain, 3-elem backtrack-to-fail, SPAN-no-giveback-fail, unanchored; string/pattern boundary re-checked (`P='ab' 'cd'`→string abcd · `'x' 'y' LEN(1)`→pattern xyz). **GATE MET: smoke 7/7 · pat-rung 19/19 no-SKIP · corpus 155/280 (floor 155, held) · beauty 1/17 · fence HARD.** ⛔ **B9 NOTE (β-derivation limit):** `left.β = left.ω_site+8` yields the LEFTMOST element's β for a multi-element left operand, so the failure path SKIPS middle elements' β (their δ-restore). Observably correct for one-shot chains of ANY length (all reach ω regardless; unanchored scanner resets δ on retry — intermediate δ on a doomed path is unobservable) AND for 2-element regenerating cases (BREAKX/ARBNO at the left of a pair regenerates fine). LATENT only for a 3+ chain with a NON-leftmost regenerating element (e.g. `LEN(1) BREAKX(..) 'x'`): the immediate-left-neighbor resume isn't reached. Fix when B9 lands regenerating-element stitching — add a 4th β slot to DTP_FRAG_t (β = rightmost element's resume, decoupled from ω_site) so each pair patches `*right.ω=left.β_field` = true nearest-left. **FULL RECIPE (derived 2026-06-08 from the proven `wire_seq` lower.c:54-84 + the B3 `bb_pattern_alt.cpp` 59-line stitch precedent; oracle targets probed):**
  ORACLE TARGETS (`sbl -b`, named-var subject — builder route needs a var subject, literal-subject still S1-BOMB): `'a' LEN(2)`/abcd→`abc` · `LEN(1) 'b'`/abcd→`ab` · `SPAN('a') 'b'`/aaab→`aaab` · `'x' LEN(2)`/abcd→**fail**. (`'a' 'b' 'c'` already PASSES — adjacent literals fold to one IR_PATTERN_LIT pre-stitch.) GAP today: any SEQ/CAT with a non-literal builder fragment yields `[]` (TT_SEQ/TT_CAT is absent from `lower_pattern_build` + `sno_pattern_buildable`, so it falls to legacy and never builds).
  PORT EQUATIONS (mirror `wire_seq` EXACTLY, one layer down): build fragments RIGHT-TO-LEFT; `entry[i].γ = (i+1<nk) ? entry[i+1] : <terminal>` (forward success), `entry[i]=αi?αi:c`, `resume[i]=βi`; then for i=1..nk-1: `apply[i].ω = nearest-left non-trivial resume[j]` (scan j=i-1..0, skip resume==ω_in, else ω_in); CAT α_out=`entry[0]`, CAT β_out=`resume[nk-1]` (RIGHTMOST resume). One-shot frags (LIT/LEN/SPAN/…) have resume→ω; BREAKX/ARB/ARBNO resume re-generates — the nearest-left scan is what makes `'a' SPAN(..)` give 'a' back correctly when it can.
  FILES (~6-8, ~150 lines):
  (1) `sno_pattern_buildable` (lower_sno.c:806) — add `if (e->t==TT_SEQ||e->t==TT_CAT){ flatten; nk≥1; all kids buildable; return 1; }`.
  (2) `lower_pattern_build` (lower_sno.c:821) — add `case TT_SEQ: case TT_CAT:` mirroring the TT_ALT arm (842-859) BUT sequential: recurse via `lower_pattern_build` per kid (NOT lower_program), chain BUILD order `prev->γ=kα`, allocate ONE `IR_PATTERN_CAT` stitch box per adjacent pair (operands = the two frags, like ALT pushes acc+k) carrying the nearest-left-resume target, set its build-order γ, return `ret(prev, …, entry[0], resume[nk-1])`.
  (3) `IR.h` — new kind `IR_PATTERN_CAT` adjacent to `IR_PATTERN_ALT` (end-of-SNO block); add to scrip_ir names.
  (4) `bb_pattern_cat.cpp` (~60 lines, mirror `bb_pattern_alt.cpp`) — read `_.op_sa` (left frag slot), `_.op_sb` (right frag slot), `_.op_off`; copy the same `.quad 0`+`jmp qword ptr [rip+slot]` join-thunk proto into g_pat_pool; patch RIGHT.head → LEFT.out_γ (`[r12+op_sa+16]`), and LEFT.resume(thunk) → RIGHT.out_ω (`[r12+op_sb+24]`). Slot offsets per universal layout: out_γ@+16, out_ω@+24. ins1/ins2 passthrough, MEDIUM_TEXT label/comment only, NO MEDIUM_BINARY arm (FACT rules).
  (5) `emit_core.c` — dispatch `case IR_PATTERN_CAT: bb_pattern_cat(); return 0;` (append in SNOBOL IR_PATTERN_* block).
  (6) `emit_bb.c` — FILL + `op_sa`/`op_sb`/`op_off` setup for IR_PATTERN_CAT (mirror IR_PATTERN_ALT's operand→slot resolution); `descr_chain_arity` → 2.
  (7) `bb_templates.h` — `void bb_pattern_cat(void);`. (8) `Makefile` — add `bb_pattern_cat.cpp` to RT_PIC_SRCS (own line, end of SNOBOL group).
  VERIFY: build → `/tmp/b6probe.sh` (5 targets above) all sbl-match → full m4 battery (smoke 7/7 · pat-rung 19/19 · corpus ≥155 non-decreasing · beauty 1/17 · fence HARD) → rebase onto live origin/main (re-fetch first; verify disjoint file sets), FF-only land, NEVER --force main. UNBLOCKS: BREAKX β-regen probe + (with B8) capture-in-built-pattern.
- [ ] **B7 NULLARY protos** — ARB/REM/BAL/FENCE/FAIL/SUCCEED/ABORT (replaces the B0 NULL-head placeholders). **FULL RECIPE (derived 2026-06-08 from bb_pattern_unary_i.cpp skeleton + B2 universal layout; split by mechanism so the trivial set lands first):**
  PROTO SKELETON (identical to unary_i `pu_proto`): blob = four `.quad 0` (+0 op1 · +8 op2/saved-δ scratch · **+16 out_γ abs64 patch** · **+24 out_ω abs64 patch**) · `_b:` β-entry (+32, = ω_site+8, UNIVERSAL) · `_a:` α-entry · `_f:` `jmp qword ptr [rip+_s+24]` · `_e:`. Builder body = unary_i's EXACTLY but **DROP the operand-fill** (`mov ecx,<n>; mov [rax],ecx`) since nullary: copy blob (rep movsb, bump g_pat_pool_cur), then frag stores `lea rcx,[rax+_a-_s]→[r12+off]` (entry) · `lea rcx,[rax+16]→[r12+off+8]` (γ_site) · `lea rcx,[rax+24]→[r12+off+16]` (ω_site). All α/β bodies in ins1/ins2 passthrough (mode-4 TEXT), MEDIUM_TEXT label/comment head only.
  **B7a — TRIVIAL ONE-SHOTS (zero new mechanism; do FIRST):** FAIL · REM · SUCCEED.
    FAIL    α: `jmp qword ptr [rip+_s+24]` (always→out_ω). β: same. (no scratch)
    REM     α: `mov [rip+_s+8],r14d` (save δ); `mov r14d,r15d` (δ=Δ); `jmp qword ptr [rip+_s+16]` (→out_γ). β: `mov r14d,[rip+_s+8]` (restore δ); `jmp qword ptr [rip+_s+24]` (→out_ω). (one-shot)
    SUCCEED α: `jmp qword ptr [rip+_s+16]` (match empty→out_γ). β: `jmp qword ptr [rip+_s+16]` (regenerate empty FOREVER — true SPITBOL semantics; bare SUCCEED with a failing tail hangs sbl too, so don't probe that shape). (no scratch)
  **B7b — REGENERATING + ABORT (needs care; separate):** ARB · FENCE · ABORT.
    ARB (regenerating, first builder-side regen leaf): α: `mov [rip+_s+8],r14d` (save start); `jmp [rip+_s+16]` (match 0). β: `mov eax,r14d; inc eax; cmp eax,r15d; jg .ovf; mov r14d,eax; jmp [rip+_s+16]` (extend +1→out_γ); `.ovf: mov r14d,[rip+_s+8]; jmp [rip+_s+24]` (restore+out_ω). Probe ARB-alone + `ARB 'x'` carefully.
    FENCE/ABORT need a DIRECT line to the head ω that BYPASSES the resume chain (FENCE α matches empty→out_γ, β→abort whole match; ABORT α→abort immediately). In the patched-pointer model out_ω is patched to the nearest-left RESUME (CAT stitch) — so β→out_ω wrongly RETRIES the left neighbor instead of aborting; correct ONLY when leftmost (out_ω=head ω there). FIX (same family as the B6 β-decoupling + B9): add a 2nd head trampoline in bb_dtp_assign (`_wabort: jmp [rip+_s+ABORT_SLOT]`) whose addr is threaded into FENCE/ABORT protos at stitch/assign time (or a runtime abort flag the driver checks); co-design with B9 ARBNO abort.
  **BAL — SEPARATE SUB-RUNG:** balanced-paren scan ( () nesting ), regenerating; needs a depth counter (use +8 scratch for depth, +0 for start) + per-char ( )-balance scan in the α; defer.
  ⛔⛔ **CRITICAL FINDING (2026-06-08 Opus 4.8, probed live — INVALIDATES the token-based routing below):** bare nullary keywords `REM`/`FAIL`/`SUCCEED`/`ARB`/`BAL`/`FENCE`/`ABORT` parse as **`TT_VAR`** (snobol4.y:196/212: bare `T_IDENT`→TT_VAR with sval=name), NOT their dedicated `TT_xxx` tokens. The `TT_REM`/`TT_FAIL`/etc. tokens arise ONLY from the function-call form via `tal_fnc_close` (snobol4.y:29-32, kw_kind table) — i.e. `REM(...)`, which is invalid syntax for an argument-free primitive. CONSEQUENCE: a `case TT_FAIL/TT_REM/TT_SUCCEED` arm in lower_pattern_build is DEAD CODE; `P = REM` routes to the legacy assign path as a variable reference (asm-confirmed: no PATTERN_REM box, no DTP_ASSIGN emitted; legacy mode-4 then yields wrong results — REM→empty, FAIL→no-fail). **CORRECTED ROUTING for B7a:** recognize the predefined-pattern NAMES on a TT_VAR node. (5-fixed) `sno_pattern_buildable`: `if (e->t==TT_VAR && e->v.sval && (!strcmp(e->v.sval,"REM")||!strcmp(..,"FAIL")||!strcmp(..,"SUCCEED"))) return 1;`. `lower_pattern_build`: add `case TT_VAR:` arm that name-switches sval → `nalloc(IR_PATTERN_REM/FAIL/SUCCEED)` + emit_leaf (builder uses IR_PATTERN_*, NOT legacy IR_PAT_* of the old `lower_pattern` arms ~919-922). `sno_seq_is_pattern`: name-check TT_VAR sval (so `'a' REM` flags has_pat) — NOT TT_FAIL/TT_SUCCEED tokens. ⚠️ RISK to clear before landing: a TT_VAR name match could mis-route a user variable literally named REM/FAIL/etc.; in SNOBOL4 these are reserved predefined patterns so it's defensible, but (a) confirm no passing corpus test assigns from a same-named user var, (b) bare `S ? REM` (scan-context, NOT assignment) goes through a DIFFERENT lowering path — sno_pattern_buildable is only the assignment gate (lower_sno.c:1114) — so scan-context nullary refs need separate handling or stay legacy. ALTERNATIVE (cleaner long-term): runtime-init predefines REM/ARB/… gvars to natively-built DT_P at startup, so every reference (assign OR scan) just fetches the built pattern. The proto α/β below are correct regardless of routing.
  WIRING TOUCHPOINTS (B7a; mirror B4/B5/B6 — **routing per CORRECTED FINDING above, not the stale (5)**): (1) `bb_pattern_nullary.cpp` (new, ONE op_kind-discriminated template FAIL/REM/SUCCEED; α/β exactly as specified above; mirror bb_pattern_unary_i sans operand-fill — built clean once, verified to assemble). (2) `bb_templates.h` decl. (3) `emit_core.c` — replace the FAIL/REM/SUCCEED stubs (~431-434) with `bb_pattern_nullary()`; leave ARB/BAL/FENCE/ABORT stubbed. (4) `emit_bb.c` — FILL cases ×3 (`g_emit.op_kind="FAIL"/"REM"/"SUCCEED"; op_off=bb_slot_alloc24(nd); FILL(...)`), `descr_chain_arity` → 0 ×3. (5) `lower_sno.c` — **TT_VAR-name routing per the CORRECTED FINDING** (the token-based `case TT_FAIL/TT_REM/TT_SUCCEED` is DEAD — do not use). (6) `Makefile` RT_PIC_SRCS + per-obj rule (mirror bb_pattern_cat). VERIFY: build → probe `P=REM`/abcd→`abcd` · `P='a' REM`/abcd→`abcd` · `P=FAIL`/abc→fail · `P='a' FAIL`/abc→fail · `P=SUCCEED 'b'`/ab→`b` all sbl-MATCH → full m4 battery (smoke 7/7 · pat-rung 19/19 · corpus ≥155 non-decreasing · beauty 1/17 · fence HARD) → commit; do NOT push without "perform hand off". (Infra steps 1-4,6 were trial-built clean this session and reverted with the dead token-routing; recreate from this recipe — they are correct, only routing (5) changed.)
- [ ] **B7-legacy-note** — original one-line scope retained for reference: ARB/REM/BAL/FENCE/FAIL/SUCCEED/ABORT replaces the B0 NULL-head placeholders.
- [ ] **B8 CAPTURE in built patterns** — `.`/`$` inside DT_P; D3 commit ring (061/064).
- [ ] **B9 ARBNO + FENCE(P)** — structural unary_p, child fragment embedded (070/075, 059-067). ⛔ CARRIES B6 NOTE: add 4th β slot to DTP_FRAG_t (β = rightmost-resume, decoupled from ω_site) so CAT's nearest-left wiring is correct for 3+ chains with a non-leftmost regenerating element; also carries B4's per-instance +8-scratch landmine (ARBNO multi-reentry needs per-activation save).
- [ ] **B10 DEFER-IN-BUILD** — *V/*E slots re-fetched at match.
- [ ] **B11 SEAL-FLIP + COLLECT** — W^X per-segment flip at match (seal-on-match physical); partial reclamation.
- [ ] **B-CONV** — retire defer thunk + shim arms into native statement-level match driver (merges into S6).
