#!/usr/bin/env bash
# test_icon_ir_rung_36.sh — rung36: JCON integration suite (75 tests) — IC-7
# Tests marked .xfail are known-unimplemented features (co-expressions, large integers,
# &error trapping, etc.) — they count as XFAIL, not FAIL.
# ⛔ 2026-08-29 (seat07, row tests-consolidate-icon): 32 of the 75 were converted into a suite pair
# (rung36_all.icn/.ref, see corpus's tests/icon/KEEP.md ## rung36) and their loose .icn/.expected
# files no longer exist. Per FINDING-2026-08-29-hq_P-converting-a-family-silently-disarms-its-
# per-family-glob-script.md: a converted family's entries MUST be delegated to the harness, never
# left to fall through this script's own [ -f ... ] checks as a silent SKIP (SKIP is not checked by
# the final `[ "$FAIL" -eq 0 ]`, so 32 silently-skipped tests would still read as a clean pass). The
# remaining 43 (29 still-red .xfail witnesses + 14 newly-found-red, see KEEP.md) stay loose and are
# still tested individually below, unchanged — only the now-converted 32 moved to delegation.
# Authors: LCherryholmes · Claude Sonnet 4.6   DATE: 2026-04-16
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/icon}"
PASS=0; FAIL=0; XFAIL=0

if [ ! -x "$SCRIP" ];  then echo "SKIP scrip not found at $SCRIP";  exit 0; fi
if [ ! -d "$CORPUS" ]; then echo "SKIP corpus not found at $CORPUS"; exit 0; fi

run() {
    local base="$CORPUS/$1"
    [ -f "${base}.xfail" ] && { echo "  XFAIL $1"; XFAIL=$((XFAIL+1)); return; }
    [ -f "${base}.expected" ] || { echo "  SKIP  $1 (no .expected)"; return; }
    local stdin_f="${base}.stdin"
    local got want
    if [ -f "$stdin_f" ]; then
        got=$(timeout 30 "$SCRIP" --run "${base}.icn" < "$stdin_f"  2>/dev/null) || true
    else
        got=$(timeout 30 "$SCRIP" --run "${base}.icn" < /dev/null   2>/dev/null) || true
    fi
    want=$(cat "${base}.expected")
    if [ "$got" = "$want" ]; then
        echo "  PASS $1"; PASS=$((PASS+1))
    else
        echo "  FAIL $1"
        echo "    want: $(echo "$want" | head -3 | tr '\n' '|')"
        echo "    got:  $(echo "$got"  | head -3 | tr '\n' '|')"
        FAIL=$((FAIL+1))
    fi
}

echo "=== rung36: JCON integration suite (43 still-loose entries, individually diagnosed) ==="
run rung36_jcon_args
run rung36_jcon_btrees
run rung36_jcon_case
run rung36_jcon_checkfpx
run rung36_jcon_ck
run rung36_jcon_collate
run rung36_jcon_cxprimes
run rung36_jcon_endetab
run rung36_jcon_errkwds
run rung36_jcon_errors
run rung36_jcon_evalx
run rung36_jcon_every
run rung36_jcon_fncs
run rung36_jcon_fncs1
run rung36_jcon_geddump
run rung36_jcon_gener
run rung36_jcon_genqueen
run rung36_jcon_image
run rung36_jcon_io
run rung36_jcon_iobig
run rung36_jcon_kwds
run rung36_jcon_large
run rung36_jcon_level
run rung36_jcon_lgint
run rung36_jcon_misc
run rung36_jcon_nargs
run rung36_jcon_others
run rung36_jcon_prefix
run rung36_jcon_prepro
run rung36_jcon_profsum
run rung36_jcon_proto
run rung36_jcon_radix
run rung36_jcon_recent
run rung36_jcon_recogn
run rung36_jcon_scan
run rung36_jcon_scan1
run rung36_jcon_scan2
run rung36_jcon_sets
run rung36_jcon_sorting
run rung36_jcon_struct
run rung36_jcon_subjpos
run rung36_jcon_toby
run rung36_jcon_var

echo ""
echo "=== rung36: converted suite delegation (32 entries in rung36_all.icn/.ref) ==="
SUITE_SNO="$CORPUS/rung36_all.icn"
SUITE_REF="$CORPUS/rung36_all.ref"
if [ ! -f "$SUITE_SNO" ] || [ ! -f "$SUITE_REF" ]; then
    echo "REFUSE (rc=2): rung36_all suite not found under $CORPUS -- cannot measure the 32 converted entries, not a pass"
    exit 2
fi
board=$(python3 "$HERE/corpus_suite_harness.py" run "$SUITE_SNO" "$SUITE_REF" --lang icon --modes m3,m4 2>&1 | grep '^SUITE_BOARD') || true
if [ -z "$board" ]; then
    echo "REFUSE (rc=2): rung36_all harness produced no SUITE_BOARD line -- cannot measure, not a pass"
    exit 2
fi
echo "  $board"
for mode in m3 m4; do
    p=$(echo "$board" | grep -oP "${mode}_pass=\K[0-9]+")
    f=$(echo "$board" | grep -oP "${mode}_fail=\K[0-9]+")
    c=$(echo "$board" | grep -oP "${mode}_crash=\K[0-9]+")
    h=$(echo "$board" | grep -oP "${mode}_hang=\K[0-9]+")
    u=$(echo "$board" | grep -oP "${mode}_unproven=\K[0-9]+")
    PASS=$((PASS+p))
    FAIL=$((FAIL+f+c+h+u))
done

echo ""
echo "PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL TOTAL=$((PASS+FAIL+XFAIL))"
[ "$FAIL" -eq 0 ]
