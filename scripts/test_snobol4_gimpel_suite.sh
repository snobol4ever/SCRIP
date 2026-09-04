#!/usr/bin/env bash
# test_snobol4_gimpel_suite.sh -- dedicated gate for corpus/packages/snobol4/gimpel (144 *_driver.sno
# programs; 145 further NAME.sno library modules exist alongside them but are structurally unscoreable
# by design -- see gimpel/README.md), minted for row
# snobol4-gimpel-aisnobol-dotnet-measured-with-dedicated-gates (hq_T 2026-09-03).
#
# THIN WRAPPER, not a reimplementation: scorecard_snobol4.sh already carries a dedicated, working
# `gimpel` suite row (weight 5, `-name *_driver.sno`, SELFDIR:include lib, 20s timeout) with mature
# oracle-liveness handling (sbl_died: an oracle that exits 0 while printing a fatal-report banner is
# UNSCR, never a silent PASS or a manufactured FAIL) -- reusing it is lower-risk than a parallel
# implementation of the exact same dialect traps scorecard_snobol4.sh's own history already paid for.
# This script only runs it scoped to `gimpel`, reads its results.tsv, and reprints the board in this
# suite's own dedicated-gate line format so the DONE-WHEN of the minting row can find it by name.
#
# Exit: 0 iff m3_fail=0 and m4_fail=0 over the SCORED (non-ORACLE_FAIL) denominator.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SCRIP="$SD/scrip"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snobol4_gimpel_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
# ⛔⭐ THE VENDORED FIXTURES ARE READ-ONLY, SO THE RUN HAPPENS ON A COPY (row snobol4-gimpel-runner-writes-
# asmtemp-into-the-vendored-dir-and-blocks-its-own-score-write, ceo -> hq_T on hq_C's opening gimpel
# measurement; the Icon Arizona precedent is c96cb087d, same class, same day).
# ⛔ THE SIDE EFFECT DISABLED THE VERY WRITE THIS RUN EXISTS TO PRODUCE: ASM_driver.sno opens its own work file
# ASMTEMP, the scorecard runs each program in the program's OWN directory (deliberately -- argv is the bare
# basename so a diagnostic echoing its own name stays comparable to a frozen .ref), so ASMTEMP landed in
# corpus/packages/snobol4/gimpel/ and left the tree DIRTY -- whereupon util_score_row.py correctly refused the
# leaderboard row, because a number measured on a dirty tree describes no tree anyone can check out. A grader
# that writes into what it grades is the same defect as a gate that edits the artifact it measures.
# ⭐ WHY AN OVERLAY RATHER THAN A cwd CHANGE: the scorecard's per-program cwd is load-bearing (above), and this
# runner does not own that instrument -- it is the shared SNOBOL4 board every suite goes through. Copying the
# 1.8M package and pointing CORPUS at an overlay leaves that contract untouched: reads resolve exactly as
# before (`include` is symlinked back to the real tree, and the suite's own lib spec is SELFDIR:include), and
# every write lands on the copy, which dies with the trap above.
CORPUS_REAL="${CORPUS:-$ROOT/corpus}"
mkdir -p "$W/corpus/packages/snobol4"
cp -a "$CORPUS_REAL/packages/snobol4/gimpel" "$W/corpus/packages/snobol4/gimpel" || {
    echo "⛔ REFUSE(rc=2): could not copy the gimpel package to a scratch overlay -- refusing to grade in the vendored dir"; exit 2; }
