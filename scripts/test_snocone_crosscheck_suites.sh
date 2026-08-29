#!/usr/bin/env bash
# test_snocone_crosscheck_suites.sh — grades every corpus/tests/snocone/crosscheck_*.{sc,ref} suite pair.
# D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; S4E_HOME overrides).
#
# These 30 pairs are the whole of the former corpus/crosscheck/snocone/ tree (181 entries), converted under
# corpus-crosscheck-probe-total-conversion (Lon 2026-08-28: crosscheck must go). 18 of the 181 are XFAIL
# witnesses for genuinely unlanded features; they bucket as <mode>_xfail, never as <mode>_fail, so this gate's
# FAIL count is unaffected by them — and an XPASS (a marker whose bug got fixed) is surfaced as loudly as a FAIL.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
D="$S4E/corpus/tests/snocone"
H="$HERE/corpus_suite_harness.py"

[ -f "$H" ] || { echo "REFUSE (rc=2): corpus_suite_harness.py not found at $H -- cannot measure, not a pass"; exit 2; }
shopt -s nullglob
suites=("$D"/crosscheck_*.sc)
[ "${#suites[@]}" -gt 0 ] || { echo "REFUSE (rc=2): no crosscheck_*.sc suites under $D -- cannot measure, not a pass"; exit 2; }

echo "=== snocone crosscheck suites (${#suites[@]} families) ==="
TOT=0; PASS=0; XFAIL=0; BAD=0
for sc in "${suites[@]}"; do
    fam="$(basename "$sc" .sc)"; ref="${sc%.sc}.ref"
    [ -f "$ref" ] || { echo "  FAIL $fam: .sc present but .ref missing"; BAD=$((BAD+1)); continue; }
    board=$(python3 "$H" run "$sc" "$ref" --lang snocone --modes m3,m4 2>/dev/null | grep '^SUITE_BOARD')
    [ -n "$board" ] || { echo "  FAIL $fam: harness emitted no SUITE_BOARD (cannot measure)"; BAD=$((BAD+1)); continue; }
    TOT=$((TOT + $(grep -oP 'total=\K[0-9]+' <<<"$board")))
    for m in m3 m4; do
        PASS=$((PASS + $(grep -oP "${m}_pass=\K[0-9]+" <<<"$board")))
        for k in fail crash hang unproven xpass; do
            v=$(grep -oP "${m}_${k}=\K[0-9]+" <<<"$board")
            [ "$v" -eq 0 ] || { echo "  FAIL $fam: ${m}_${k}=${v}"; BAD=$((BAD+v)); }
        done
    done
    XFAIL=$((XFAIL + $(grep -oP 'm3_xfail=\K[0-9]+' <<<"$board")))
done
echo ""
echo "entries=$TOT  PASS=$PASS (both modes)  XFAIL=$XFAIL (per mode)  FAIL=$BAD"
[ "$BAD" -eq 0 ] && echo "GATE OK" || echo "GATE FAILED"
[ "$BAD" -eq 0 ]
