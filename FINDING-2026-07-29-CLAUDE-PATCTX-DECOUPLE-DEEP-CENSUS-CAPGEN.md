# FINDING 2026-07-29 (Claude) — PATCTX: HEAD/RELEASE decoupled; deep-arrival census completed; capture generation split

DIRECTIVE (Lon): decouple IR_MATCH_HEAD ↔ IR_MATCH_RELEASE — HEAD must save R13/R14/R15 and
set them anew; RELEASE reverses.  Design questions (rbp frame vs marker; R12 frames vs markers;
a second capture register) answered in-session; synthesis at bottom.

BASELINE (SCRIP 57a7b598): crosscheck m3 277/38 · m4 275/38 (SKIP 2) · DIVERGE 3
MID     (PATCTX+census):   crosscheck m3 304/11 · m4 303/10 (SKIP 2) · DIVERGE 3
FINAL   (this tree, +PIN-ALL): m3 313/2 · m4 311/2 (SKIP 2) · DIVERGE 0
Survivors: test_case (icase matching, unrelated family) and 160_pat_alt_inner_gen_resume
(V=[] vs V=[X]: ALT inner-generator resume drops a capture -- next-session target).
27 healed m3 / 28 m4, ZERO new failures; final fail sets are strict subsets of baseline.
Healed: the calc/json/regex/fence-capture families (120–127, 131, 143, 145–147, 152),
alt-commit 053, fence-fn-nested 066, recursive-star lists 178/179, W02×2/W03, cross,
expr_eval, test_string, word1–4, wordcount — and the s197 "deferred-argument family"
(defer-of-call SEGV both modes) which turns out to be the same depth-alias class, not a
separate bug.  DIVERGE rotated: 124 healed outright; 140 now m3-only.

## Fix 1 — PATCTX (bb_match_head/bb_match_release/zeta_storage/gen_runtime)
HEAD's zls grant widened 3→5 quads: +48 sigma_save, +56 delta_save, +64 Delta_save,
+72 capgen_save (former pad — zero downstream offset ripple beyond the two new quads).
head α saves r13/r14/r15 + g_cap_gen before rt_match_enter installs the new subject; BOTH
exits — head's ω-choke on failure, release_pump's tail on success (after the pump, which
needs the INNER Σ for rt_dcap_end_ok_open and may itself nest) — restore all three registers
and call rt_match_ctx_restore(Σ, Δ, gen), re-syncing the C-side Σ/Σlen mirror (live readers:
pattern_match.c, runtime_eval.c) and the capture generation.  FRQ validity at each site:
α is pre-carve, both exits are post-unwind == α depth in every port/frame arm; the hfc live-cell
window is hand-addressed by the existing template and never straddled by the new accesses.

## Autopsy 1 — the depth pun (found via a 1-test regression, 125_pat_json_literal)
The sealed non-star defer's γ-continuation restore is spelled FRQ(op_off) — same spelling as
its α stamp — but PAT$γ SUSPENDS (push rbp; push res) and re-enters the continuation with rsp
16+blobframe LOW.  The s137 comment conditions that spelling on rbp ("FRQ(op_off), rbp-relative
→ recursion-safe"); in the RSP regime it only ever worked by SLOT-VALUE ALIASING: [rsp+mark] at
residue depth landed on head.zls2_mark, which holds the same rsp value.  The PATCTX widening
moved the alias target onto head.sigma_save — outer r13 == 0 at the outermost match — so
rsp := 0 and the next call faulted.  The alias was load-bearing across a large slice of the
corpus; that is why completing the census healed 27 programs and the defer-of-call family.

## Fix 2 — deep-arrival census completed (emit.cpp, emit_graph_has_deep_arrival)
IR_MATCH_DEFER's sno_defer_is_star gate DELETED: defer is unconditionally deep, same as
IR_MATCH_VALUE, per the census's own prose ("DEFER/VALUE σ-transfer into γ-retaining PAT$
blobs, post-transfer depth dynamic") and its own safety rule ("the conservatism lives in the
LIST being complete").  Star was never the load-bearing property; γ-retention is, and every
defer target can retain.

## Autopsy 2 — nest1 gave R=ZZQX (oracle: QX) with M=B correct
g_cap_gen is a single per-match counter stamped by rt_match_enter.  With nesting now LIVE,
the inner match's stamp invalidated the OUTER match's open SAVE brackets: rt_cap_pop no-ops on
stale gen, rt_cap_top returns 0, the COND captured [0,end).  The generation id is pattern
context.

## Fix 3 — PATCTX-2: well/current split (pattern_match.c)
g_cap_gen (now exported; read at head α via [rip+g_cap_gen], both media) is the CURRENT id;
g_cap_gen_next is the monotonic WELL.  rt_cap_match_begin draws fresh from the well; exits
restore the SAVED id — never a decrement, because a restored-then-re-bumped counter would
re-issue the inner match's retired stamp and zombie its success-exited frames.  Retired ids
never re-issue (same 2^32 wrap exposure as before), so the lazy-kill invariant — stale gen ⟹
dead frames — survives nesting.

## Autopsy 3 — the FLATDISP misread (runtime-arg primitives, the fence_fn trio and kin)
Minimal reproducer f3.sno: `digits='0123456789'; X='123abc'; X SPAN(digits) . N` fails while
the literal-arg twin passes.  gdb absolute addresses: the pre-chain coerce writes a perfect
10-char descriptor at its op_off (drive-level slots AGREE: coerce op_off == span op_sa == 208,
proven by drive diagnostics -- every allocator/duplicate theory is dead), but the span's
emitted FRQ(op_sa+8) read carries a baked op_flat_disp of 128 against 32 bytes actually live
at its α, landing 72 bytes above the value on a virgin slot.  The baked prefix counts cells
that popped before the reader (value cells consumed at γ vs the S10c pop-at-ω accounting).
This is the FLATDISP family with a lab-grade witness, and it is regime-scoped: rbp-relative
FRQ is depth-immune, so the class exists only where the deep-arrival census exempted a graph
into the RSP regime.

## Fix 4 — PIN-ALL (emit_graph_has_deep_arrival returns 1; census preserved beneath)
The pin-all experiment healed f3, the fence_fn trio, 153, 154, 061, 047's SEGV, the 140/141
diverging SEGVs, and W06 -- full suite 313/2 · 311/2 · DIVERGE 0, zero regressions, witnesses
oracle-exact both media, regen clean.  Landed as the predicate's first line with the census
retained as the documented re-earn path: re-narrowing is deleting one line, licensed only by
an fc-prefix audit that makes baked disp equal live depth at every FRQ reader.  FORTH cells
still carve and pop identically under the pin -- only the fragile rsp-relative addressing goes
dormant -- so the ZB-FC investment is untouched.  Aligned with REG-7 U4's unconditional-rbp
frame and the rbp-pin-main rung; if Lon rules for the RSP regime's return, the revert is one
line and this FINDING holds both watermarks.

## Fix 5 -- FLATDISP-9: the double-add root of the whole disease (x86_asm.h FR/FRQ/FRQB)
Under an unpinned graph, FR/FRQ/FRQB pre-added x86_frame_off (off + op_flat_disp) into the
operand STRING, and x86_parse re-derives the offset so the frame encoders (x86_frame_modrm /
x86_frame_text_mem -- the line-363 single-owner) added the compensation AGAIN: every flat ref
carried 2x disp.  Per-box store/load pairs self-cancelled, which is how it stayed latent for
the whole rsp era; any cross-depth writer/reader pair (coerce at disp 0 writing flat 208, span
at disp 64 reading 216+64+64=344) read a foreign slot.  Fix: the three macros carry the RAW
offset; the encoders are the one adder.  With that, PIN-ALL was retired per Lon's pivot (rbp
only for the intended frame constructs; RSP zeta is the direction): the census-based regime is
re-armed, f3 / the fence_fn trio / 153 / 154 / 047 / 061 / W06 all pass ON RSP -- eight
programs that previously required the pin.  Residual vs pin-all: 140/141 SEGV m3-only again
(the EVAL-minted in-process graph family, next target); re-enabling the pin is one `return 1;`
in emit_graph_has_deep_arrival, both watermarks recorded here.

