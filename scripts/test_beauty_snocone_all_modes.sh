#!/usr/bin/env bash
# test_beauty_snocone_all_modes.sh -- SC-19/SC-20/SC-21/SC-22
# Run all 14 beauty-sc subsystems under BOTH real modes: --run (m3) and
# --compile (m4).
# Gate: 14 PASS + 1 SKIP (beauty, no beauty.sc) per mode
#
# B-9 FIX (BOARD): MODES was (--run --run --run) -- --compile was never
# exercised. See scripts/test_gate_sn7_beauty_self_host.sh for the same
# class fixed the same session.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
BEAUTY_DIR="${CORPUS:-$S4E/corpus}/snocone/demo/snobol4/beauty/test"
TIMEOUT="${TIMEOUT:-10}"
GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[0;33m'; RESET='\033[0m'

SUBSYSTEMS=(arith assign fence global match roman semantic ShiftReduce ReadWrite counter stack strings trace beauty tree)
MODES=(--run --compile)

PASS=0; FAIL=0; SKIP=0

run_one() {
    local subsys="$1" mode="$2"
    local sc="$BEAUTY_DIR/test_$subsys.sc"
    local ref="$BEAUTY_DIR/test_$subsys.ref"
    if [[ ! -f "$sc" ]]; then
        echo -e "${YELLOW}SKIP${RESET}  $subsys $mode (no $subsys.sc)"; SKIP=$((SKIP+1)); return
    fi
    if [[ ! -f "$ref" ]]; then
        echo -e "${YELLOW}SKIP${RESET}  $subsys $mode (no $subsys.ref)"; SKIP=$((SKIP+1)); return
    fi
    local got; got=$(timeout "$TIMEOUT" "$SCRIP" "$mode" "$sc" < /dev/null 2>/dev/null) || true
    local exp; exp=$(cat "$ref")
    if [[ "$got" == "$exp" ]]; then
        echo -e "${GREEN}PASS${RESET}  $subsys $mode"; PASS=$((PASS+1))
    else
        echo -e "${RED}FAIL${RESET}  $subsys $mode"
        diff <(echo "$exp") <(echo "$got") | head -10
        FAIL=$((FAIL+1))
    fi
}

echo "=== beauty-sc all modes ==="
for mode in "${MODES[@]}"; do
    echo "--- $mode ---"
    for subsys in "${SUBSYSTEMS[@]}"; do
        run_one "$subsys" "$mode"
    done
done

echo ""
echo "PASS=$PASS FAIL=$FAIL SKIP=$SKIP"
# ⛔⭐ ZERO-GRADED IS A REFUSAL, NOT A PASS (hq_B 2026-08-30, row graders-denominator-audit-six-families).
# MEASURED: BEAUTY_DIR points at $CORPUS/snocone/demo/... and corpus/snocone/ DOES NOT EXIST -- that path
# predates the corpus re-grid to verbs-on-top. No beauty test_*.sc witness exists anywhere under corpus
# (searched: zero hits). So every subsystem took the SKIP arm, PASS and FAIL both stayed 0, and
# `[[ $FAIL -eq 0 ]]` reported GREEN having graded nothing -- skip-as-success over a dead path.
# ⭐ The denominator was declared in this file's own header the whole time ("# Gate: 14 PASS + 1 SKIP
# per mode") and nothing ever compared against it. A declared expectation no code reads is a comment.
# ⛔ REFUSING RATHER THAN RE-POINTING, ON PURPOSE: the witnesses are genuinely gone, not renamed -- this
# is NOT the absorbed-into-ALL.* shape. Its own sibling test_gate_sn7_beauty_self_host.sh already refuses
# UNPROVEN(2) for the same corpus reorg (ceo: "pre-existing since a corpus reorg, rides the beauty rows"),
# so this now agrees with it instead of contradicting it with a green. Restoring the witnesses is a
# beauty-row decision, not grader hygiene.
if [ $((PASS+FAIL)) -eq 0 ]; then
    echo "REFUSE (rc=2): graded ZERO subsystems -- BEAUTY_DIR=$BEAUTY_DIR does not exist (pre-re-grid path) and no beauty test_*.sc witness exists anywhere under corpus. Cannot measure, not a pass. Same corpus reorg as test_gate_sn7_beauty_self_host.sh; rides the beauty rows."
    exit 2
fi
[[ $FAIL -eq 0 ]]
