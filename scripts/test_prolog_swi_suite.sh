#!/usr/bin/env bash
# test_prolog_swi_suite.sh — run SWI plunit conformance suite under --run and --compile
# Iterates corpus/tests/prolog/swi_tests/test_*.pl, loads each with
# plunit.pl shim + a main wrapper, compares PASS/FAIL per suite against .ref.
#
# Matching: set-based (order-independent, deduped — ignores double-run artefacts).
# Default (no --mode given): runs BOTH modes in one invocation, each printing its
# own "Suite totals: ... mode=X" line; overall exit 0 iff both modes clear their
# 80% coverage gate. Pass --mode to grade a single mode instead (one totals line).
# Options:
#   --verbose       show raw scrip output for failing files
#   --file NAME     run only NAME.pl  (e.g. --file test_bips)
#   --mode MODE     --run | --compile  (default: run both, one line each)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
RT="${HERE}/../out"
CORPUS=$S4E/corpus/tests/prolog
SWIT=$S4E/corpus/packages/prolog/swi_tests
PLUNIT=$CORPUS/plunit.pl
MATCH_PY="${HERE}/util_swi_match.py"
REPORT_PY="${HERE}/util_swi_report.py"
WRAP=$(mktemp /tmp/pl_wrap_XXXXXX.pl)
ACTUAL_TMP=$(mktemp /tmp/pl_actual_XXXXXX.txt)
ASM_TMP=$(mktemp /tmp/pl_asm_XXXXXX.s)
PROG_TMP=$(mktemp /tmp/pl_prog_XXXXXX)
trap 'rm -f "$WRAP" "$ACTUAL_TMP" "$ASM_TMP" "$PROG_TMP"' EXIT

VERBOSE=0; ONLY_FILE=""; MODE=""
while [[ $# -gt 0 ]]; do
    case "$1" in
        --verbose)  VERBOSE=1; shift ;;
        --file)     ONLY_FILE="$2"; shift 2 ;;
        --mode)     MODE="$2"; shift 2 ;;
        --run|--compile) MODE="$1"; shift ;;
        *) echo "Unknown arg: $1"; exit 1 ;;
    esac
done

[ -d "$SWIT" ]   || { echo "⛔ REFUSED-TO-GRADE: $SWIT missing"; exit 2; }
[ -f "$PLUNIT" ] || { echo "⛔ REFUSED-TO-GRADE: $PLUNIT missing"; exit 2; }
[ -x "$SCRIP" ]  || { echo "⛔ REFUSED-TO-GRADE: scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE: libscrip_rt.so not built"; exit 2; }

printf 'main :- run_tests.\n:- initialization(main).\n' > "$WRAP"

# run_one_mode: grades the whole SWIT corpus under one scrip mode flag ("--run"
# or "--compile"), prints its own "Suite totals:" line, returns 1 if that mode's
# coverage is <80% (never aborts the script itself — set -e is suspended for a
# function call under `||`, which is what lets the caller run a second mode after
# the first one's gate check fails).
run_one_mode() {
    local mode="$1"
    local PASS=0 FAIL=0 TOTAL=0

    for f in $(find "$SWIT" -name "*.pl" | sort); do   # RECURSIVE (row swi-tests-subdirs-invisible, Lon direct 2026-08-30): the flat test_*.pl glob saw 18 of 249 -- 21 subdirs + test.pl were structurally invisible
        base=$(basename "$f" .pl)
        ref="$SWIT/${base}.ref"
        [ -f "$ref" ] || continue
        [ -z "$ONLY_FILE" ] || [ "$base" = "$ONLY_FILE" ] || continue

        suite_total=$(wc -l < "$ref")
        TOTAL=$((TOTAL + suite_total))

        if [ "$mode" = "--compile" ]; then
            # m4 is TEXT-emit only: --compile dumps asm to stdout, it does not run
            # anything itself. Same pipeline as test_3way_snobol4.sh: emit .s, gcc
            # assembles+links against libscrip_rt.so, then the LINKED BINARY runs.
            : > "$ACTUAL_TMP"
            if timeout 30 "$SCRIP" --compile "$PLUNIT" "$f" "$WRAP" > "$ASM_TMP" 2>/dev/null \
               && [ -s "$ASM_TMP" ] \
               && gcc -no-pie "$ASM_TMP" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$PROG_TMP" 2>/dev/null; then
                timeout 30 "$PROG_TMP" < /dev/null 2>/dev/null \
                    | grep -E '^(PASS|FAIL|EMPTY) ' > "$ACTUAL_TMP" || true
            fi
        else
            timeout 30 "$SCRIP" "$mode" "$PLUNIT" "$f" "$WRAP" < /dev/null 2>/dev/null \
                | grep -E '^(PASS|FAIL|EMPTY) ' > "$ACTUAL_TMP" || true
        fi

        matched=$(python3 "$MATCH_PY" "$ref" "$ACTUAL_TMP")
        PASS=$((PASS + matched))
        FAIL=$((FAIL + suite_total - matched))

        if [ "$matched" -eq "$suite_total" ]; then
            echo "  PASS $base ($suite_total suite-lines)  [$mode]"
        else
            echo "  FAIL $base  match=$matched/$suite_total  [$mode]"
            python3 "$REPORT_PY" "$ref" "$ACTUAL_TMP"
            if [ "$VERBOSE" -eq 1 ]; then
                echo "  --- raw output ---"
                cat "$ACTUAL_TMP"
                echo "  ---"
            fi
        fi
    done

    echo ""
    echo "Suite totals: PASS=$PASS FAIL=$FAIL TOTAL=$TOTAL  mode=$mode"
    # ⭐ PASS/FAIL/TOTAL are `local` to this function, so they do not exist by the time the script ends.
    # Accumulate the board across modes into a global here rather than making the totals global, which
    # would let a later mode silently overwrite an earlier one and report the last mode as "the suite".
    SWI_BOARD="${SWI_BOARD:+$SWI_BOARD · }$mode $PASS/$TOTAL (FAIL=$FAIL)"
    [ "$TOTAL" -gt 0 ] || { echo "⛔ REFUSED-TO-GRADE: no test files found"; exit 2; }
    local pct=$((PASS * 100 / TOTAL))
    echo "Coverage: ${pct}%  (gate: >=80%)"
    [ "$pct" -ge 80 ]
}

OVERALL_RC=0
if [ -n "$MODE" ]; then
    run_one_mode "$MODE" || OVERALL_RC=1
else
    run_one_mode "--run" || OVERALL_RC=1
    echo ""
    run_one_mode "--compile" || OVERALL_RC=1
fi

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board.
python3 "$HERE/util_score_row.py" write --lang prolog --column vendor --suite SWI \
    --measurer "${S4E_SEAT:-unknown-seat}" --text "${SWI_BOARD:-no mode ran} (\`test_prolog_swi_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
exit "$OVERALL_RC"