## Template literal-RBP census after the pivot (Lon eradication directive)
Every remaining literal "rbp" in templates is on the intended list: xa_flat (STATEMENT/
FUNCTION seed + heap-frame adopt), bb_save_restore (FUNCTION wire frame), bb_match_fence1
(floor + watermark), bb_match_arbno (zv borrow), head/release/replace +40 brackets (defend the
borrow), bb_create (Icon).  ZERO unintended literal refs found; the 19,341 eradicated
references were pin-era FR/FRQ addressing.  The 16,237 residual generated refs belong to
census-pinned deep-class graphs (defer/fence/arbno/gen statements, blobs, procs) -- retiring
those is the remaining zeta work: extend FORTH cells to all LOCALS and RESULTS and give the
deep classes depth-immunity without the pin (the ? statement frame rung), eliminating flat
reach-over slots entirely per Lon: "we are not trying to reach over."

## Lon's SEQ-delta idea (banked, 2026-07-29)
Force IR_SEQ for ALL sequences; BB_SEQUENCE measures its rsp delta and publishes it as its
RESULT (size of the sequence's zeta data now), updated on every re-entry; ALT/ARBNO/FENCE take
the length as an operand.  Not needed for addressing once reach-over is gone, but on FINAL
SUCCESS the lengths could drive precise frees.  Recorded for the free-on-success rung.

## Witnesses (all sbl-oracled; /home/claude/w/, worth promoting to the corpus)
nest1.sno   — P = *F() 'X' where F's body runs its own match: SEGV at baseline → R=QX / M=B,
              oracle-exact, BOTH media.  The full nesting contract in one program.
defonly.sno — defer-of-call, no inner match: SEGV at baseline (s197 family) → R=QX oracle-exact.
pump1.sno   — name-position conditional assignment '. *F()': SCRIP SUCCEEDS (A=Z V=ZQ M=B B=X),
              SPITBOL MATCHFAILs.  NEW unresolved divergence probe; manual Ch.6/7 name-expression
              semantics study next session before touching anything.

## Residuals, named not hidden
- pat_seal bypass: ABORT/FENCE-seal kills jump straight to fJ, skipping head's ω-choke — zls
  unwind, CAS pop, AND the PATCTX restore.  Pre-existing leak class; closes under the rbp
  match-frame rung (a wild jump can skip inline cleanup, not a frame).
- Remaining fails (all pre-baseline): 061_capture_in_arbno, test_case, 047_pat_rtab,
  063/064/065 fence_fn, 140/141 eval_double_fn (140 m3-only), 153, 154, 160, W06_tab (m4).
- Design synthesis delivered to Lon in-session: the ? statement rbp frame (STATEMENT in the
  s193 four-construct ruling taken literally — head is the prologue, release the epilogue)
  subsumes the marker questions: Σ/δ/Δ/gen saves, the capture watermark, and the fc pre-push
  rsp all become [rbp_m−k] slots; ω unwind is one lea; R11 is caller-saved/PLT-clobbered and
  structurally unavailable, but the frame needs no new register.  The +48..+72 slots and the
  census completion are forward-compatible steps toward that rung; +40's own comment already
  anticipates re-tenancy as saved-outer-rbp.
- Regen: benchmark/feature/demo .s regen scripts run clean; zero committed .s drift.
