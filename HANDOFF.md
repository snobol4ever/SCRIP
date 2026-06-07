# HANDOFF — 10th run (Claude Sonnet 4.6, 2026-06-06)

## Repo state
- Branch: `main`, HEAD: `a288fda`
- Build: CLEAN (`make scrip` → `Built: scrip`)
- All standing gates: GREEN
  - smoke PASS=7 FAIL=0
  - pat rung PASS-M4=18 FAIL=0 SKIP=1 (053 known)
  - bb_bin_t abolished
  - vstack refs 3 (target 0, no regression)
  - purity 2 known side-effects (bb_call_write_slot, bb_every — unchanged)

## Rank
- Session open:  80 dirty / 24 clean, 1247 violations
- Session close: 70 dirty / 34 clean, 1145 violations
- Net: +10 clean, −102 violations

## Cursor
**`bb_disj.cpp`** — not yet audited this session; pull first, then `bash scripts/audit_bb_fixup_file.sh src/emitter/BB_templates/bb_disj.cpp`

## Stops completed this session (stops 20–33)

| Stop | File | Gains | Note |
|------|------|-------|------|
| 20 | bb_callee_frame.cpp | pe 4→0, lv 3→0 | ✅ CLEAN |
| 21 | bb_catch.cpp | ef 6→0, pe 3→0, lv 3→0 | ✅ CLEAN |
| 22 | bb_cell_call.cpp | pe 3→0, lv 3→0 | ✅ CLEAN |
| 23 | bb_cell_choice.cpp | pe 3→0, helpers | [S] lv=4 mutable-accum residue |
| 24 | bb_cell_cut.cpp | pe 3→0 | ✅ CLEAN |
| 25 | bb_cell_ite.cpp | pe 1→0, lv 1→0 | ✅ CLEAN |
| 26 | bb_cell_unify.cpp | pe 10→0, lv 7→0 | ✅ CLEAN |
| 27 | bb_choice.cpp | ef 5→0, lv 2→0 | [S] lv=1 loop-accum |
| 28 | bb_conj.cpp | ef 2→0 | [S] rb=1 GAP-5, lv=1 loop-accum |
| 29 | bb_cut.cpp | pe 3→0 | ✅ CLEAN |
| 30 | bb_det_cmp.cpp | pe 7→0, lv 6→0 | ✅ CLEAN |
| 31 | bb_det_is.cpp | pe 12→0, lv 1→0 | [S] lv=5 out-param + recursive locals |
| 32 | bb_det_nl.cpp | pe 3→0 | ✅ CLEAN |
| 33 | bb_det_write.cpp | pe 9→0 | ✅ CLEAN |

## [S] flag legend
`[S]` = structurally necessary residue — not a skip, means the violation
requires a design decision (GAP-5 E9 reloc, mutable loop accumulators, recursive
out-param helpers). Next session should review these before revisiting.

## Session techniques confirmed
- `FOR()` lambda replaces loops in `_str`
- Accessor helpers (`bXX_*()`) pull all local typed vars out of `_str`
- `dcm_both_lit_i()` + `dcm_fold_result()` pattern removes `long av, bv` out-param pair
- `bcch_build()` forward-declared before `_str` to hold mutable accumulator
- All PORT_ALPHA/BETA/GAMMA/OMEGA → literal Greek strings `"α"` `"β"` `"γ"` `"ω"`
- emit_fmt() → `std::string(...) + std::to_string(...)` or direct string concat

## Next session setup
```
cd /home/claude
git clone https://ghp_TOKEN@github.com/snobol4ever/SCRIP.git SCRIP
cd SCRIP
bash scripts/install_system_packages.sh
bash scripts/build_scrip.sh
make libscrip_rt
bash scripts/test_smoke_snobol4.sh          # must be PASS=7 FAIL=0
bash scripts/audit_bb_fixup_rank.sh         # print rank table
bash scripts/audit_bb_fixup_file.sh src/emitter/BB_templates/bb_disj.cpp
```
Then fix bb_disj.cpp per v2 rules and loop.
