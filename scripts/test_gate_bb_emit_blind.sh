#!/usr/bin/env bash
# test_gate_bb_emit_blind.sh — EMIT-BLIND gate for GOAL-BB-FIXUP.md
# Counts direct neighbor dereferences (pBB->[αβγω]) in BB_templates/*.cpp (comments stripped).
# INFORMATIONAL at baseline; --strict enforces zero (FIX-FENCE requirement).
# Baseline snapshot 2026-06-04: 12 files, 132 direct pBB->[αβγω] refs
# + 5 files with local-alias neighbor walks (lf->t / fin->.*->t / a0->t).
# Usage: bash scripts/test_gate_bb_emit_blind.sh [--strict]
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
strict=0
[ "${1:-}" = "--strict" ] && strict=1
strip() { perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$1"; }
total_direct=0
total_alias=0
report_direct=""
report_alias=""
for f in src/emitter/BB_templates/bb_*.cpp; do
    [ -f "$f" ] || continue
    name="$(basename "$f")"
    direct=$(strip "$f" | grep -cE 'pBB->[αβγω]' || true)
    alias=$(strip  "$f" | grep -cE '\b(lf|fin|a0|a1|arm|node)->(α|β|γ|ω|t)\b' || true)
    [ "$direct" -gt 0 ] && { total_direct=$((total_direct + direct)); report_direct="$report_direct $name($direct)"; }
    [ "$alias"  -gt 0 ] && { total_alias=$((total_alias + alias));   report_alias="$report_alias $name($alias)"; }
done
total=$((total_direct + total_alias))
echo "=== BB EMIT-BLIND gate (neighbor dereferences in templates) ==="
printf "  Direct pBB->[αβγω] refs: %d" "$total_direct"
[ -n "$report_direct" ] && printf "  [%s]" "$report_direct" || true
echo ""
printf "  Alias neighbor-walk refs: %d" "$total_alias"
[ -n "$report_alias" ] && printf "  [%s]" "$report_alias" || true
echo ""
printf "  TOTAL emit-blind refs: %d  (baseline 2026-06-04: 132 direct + alias)\n" "$total"
if [ "$strict" -eq 1 ]; then
    if [ "$total" -ne 0 ]; then
        echo "FAIL (--strict): $total emit-blind neighbor references remain; FIX-FENCE requires zero."
        exit 1
    fi
    echo "PASS (--strict): zero emit-blind neighbor references — EMIT-BLIND clean."
else
    echo "(informational — run with --strict to gate at zero)"
fi
exit 0
