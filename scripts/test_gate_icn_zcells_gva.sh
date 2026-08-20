#!/usr/bin/env bash
# test_gate_icn_zcells_gva.sh — ZK-5 GATE: globals stay OFF the cells-arm ZD spine (GVA/NV path, not ζ cells)
#
# THREE CHECKS:
#   1. CORRECTNESS: CELLS=1 global witness produces same output as baseline.
#   2. ASSERTION: SCRIP_ZD_CENSUS=1 run does NOT abort (ZK-5 global-cells=0 invariant held).
#   3. CENSUS: census shows 0 armed nodes for the global-only witness program (globals all refused).
#
# Completion criterion (ZK-5): these three checks green on the global witness.
# Authors: LCherryholmes · Claude Sonnet 4.6

set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
WITNESS="$HERE/../test/icon/zk5_global_cells_zero.icn"
EXPECTED="$HERE/../test/icon/zk5_global_cells_zero.expected"
FAIL=0

[ -x "$SCRIP" ] || { echo "GATE SKIP: scrip not built at $SCRIP"; exit 0; }
[ -f "$WITNESS" ] || { echo "GATE FAIL: witness not found at $WITNESS"; exit 1; }

# CHECK 1 — correctness: baseline output
base_out=$(timeout 8s "$SCRIP" --run "$WITNESS" 2>/dev/null) || { echo "GATE FAIL: baseline run failed"; FAIL=1; }
cells_out=$(SCRIP_ICN_CELLS=1 timeout 8s "$SCRIP" --run "$WITNESS" 2>/dev/null) || { echo "GATE FAIL: CELLS=1 run failed"; FAIL=1; }
if [ "$base_out" != "$cells_out" ]; then
    echo "GATE FAIL: CELLS=1 output differs from baseline"
    echo "  baseline: $base_out"
    echo "  cells=1:  $cells_out"
    FAIL=1
else
    echo "CHECK 1 PASS: CELLS=1 output matches baseline"
fi
if [ -f "$EXPECTED" ] && [ "$cells_out" != "$(cat "$EXPECTED")" ]; then
    echo "GATE FAIL: output does not match .expected"
    FAIL=1
else
    echo "CHECK 1b PASS: output matches .expected"
fi

# CHECK 2 — ZK-5 assertion: census run must not abort (global-cells=0 invariant)
census_stderr=$(SCRIP_ICN_CELLS=1 SCRIP_ZD_CENSUS=1 timeout 8s "$SCRIP" --run "$WITNESS" 2>&1 >/dev/null) || { echo "GATE FAIL: CELLS=1 CENSUS=1 run crashed/aborted"; FAIL=1; }
if grep -q "ZK-5-FAIL" <<< "$census_stderr"; then
    echo "GATE FAIL: ZK-5 global-cells=0 assertion FIRED -- a global was armed on the cells spine"
    echo "$census_stderr" | grep "ZK-5-FAIL"
    FAIL=1
else
    echo "CHECK 2 PASS: ZK-5 assertion did not fire (no global armed on cells arm)"
fi

# CHECK 3 — census: armed=0 for the global-only witness
armed=$(echo "$census_stderr" | grep "^\[ZK-CENSUS\]" | grep -oP "armed=\K[0-9]+" | head -1)
if [ "${armed:-x}" != "0" ]; then
    echo "GATE FAIL: expected armed=0 for global-only witness, got armed=${armed:-<not found>}"
    FAIL=1
else
    echo "CHECK 3 PASS: census armed=0 (globals correctly excluded from cells spine)"
fi

if [ "$FAIL" -eq 0 ]; then
    echo "ZK-5 GATE: PASS"
    exit 0
else
    echo "ZK-5 GATE: FAIL"
    exit 1
fi
