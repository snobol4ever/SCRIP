#!/usr/bin/env bash
# test_gate_score_cell_no_silent_prose_loss.sh -- the invariant gate for cell_prose_loss (row
# score-md-runners-rewrite-a-cell-in-place-and-silently-discard-the-prose-they-never-modelled, hq_B mint
# 2026-09-04T00:36:35Z off hq_B's 2026-09-03 ICN4 finding).
#
# WHAT IT PROTECTS. Two independent code sites replace a SCORE.md cell wholesale --
# util_score_row.py:cmd_write's no-suite path (`cells[idx] = text`, ~17 runners including
# test_corpus_snobol4.sh and board_icon_master.sh) and util_apply_score_grid.py:apply_grid
# (`cells[di] = new`, the September-10-grid merge). Both can silently discard whatever hand-written prose
# a human appended to a cell and no runner ever modelled, WITH THE MEASURED NUMBERS UNCHANGED -- so
# nothing about the diff looks wrong. Measured live, .github 46ff295c: test_corpus_snobol4.sh dropped
# hq_P's re-confirmation sentence from the snobol4 board cell while the fraction it replaced (1689/1689
# FAIL=0) was byte-identical before and after. Repaired by hand; this gate is what stops it recurring.
#
# ⛔ AND THE OTHER DIRECTION, EQUALLY: the SAME repair shows a runner's own boilerplate drifting in
# punctuation between two captures with no human involved ("SKIP=0 · MISSING=0" became "SKIP=0
# MISSING=0"; a comma moved beside a citation backtick). A guard that also flags that reformatting is
# noisy enough that nobody keeps it turned on -- so this gate proves BOTH: the real sentence loss is
# caught, and the real boilerplate drift from the SAME commit is not.
#
# ⭐ PRESERVE-OR-REFUSE, RULED: this row's own text leaves the choice open ("hq_T owns the SCORE tooling
# and should rule"). Both call sites REFUSE (never silently rc=0) -- util_score_row.py:cmd_write refuses
# the single write outright (rc=2; every caller already has a `|| echo ...` fallback, so no caller needed
# a code change), util_apply_score_grid.py:apply_grid skips only the at-risk cell and returns rc=1 for the
# batch (a merge covers many rows in one call, so one risky cell must not abort the rest). An `ask` to
# hq_T is filed for this row's QA; if the ruling comes back "preserve", this gate is the one place that
# needs to change shape, and it should go red first to prove the new behaviour rather than being amended
# to match it after the fact.
#
# ⛔ IT GRADES SCRATCH COPIES, NEVER THE REAL BOARD -- both selftests copy .github/SCORE.md to a tempdir
# and write there; this gate asserts that too (the real .github working tree must be no dirtier after the
# run than before), same discipline as the sibling gate on this file, and for the same reason: a gate
# that edits the artifact it grades is how a green board and a corrupted file coexist.
#
# Usage: bash scripts/test_gate_score_cell_no_silent_prose_loss.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

ROW_HELPER="$HERE/util_score_row.py"
GRID_HELPER="$HERE/util_apply_score_grid.py"
gate_require "$ROW_HELPER" "the one leaderboard helper util_score_row.py"
gate_require "$GRID_HELPER" "the grid-merge helper util_apply_score_grid.py"
GH="$ROOT/../.github"
gate_require "$GH/SCORE.md" "the ONE LEADERBOARD .github/SCORE.md"

# ⛔ Dirtiness baseline BEFORE either selftest, not assumed clean -- this gate can run mid-landing.
before="$(git -C "$GH" status --porcelain -- SCORE.md 2>/dev/null)"

violations=0
examined=0

# ARM 1 -- cell_prose_loss itself, and cmd_write's integration of it, via util_score_row.py's own selftest.
examined=$((examined + 1))
out="$(python3 "$ROW_HELPER" selftest 2>&1)"; rc=$?
echo "$out" | sed 's/^/    /'
if [ "$rc" -ne 0 ]; then
    echo "GATE FAIL: util_score_row.py selftest exited $rc (expected 0) -- see its SELFTEST lines above"
    violations=$((violations + 1))
fi
for want in "cell_prose_loss did not false-positive on punctuation-only reformatting" \
            "cell_prose_loss correctly caught the real .github 46ff295c dropped sentence" \
            "cmd_write correctly REFUSED rc=2 rather than silently drop prose" \
            "cmd_write correctly proceeded on a pure reformat (nothing to lose)"; do
    examined=$((examined + 1))
    case "$out" in
        *"$want"*) ;;
        *) echo "GATE FAIL: util_score_row.py selftest never proved: $want"; violations=$((violations + 1)) ;;
    esac
done

# ARM 2 -- the SAME guard, wired into the OTHER wholesale-overwrite site, via its own selftest. A fix
# proven only in util_score_row.py says nothing about util_apply_score_grid.py: the two share the helper
# function, never a call path, so a regression in either call site needs its own witness.
examined=$((examined + 1))
gout="$(python3 "$GRID_HELPER" --selftest 2>&1)"; grc=$?
echo "$gout" | sed 's/^/    /'
if [ "$grc" -ne 0 ]; then
    echo "GATE FAIL: util_apply_score_grid.py --selftest exited $grc (expected 0) -- see its SELFTEST lines above"
    violations=$((violations + 1))
fi
examined=$((examined + 1))
case "$gout" in
    *"apply_grid correctly skipped a cell that would have discarded hand-written prose (rc=1)"*) ;;
    *) echo "GATE FAIL: util_apply_score_grid.py selftest never proved the merge path skips an at-risk cell"
       violations=$((violations + 1)) ;;
esac

# ARM 3 -- neither selftest may have touched the real board.
examined=$((examined + 1))
after="$(git -C "$GH" status --porcelain -- SCORE.md 2>/dev/null)"
if [ "$before" != "$after" ]; then
    echo "GATE FAIL: a selftest changed the REAL .github/SCORE.md (porcelain moved from '$before' to '$after')"
    echo "    A gate that edits the artifact it grades is how a green board and a corrupted file coexist."
    violations=$((violations + 1))
fi

# ARM 4 -- the function both call sites share must actually be the SAME function, not two forks of one
# idea that will only ever be tested and fixed together by accident. Cheap to assert, expensive to lose.
examined=$((examined + 1))
if ! grep -q "^def cell_prose_loss" "$ROW_HELPER"; then
    echo "GATE FAIL: util_score_row.py defines no cell_prose_loss -- the shared guard moved or was removed"
    violations=$((violations + 1))
elif ! grep -q "_usr\.cell_prose_loss" "$GRID_HELPER"; then
    echo "GATE FAIL: util_apply_score_grid.py no longer calls _usr.cell_prose_loss -- the two call sites"
    echo "    have drifted apart; either re-wire the shared call or split this gate's ARM 1/ARM 2 for real."
    violations=$((violations + 1))
fi

GATE_EXAMINED="$examined arms"
gate_verdict "$violations" "a SCORE.md cell overwrite silently discarded hand-written prose it never modelled"
