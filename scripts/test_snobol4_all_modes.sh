#!/usr/bin/env bash
# scripts/test_snobol4_all_modes.sh — SNOBOL4 mode-4 summary runner (Lon directive 2026-06-06)
# Formerly collected M2/M3/M4. Now mode-4 only.
# Runs every SNOBOL4 test suite and displays a mode-4 progress table.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
export SCRIP

_num() { printf '%s' "$1" | grep -oE '[0-9]+' | head -1; }

run_suite() {
    local label="$1" script="$2"
    local out t0 t1
    t0=$SECONDS
    out=$(bash "$script" 2>/dev/null || true)
    t1=$SECONDS
    local p4=0 f4=0
    p4=$(_num "$(printf '%s\n' "$out" | grep -E 'PASS[=-][0-9]+' | grep -oE 'PASS[=-][0-9]+' | tail -1)")
    f4=$(_num "$(printf '%s\n' "$out" | grep -E 'FAIL[=-][0-9]+' | grep -oE 'FAIL[=-][0-9]+' | tail -1)")
    p4=${p4:-0}; f4=${f4:-0}
    printf "  %-42s  M4: PASS=%-4d FAIL=%-4d  %ds\n" \
        "$label" "$p4" "$f4" "$((t1-t0))"
}

T0_ALL=$SECONDS
echo "=== SNOBOL4 — MODE-4 ONLY progress table ==="
echo ""
run_suite "smoke"              "$HERE/test_smoke_snobol4.sh"
run_suite "pat_rung (038-057)" "$HERE/test_snobol4_pat_rung_suite.sh"
run_suite "corpus" "$HERE/test_corpus_snobol4.sh"
echo ""
printf "TOTAL TIME: %ds\n" "$((SECONDS-T0_ALL))"
