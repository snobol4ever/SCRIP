# HANDOFF 2026-06-07-H — Opus 4.8 — IR-REDESIGN: IRD-3 bulk (c) ring cluster + SEQ orphan

**Goal:** GOAL-IR-REDESIGN.md (single source of truth — see its watermark for live state).
**SCRIP origin at close:** 6c12e26 (+ this doc's commit). **Both trees CLEAN, all pushed.**

## Commits this session (each fully gated)

- **8af31d1 — RING CONSUMERS.** Interp arms for every icn_ring_to_tree-served kind →
  operands-first dual-read: BINOP_GEN/TO/TO_BY via hoisted `ir_pair_arg` Lc/Hc(Rc)
  locals (ring-mode discriminators included); UNOP/NEG/POS/NONNULL/NOT/SIZE via
  `c0 = n_operands>0 ? operands[0] : α`. EVERY already dual since IRD-3e. Emitter
  side pre-verified dual BEFORE edits: flat_drive_* via bb_child0/1, bb_every.cpp
  via ir_call_arg, descr_chain_operand_refs already operands (SCAN-exempt),
  bb_walk_rec operands-aware, ir_is_single_shot default arm walks operands.
  m2-safety proof: lowering census shows these kinds carry children via
  γ-chain/operand_aux/operands — α/β NULL in m2, so operands-first is
  NULL-for-NULL identical. LAW check: zero relative γ-hops on fetched locals.

- **21add4c — RING WRITER FLIP.** icn_ring_to_tree ar==2/ar==1 arms (scrip.c
  88–92) α/β → operands[0..1] via ir_operand_push; order [0]=left(α) [1]=right(β)
  matches ir_pair_arg and emitter descr_chain_operand_refs. Function now carries
  ZERO α/β writes. Proofs: (1) gdb LIVE-PATH — pre-flip bp :92 fires on
  `write(*&subject)` ring-live probe; post-flip ir_operand_push frame#1 =
  icn_ring_to_tree:94 same probe. (2) stash A/B behavior parity ×2 probes
  (SIZE+KEYWORD, NONNULL+KEYWORD) — byte-identical output+rc; both hit the
  PRE-EXISTING GROUND-ZERO-3 rt_call_builtin abort downstream (rc=134 parity;
  that box rebuild is B-ladder scope). (3) static dump A/B EMPTY-class per
  faa9b52 precedent — ring conversion runs in the m3/m4 driver AFTER --dump-bb.

- **6c12e26 — SEQ α-ORPHAN DELETIONS** (verify-then-delete, faa9b52 class).
  SEQ.α has ZERO writers: broadened census (arrow + dot + deref + memcpy write
  shapes; only the audit tool's dot-writes exist, already flagged) + builder
  proof (wire_seq γ-chains children, returns entry[0] via ret(), never stores
  node->α). Deleted: flat_drive_seq dead BFS body (~45 lines; guard pair-jmp
  path IS the function, still serves IR_SEQ and IR_SEQ_EXPR dispatch) + orphan
  seq_node_label; interp IR_SEQ raku-SUSPEND arm (guard needs non-NULL α, never
  fires); tail `return bb->α` folded to `return NULL`. dval==1.0 concat arm has
  no α dependence and STAYS. Proof: bake byte-identity (A/B-EMPTY standard).

## Gates per commit
Full bake `scripts/bake_ird3_baseline.sh` diffed vs pristine /tmp/base_pre:
ONLY prove_lower col-7 pointer-punned ivals (LAW 6), masked-identical, PASS=68;
all 5 sweeps (sno 153/icn 9/pl 8/pas 5/sco 191) + 5 smokes byte-identical;
icon m2 12/12 HARD, m3/m4 10/2 floors.

## Race absorbed ×1 (LAW honored)
pull --rebase before push pulled ed5fe6e (Sonnet handoff), 8f4f773 (IRD-2b fix:
gz reader units[j]->β → operands[1]; pl rungs m3 22→29), 0af3eb7 + 66c7bdf
(BB-FIXUP bb_unify/bb_unop). Merged head REBUILT + FULL RE-BAKE — byte-identical
to pristine again. ⚠ gz-synth regime is ACTIVE-CONCURRENT (IRD-2b work touching
the same sites): FRESH census mandatory before any gz edit next session.

## Counts banked
IR_interp.c α/β refs 184 → 105; emit_bb.c → 46; icn_ring_to_tree → 0 writes.

## New findings
- lower_internal.h `iref()` ALREADY writes IR_ref_t{node,"α"/"β"} — the IRD-4
  target carrier exists in lowering today.
- NEW LAW (recorded in goal file): writer censuses MUST include dot-access /
  deref / memcpy shapes, not just `->field =` (audit-tool dot-writes were the
  only extra hits this time; the shape gap was real).
- RESIDUE: interp IR_SEQ_EXPR α-chain reads — same zero-writer class, needs its
  own verify pass. raku_nfa_bb.c 154/158 NFA α writers are OUTSIDE the goal's
  enumerated remaining list — Lon ruling needed (missed cluster vs NFA exemption).

## Next session boilerplate
Clone/pull BOTH repos; authenticate origin with Lon's token
(`git remote set-url origin https://TOKEN@github.com/snobol4ever/<repo>`);
git identity LCherryholmes/lcherryh@yahoo.com per repo;
`apt-get install -y libgc-dev; make; make libscrip_rt` (MANDATORY for m4);
clone canonical refs (proebsting/jcon, gtownsend/icon → refs/, gitignored);
bake `scripts/bake_ird3_baseline.sh /tmp/base_pre` FOREGROUND BEFORE touching
code; `git pull --rebase` both repos before working; a clean rebase is NOT a
gate — re-verify the merged head. Then NEXT per goal file.

**Authors:** Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude
