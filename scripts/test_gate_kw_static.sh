#!/usr/bin/env bash
# test_gate_kw_static.sh — KW-STATIC gate (GOAL-SNOBOL4-100 D-3, rungs KW-1..KW-4)
#
# Runs corpus/probe/kw/ in BOTH modes against the live-oracle .ref files.
# KW-1 (census) minted the witnesses; the gate reads 0/4 at census time and
# must read 4/4 before KW-4 is done.  Record: FINDING-2026-08-19-s146.
#
# Usage:  bash scripts/test_gate_kw_static.sh [--mode 3|4|both] [--verbose]
# Exit:   0 = every witness PASS in every selected mode; 1 = at least one DIFF.
set -uo pipefail
SCRIP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROBE_DIR="${KW_PROBE_DIR:-/home/claude/corpus/probe/kw}"
SCRIP_BIN="${SCRIP_BIN:-$SCRIP_DIR/scrip}"
MODE="both"; VERBOSE=0; ARMED="${SCRIP_KW_STATIC:-}"
while [[ $# -gt 0 ]]; do
    case "$1" in
        --mode) MODE="$2"; shift 2 ;;
        --verbose|-v) VERBOSE=1; shift ;;
        --armed) ARMED=1; shift ;;
        --legacy) ARMED=0; shift ;;
        *) echo "unknown option: $1"; exit 2 ;;
    esac
done
[[ -x "$SCRIP_BIN" ]] || { echo "GATE BLOCKED: no scrip binary at $SCRIP_BIN (run make -j8 scrip)"; exit 2; }
[[ -d "$PROBE_DIR" ]] || { echo "GATE BLOCKED: no probe dir at $PROBE_DIR (clone corpus)"; exit 2; }
[[ -n "$ARMED" ]] && export SCRIP_KW_STATIC="$ARMED"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
pass=0; fail=0; failed_names=()
# a witness may ship a .dat beside it -- that file is its stdin (kw_trim_effect needs one to
# show that &TRIM actually trims).  Absent, stdin is /dev/null.
stdin_for() { local d="${1%.sno}.dat"; [[ -f "$d" ]] && echo "$d" || echo /dev/null; }
run_mode3() { timeout 30 "$SCRIP_BIN" --run "$1" < "$(stdin_for "$1")" > "$2" 2>&1; }
run_mode4() {
    local sno="$1" out="$2" s="$WORK/m4.s" exe="$WORK/m4.bin"
    timeout 30 "$SCRIP_BIN" --compile "$sno" < /dev/null > "$s" 2>/dev/null || return 1
    gcc -no-pie -o "$exe" "$s" -L"$SCRIP_DIR/out" -lscrip_rt -lm > /dev/null 2>&1 || return 1
    LD_LIBRARY_PATH="$SCRIP_DIR/out" timeout 30 "$exe" < "$(stdin_for "$sno")" > "$out" 2>&1
}
for ref in "$PROBE_DIR"/*.ref; do
    [[ -e "$ref" ]] || { echo "GATE BLOCKED: no .ref files in $PROBE_DIR"; exit 2; }
    base="$(basename "$ref" .ref)"; sno="$PROBE_DIR/$base.sno"
    [[ -f "$sno" ]] || { echo "SKIP  $base (no .sno beside .ref)"; continue; }
    for m in 3 4; do
        [[ "$MODE" == "both" || "$MODE" == "$m" ]] || continue
        out="$WORK/$base.m$m"
        if [[ "$m" == 3 ]]; then run_mode3 "$sno" "$out"; else run_mode4 "$sno" "$out"; fi
        if diff -q "$ref" "$out" > /dev/null 2>&1; then
            pass=$((pass+1)); [[ $VERBOSE -eq 1 ]] && echo "PASS  $base m$m"
        else
            fail=$((fail+1)); failed_names+=("$base m$m")
            echo "DIFF  $base m$m"
            [[ $VERBOSE -eq 1 ]] && diff "$ref" "$out" | head -20
        fi
    done
done
echo "-----------------------------------------------------------------------"
echo "KW-STATIC GATE: $pass PASS / $((pass+fail)) total   (mode=$MODE, SCRIP_KW_STATIC=${SCRIP_KW_STATIC:-unset/legacy})"
if [[ $fail -gt 0 ]]; then
    echo "FAILING: ${failed_names[*]}"
    echo "GATE RED — see FINDING-2026-08-19-s146-KW1-census-keyword-truth-table.md"
    exit 1
fi
echo "GATE GREEN"
exit 0