for d in "$CORPUS_REAL"/*/; do
    b="$(basename "$d")"; [ "$b" = packages ] && continue
    ln -s "$d" "$W/corpus/$b" 2>/dev/null || true
done
CORPUS="$W/corpus" bash "$HERE/scorecard_snobol4.sh" run --suites gimpel --out "$W" > "$W/run.log" 2>&1
rc=$?
TSV="$W/results.tsv"
[ -f "$TSV" ] || { echo "⛔ REFUSE(rc=2): scorecard_snobol4.sh produced no results.tsv (rc=$rc) -- run.log:"; cat "$W/run.log"; exit 2; }
TOTAL=$(awk -F'\t' 'END{print NR}' "$TSV")
# ⛔⭐ A RUN THAT GRADED NOTHING IS UNMEASURED, NEVER A BOARD OF ZEROS (found 2026-09-04 while curing the
# vendored-dir write, hq_T). scorecard_snobol4.sh TRUNCATES results.tsv before it does anything else, so any
# refusal AFTER that point -- and the common one is its own board-contention guard, "another SNOBOL4 board is
# already running on this box", which is a REFUSAL and not a failure -- leaves an EMPTY results.tsv behind.
# This runner then printed `GIMPEL_BOARD total=0 scored=0 ... m3_fail=0 m4_fail=0` and, because the final
# verdict is `[ "$M3F" = 0 ] && [ "$M4F" = 0 ]`, EXITED 0. A perfect green board over an empty population, on a
# run that never started. ⛔ That is the absent-oracle false-green in its purest form: zero failures because
# zero programs, indistinguishable in the exit code from 144 passes.
if [ "$TOTAL" -eq 0 ]; then
    echo "⛔ REFUSE(rc=2): the gimpel run graded ZERO programs -- results.tsv is empty, so this is UNMEASURED, not clean."
    echo "   scorecard_snobol4.sh exited rc=$rc; its own log follows (the usual cause is the board-contention"
    echo "   guard: another SNOBOL4 board is running on this box, which is a refusal to measure under contention,"
    echo "   not a failure -- wait for it and re-run):"
    sed 's/^/     /' "$W/run.log"
    exit 2
fi
UNSCR=$(awk -F'\t' '$3=="ORACLE_FAIL"' "$TSV" | wc -l)
M3P=$(awk -F'\t' '$3!="ORACLE_FAIL" && $3=="PASS"' "$TSV" | wc -l)
M3F=$(awk -F'\t' '$3!="ORACLE_FAIL" && $3!="PASS"' "$TSV" | wc -l)
M4P=$(awk -F'\t' '$3!="ORACLE_FAIL" && $4=="PASS"' "$TSV" | wc -l)
M4F=$(awk -F'\t' '$3!="ORACLE_FAIL" && $4!="PASS"' "$TSV" | wc -l)
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
SCORED=$((TOTAL-UNSCR))
# ⛔⭐ A ZERO DENOMINATOR IS A REFUSAL, NEVER A STEADY STATE (RULES.md, the denominator law) -- AND THIS SCRIPT
# USED TO PRINT IT AS A GREEN BOARD. The verdict below is `[ "$M3F" = 0 ] && [ "$M4F" = 0 ]`, which is
# VACUOUSLY TRUE over an empty results.tsv: `GIMPEL_BOARD total=0 ... m3_fail=0 m4_fail=0` and rc=0. MEASURED
# 2026-09-04 by hq_C: running this suite CONCURRENTLY with test_corpus_snobol4.sh produced exactly that --
# scorecard_snobol4.sh's own concurrent-board registry (sc_peers, the thing that exists to refuse a contended
# measurement) declined to grade, left an empty results.tsv, and this wrapper turned that refusal into a
# PASS. The same suite run solo, same binary, same minute, read total=144 scored=126. A board that reports
# "no failures" because it graded nothing is the false green the registry was built to prevent, arriving
# through the one path the registry cannot see: its own caller's arithmetic.
[ "$TOTAL" -gt 0 ] || { echo "⛔ REFUSE(rc=2): results.tsv is EMPTY (total=0) -- scorecard_snobol4.sh exited rc=$rc and graded nothing, so there is no denominator and no verdict. This is NOT a pass. Commonest cause: another board was live and the concurrent-board registry declined (run this suite solo). run.log:"; cat "$W/run.log"; exit 2; }
[ "$SCORED" -gt 0 ] || { echo "⛔ REFUSE(rc=2): every one of the $TOTAL entries is ORACLE_FAIL (scored=0) -- the oracle, not SCRIP, is what this run measured. Preflight the oracle before trusting any verdict."; exit 2; }
echo "GIMPEL_BOARD total=$TOTAL scored=$SCORED unscr=$UNSCR m3_pass=$M3P m3_fail=$M3F m4_pass=$M4P m4_fail=$M4F -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf (via scorecard_snobol4.sh --suites gimpel)"
awk -F'\t' '$3=="ORACLE_FAIL"{printf "  UNSCR  %s  %s\n", $2, $7}' "$TSV"
awk -F'\t' '$3!="ORACLE_FAIL" && ($3!="PASS" || $4!="PASS"){printf "  RED    %s  m3=%s m4=%s%s\n", $2, $3, $4, ($7!="" ? "  "$7 : "")}' "$TSV"
[ "$M3F" = 0 ] && [ "$M4F" = 0 ]
