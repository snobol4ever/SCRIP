#!/usr/bin/env bash
# test_gate_score_row_rewrites_in_place.sh — the invariant gate for THE ONE LEADERBOARD's write path
# (row `score-md-rows-are-rewritten-by-the-runner-that-measured-them`, hq_T 2026-09-03).
#
# WHAT IT PROTECTS. RULES.md's ONE-LEADERBOARD FACT RULE names one property twice, because it is the
# one that decides whether SCORE.md is worth reading: "a row is rewritten IN PLACE, never appended
# beside a stale one". An append-instead-of-rewrite bug does not look like a bug. It produces a file
# that is LONGER and FULLER and contains every number anyone ever measured, and a reader picking the
# first matching row gets whichever measurement happened to land first — the oldest one. That is a
# leaderboard that gets more wrong the more it is used, and it would pass any test that only asked
# "did the new number appear".  So this gate asserts the new number appeared AND the old one did not
# survive AND the row count did not move.
#
# ⛔ AND EVERY REFUSAL PATH, because the second half of the rule is that this helper must never invent
# a cell. The refusals are the product here as much as the writes are: unknown language, unknown
# column, a cell with no digit in it, an unsigned row, and a '|' that would silently reshape the
# markdown table. Each must exit 2. A helper that guesses is worse than no helper, because the row it
# guesses carries a tree stamp that makes it look measured.
#
# ⭐⭐ THE PROPERTY PAID OFF UNDER REAL CONTENTION ON DAY ONE, and this is hq_C's report of it
# (2026-09-03), kept here because it is evidence rather than argument. They ran a write for snobol4
# and icon; between the write and the push, another session rewrote BOTH rows, and the rebase gave a
# genuine content conflict on SCORE.md. Their words: "because it rewrites ONE cell in place with
# provenance instead of regenerating the file, the conflict was two lines wide and readable, and I
# could tell in one glance which half of each row was newer. An append-style board would have left me
# two plausible rows and no way to choose." They kept the other session's snobol4 row (its entries
# column was genuinely newer) and their own icon row (378 on a descendant supersedes 377 on an
# ancestor) -- a merge, never a force. ⛔ So rewrite-in-place is not merely tidier than appending: it
# is what makes a concurrent conflict DECIDABLE by a human in one glance. That is the property this
# gate exists to keep, and the reason it is worth an arm rather than a comment.
#
# ⛔ IT GRADES A SCRATCH COPY, NEVER THE REAL BOARD. The helper's own `selftest` subcommand copies
# SCORE.md to a tempdir and writes there; this gate asserts that too (the real .github working tree
# must be no dirtier after the run than before), because a gate that edits the artifact it grades is
# how a green board and a corrupted file coexist.
#
# Usage: bash scripts/test_gate_score_row_rewrites_in_place.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

HELPER="$HERE/util_score_row.py"
gate_require "$HELPER" "the one leaderboard helper util_score_row.py"
gate_require "$ROOT/../.github/SCORE.md" "the ONE LEADERBOARD .github/SCORE.md"

# ⛔ The dirtiness baseline is taken BEFORE the selftest, not assumed clean: this gate runs in real
# sessions mid-landing, so "was dirty and stayed exactly as dirty" is the honest invariant, never
# "is clean". Comparing to a hardcoded clean would fail every seat that ran it while working.
GH="$ROOT/../.github"
before="$(git -C "$GH" status --porcelain -- SCORE.md 2>/dev/null)"

violations=0
examined=0

# ARM 1 — the helper's own selftest: rewrite-in-place over two writes, and all five refusal paths.
out="$(python3 "$HELPER" selftest 2>&1)"; rc=$?
examined=$((examined + 1))
echo "$out" | sed 's/^/    /'
if [ "$rc" -ne 0 ]; then
    echo "GATE FAIL: util_score_row.py selftest exited $rc (expected 0) — see its own SELFTEST lines above"
    violations=$((violations + 1))
fi
for want in "rewrite-in-place holds" "not appended beside it" "unknown language correctly REFUSED" \
            "unknown column correctly REFUSED" "no digit correctly REFUSED" \
            "pipe injection correctly REFUSED" \
            "absent measurer, known root correctly DERIVED" \
            "placeholder measurer, known root correctly DERIVED" \
            "absent measurer, numbered seat root correctly DERIVED" \
            "placeholder measurer on an UNKNOWN root correctly REFUSED"; do
    examined=$((examined + 1))
    case "$out" in
        *"$want"*) ;;
        *) echo "GATE FAIL: the selftest never proved: $want"; violations=$((violations + 1)) ;;
    esac
done

# ARM 2 — the selftest must not have touched the real board.
examined=$((examined + 1))
after="$(git -C "$GH" status --porcelain -- SCORE.md 2>/dev/null)"
if [ "$before" != "$after" ]; then
    echo "GATE FAIL: the selftest changed the REAL .github/SCORE.md (porcelain moved from '$before' to '$after')"
    echo "    A gate that edits the artifact it grades is how a green board and a corrupted file coexist."
    violations=$((violations + 1))
fi

# ARM 3 — the bash call shape exists in the shared authority, so runners have one line and not a
# second implementation. Sourcing lib_gate.sh already happened above; the function must be defined.
examined=$((examined + 1))
if ! declare -F gate_score_row >/dev/null 2>&1; then
    echo "GATE FAIL: lib_gate.sh defines no gate_score_row — bash runners have no one-line call, which is"
    echo "    how the FACT RULE degrades back into 'remember to edit the file by hand'."
    violations=$((violations + 1))
fi

# ARM 4 — `check` must run and produce a per-row staleness verdict. rc=1 (rows ARE stale) is a real
# answer and passes this gate; rc=2 (cannot measure) does not. ⛔ Capture first, then test: `$?` after
# a pipeline reports the pager, not the command (CLAUDE.md, measured live).
examined=$((examined + 1))
chk="$(python3 "$HELPER" check --no-fetch 2>&1)"; crc=$?
if [ "$crc" -ge 2 ]; then
    echo "GATE FAIL: util_score_row.py check could not measure staleness (rc=$crc)"
    echo "$chk" | sed 's/^/    /'
    violations=$((violations + 1))
elif ! printf '%s' "$chk" | grep -q 'commits behind origin/main\|UNPINNED\|UNKNOWN'; then
    echo "GATE FAIL: check printed no per-row staleness verdict at all"
    echo "$chk" | sed 's/^/    /'
    violations=$((violations + 1))
fi

GATE_EXAMINED="$examined arms"
gate_verdict "$violations" "leaderboard write-path invariants broken"
