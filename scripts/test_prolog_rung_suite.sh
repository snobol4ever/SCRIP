#!/usr/bin/env bash
# scripts/test_prolog_rung_suite.sh — Prolog rung ladder runner.
# Self-contained. Run from anywhere with no env vars.
# Usage: bash scripts/test_prolog_rung_suite.sh [--rung RUNG] [--mode MODE] [--scrip PATH] [--corpus PATH]
#
# Runs the Prolog rung corpus against scrip and reports PASS/FAIL vs .expected files.
# MODE selects which engine path:
#   interp  (Mode 2, --interp, default)  — reference path
#   run     (Mode 3, --run)              — native codegen/linear
#   compile (Mode 4, --compile x86)      — emit→assemble→link→exec via run_prolog_via_x86_backend.sh
# Files with a matching .xfail marker are skipped (XFAIL).
#
# GATE-3 source of truth for the Prolog rung ladder. PASS must be >= previous.
#
# Authors: LCherryholmes · Claude Sonnet 4.6 · Claude Opus 4.7

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-/home/claude/corpus/programs/prolog}"
RUNG=""
MODE="interp"

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rung)   RUNG="$2";   shift 2 ;;
        --mode)   MODE="$2";   shift 2 ;;
        --scrip)  SCRIP="$2";  shift 2 ;;
        --corpus) CORPUS="$2"; shift 2 ;;
        *) echo "Usage: $0 [--rung RUNG] [--mode interp|run|compile] [--scrip PATH] [--corpus PATH]" >&2; exit 1 ;;
    esac
done

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip binary not found at $SCRIP" >&2
    exit 0
fi
if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS" >&2
    echo "     clone snobol4ever/corpus to /home/claude/corpus to run this suite" >&2
    exit 0
fi

PASS=0; FAIL=0; XFAIL=0

run_prog() {
    local pl="$1" tmo="$2"
    case "$MODE" in
        interp)  timeout "$tmo" "$SCRIP" --interp "$pl" < /dev/null 2>/dev/null ;;
        run)     timeout "$tmo" "$SCRIP" --run "$pl" < /dev/null 2>/dev/null ;;
        compile) timeout "$tmo" bash "$HERE/run_prolog_via_x86_backend.sh" "$pl" < /dev/null 2>/dev/null ;;
        *) echo "bad mode $MODE" >&2; exit 1 ;;
    esac
}

run_one() {
    local pl="$1"
    local tmo="${2:-8}"
    local exp="${pl%.pl}.expected"
    [ -f "$exp" ] || return 0
    local base="${pl%.pl}"
    local name
    name=$(basename "$pl" .pl)
    if [ -f "${base}.xfail" ]; then
        echo "XFAIL $name"
        XFAIL=$((XFAIL+1))
        return 0
    fi
    local got want
    got=$(run_prog "$pl" "$tmo") || true
    want=$(cat "$exp")
    if [ "$got" = "$want" ]; then
        echo "PASS $name"
        PASS=$((PASS+1))
    else
        echo "FAIL $name"
        echo "  want: $(echo "$want" | tr '\n' '|')"
        echo "  got:  $(echo "$got"  | tr '\n' '|')"
        FAIL=$((FAIL+1))
    fi
}

if [ -n "$RUNG" ]; then
    for pl in "$CORPUS"/${RUNG}_*.pl; do
        [ -f "$pl" ] || continue
        run_one "$pl"
    done
else
    for pl in "$CORPUS"/rung0[1-9]_*.pl \
              "$CORPUS"/rung1[0-9]_*.pl \
              "$CORPUS"/rung2[0-9]_*.pl \
              "$CORPUS"/rung3[0-9]_*.pl \
              "$CORPUS"/rung4[0-9]_*.pl; do
        [ -f "$pl" ] || continue
        run_one "$pl" 8
    done
fi

echo "--- Prolog ($MODE): PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL TOTAL=$((PASS+FAIL+XFAIL)) ---"
[ "$FAIL" -eq 0 ]
