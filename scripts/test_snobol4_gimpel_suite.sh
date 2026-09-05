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
# ⛔⭐ THE OVERLAY IS NOT TRUSTED, IT IS CHECKED -- BEFORE AND AFTER, EVERY RUN (seat16, 2026-09-04 19:16 CDT:
# witnessed ASMTEMP in the vendored dir AFTER the overlay cure landed in its clone, and could not reproduce it
# mechanically; hq_T could not reproduce it either -- a full 144-program run with a one-second watcher armed on
# the real directory left it clean). ⛔ TWO PEOPLE UNABLE TO REPRODUCE A WITNESSED LEAK IS NOT EVIDENCE THAT IT
# DID NOT HAPPEN; it is evidence that argument is the wrong instrument. So the runner now measures its own side
# effect: it fingerprints the vendored directory before grading and again after, and if a single byte differs it
# REFUSES rc=2 naming the files. ⭐ A guard that fires ONCE with the file named is worth more than an hour of
# reasoning about whether an overlay can leak, and it costs two `find` calls.
_vend_fingerprint() { find "$CORPUS_REAL/packages/snobol4/gimpel" -type f -printf '%p %s %T@\n' 2>/dev/null | LC_ALL=C sort; }
VEND_BEFORE="$(_vend_fingerprint)"
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
# ⛔⭐ TWO HQs CURED THE total=0 DEFECT INDEPENDENTLY, WITHIN ONE HOUR, IN THIS FILE -- hq_T (7b18a3c52, hit via
# the vendored-dir overlay row) and hq_C (cfde5756f, hit by running gimpel concurrently with the master board).
# Both cures were right and the merge left the tree carrying BOTH, one above and one here. Consolidated into
# the single early refusal above, which fires before the counters and prints the scorecard's own log.
# ⭐ THE COINCIDENCE IS THE LESSON, not the duplication: this is the one-cure-many-copies class running FORWARD
# for once -- the same defect, found twice the same hour by two people who never spoke, because a concurrent
# board makes it reproducible on demand. A defect that two independent lanes hit in an hour was never rare; it
# was only invisible, and it will be there again in the next wrapper nobody has run two of yet.
# ⛔ THIS ARM IS hq_C's AND IT IS NOT A DUPLICATE: total=0 is "nothing was graded"; scored=0 is "every entry was
# graded and the ORACLE failed on all of them" -- a different fact, about a different component, with a
# different cure. Keeping only the first would have re-opened half the hole.
[ "$SCORED" -gt 0 ] || { echo "⛔ REFUSE(rc=2): every one of the $TOTAL entries is ORACLE_FAIL (scored=0) -- the oracle, not SCRIP, is what this run measured. Preflight the oracle before trusting any verdict."; exit 2; }
VEND_AFTER="$(_vend_fingerprint)"
if [ "$VEND_BEFORE" != "$VEND_AFTER" ]; then
    echo "⛔ REFUSE(rc=2): THIS RUN CHANGED THE VENDORED DIRECTORY IT WAS GRADING -- the scratch overlay did not"
    echo "   contain a write. The board below is NOT trustworthy (a grader that writes into its own fixtures may"
    echo "   have graded a file it had already altered), and the tree is now dirty, which correctly blocks the"
    echo "   leaderboard write. What changed:"
    diff <(printf '%s\n' "$VEND_BEFORE") <(printf '%s\n' "$VEND_AFTER") | sed 's/^/     /' | head -20
    echo "   Cure: remove the stray file, then report this WITH the diff above -- it is the first mechanical"
    echo "   evidence of an escape that two people have witnessed and neither could reproduce."
    exit 2
fi
echo "GIMPEL_BOARD total=$TOTAL scored=$SCORED unscr=$UNSCR m3_pass=$M3P m3_fail=$M3F m4_pass=$M4P m4_fail=$M4F -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf (via scorecard_snobol4.sh --suites gimpel)"
awk -F'\t' '$3=="ORACLE_FAIL"{printf "  UNSCR  %s  %s\n", $2, $7}' "$TSV"
awk -F'\t' '$3!="ORACLE_FAIL" && ($3!="PASS" || $4!="PASS"){printf "  RED    %s  m3=%s m4=%s%s\n", $2, $3, $4, ($7!="" ? "  "$7 : "")}' "$TSV"
# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): WITNESSED TWICE IN ONE HOUR on this exact file -- the concurrent-board
# registry in scorecard_snobol4.sh declined (rc=$rc, an UPSTREAM refusal), which truncates results.tsv
# to empty and leaves it there; `-f "$TSV"` above only proves the FILE exists, not that it holds any
# rows, so TOTAL/SCORED/M3F/M4F all read 0 and the line below used to read that as clean. NO LOGIC
# HERE: util_require_population.sh sources gate_floor from lib_gate.sh, the ONE authority.
"$HERE/util_require_population.sh" --gate test_snobol4_gimpel_suite "$SCORED" 1 "scored rows (total=$TOTAL unscr=$UNSCR, scorecard rc=$rc)" || exit 2
[ "$M3F" = 0 ] && [ "$M4F" = 0 ]
