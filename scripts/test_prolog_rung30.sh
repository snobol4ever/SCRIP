#!/usr/bin/env bash
# test_prolog_rung30.sh — DCG: basic_terminals, generate, nonterminals, phrase3, pushback_rest
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPBIN="${HERE}/../scrip"
CORPUS="$S4E/corpus/tests/prolog"
FAMILY=rung30
SNO="$CORPUS/$FAMILY.pl"
REF="$CORPUS/$FAMILY.ref"
PASS=0; FAIL=0
[ -d "$CORPUS" ] || { echo "⛔ REFUSED-TO-GRADE: $CORPUS missing"; exit 2; }

echo "=== rung30: DCG (basic_terminals/generate/nonterminals/phrase3/pushback_rest) ==="

# consolidated 2026-08-28 (tests-consolidate-prolog): 3 of 5 entries (basic_terminals, phrase3,
# pushback_rest -- deterministic DCG parses, no surviving choice point) converted to the suite
# pair below -- delegate to `run`, which extracts and runs each entry alone (a suite file is
# never run whole, corpus_suite_harness.py's own docstring). The other 2 (generate: findall
# over phrase/2; nonterminals: multi-clause DCG needing backtracking across 3 phrase/2 calls)
# both crash 3/3 direct runs with "*** stack smashing detected ***" (rc=134) -- fits PZ-4's
# documented trigger set (prolog-multiclause-uninit-lexprep-frame). They stay loose ON PURPOSE,
# not KEEP.md (not a permanent design choice, see task file tests-consolidate-prolog), and are
# DELIBERATELY KEPT in this script's own board below (rc-checked, not stdout-only -- see
# test_prolog_rung13/14's same-day fix for why) so the crash stays visible instead of silently
# disappearing once PZ-4 lands.
if [ -f "$SNO" ] && [ -f "$REF" ]; then
    out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog --modes m3 2>&1)
    echo "$out" | grep -v '^SUITE_BOARD'
    board=$(echo "$out" | grep '^SUITE_BOARD')
    p=$(echo "$board" | grep -oP 'm3_pass=\K[0-9]+')
    f=$(echo "$board" | grep -oP 'm3_fail=\K[0-9]+')
    c=$(echo "$board" | grep -oP 'm3_crash=\K[0-9]+')
    h=$(echo "$board" | grep -oP 'm3_hang=\K[0-9]+')
    u=$(echo "$board" | grep -oP 'm3_unproven=\K[0-9]+')
    PASS=$((PASS+p)); FAIL=$((FAIL+f+c+h+u))
fi

shopt -s nullglob
for f in "$CORPUS"/rung30_dcg_generate.pl "$CORPUS"/rung30_dcg_nonterminals.pl; do
    ref="${f%.pl}.expected"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIPBIN" --run "$f" < /dev/null 2>/dev/null)
    rc=$?
    expected=$(cat "$ref")
    if [ "$rc" -eq 0 ] && [ "$actual" = "$expected" ]; then
        echo "  PASS $(basename "$f")"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename "$f") (loose -- known PZ-4 prolog-multiclause-uninit-lexprep-frame, rc=$rc)"; FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"
[ "$FAIL" -eq 0 ]
