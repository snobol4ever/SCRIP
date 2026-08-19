#!/usr/bin/env bash
# scripts/test_prolog_rung_suite.sh — Prolog rung ladder runner.
# Self-contained. Run from anywhere with no env vars.
# Usage: bash scripts/test_prolog_rung_suite.sh [--rung RUNG] [--mode MODE] [--scrip PATH] [--corpus PATH]
#
# GOAL-PROLOG-BB mandates running ALL modes on every gate run (see GOAL "Testing discipline").
# With no --mode (or --mode all, the DEFAULT) the corpus is run in all three engine paths:
#   interp  (Mode 2, --run)            — reference path      — HARD GATE (PASS must be >= previous).
#   run     (Mode 3, --run)               — native/stackless    — TRACKED (DECLINED until GZ regrows it).
#   compile (Mode 4, --compile x86)       — emit→assemble→link→exec via run_prolog_via_x86_backend.sh
#                                                                — TRACKED (DECLINED until BB-native x86 emit returns).
# A mode whose probe prints the Stack-Machine-eXcision banner is reported DECLINED (expected mid-Ground-Zero)
# and its per-file loop is skipped (no point running 100+ aborts); it auto-resumes the moment the mode regrows.
# Pass --mode interp|run|compile to run a single mode. GATE-3 source of truth for the Prolog rung ladder.
#
# Authors: LCherryholmes · Claude Sonnet 4.6 · Claude Opus 4.7 · Claude Sonnet
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/programs/prolog}"
RUNG=""
MODE="all"                              # DEFAULT: run all three modes
SMX_SIG='\[SMX\]'                       # both decline banners begin with "[SMX]"

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rung)   RUNG="$2";   shift 2 ;;
        --mode)   MODE="$2";   shift 2 ;;
        --scrip)  SCRIP="$2";  shift 2 ;;
        --corpus) CORPUS="$2"; shift 2 ;;
        *) echo "Usage: $0 [--rung RUNG] [--mode all|interp|run|compile] [--scrip PATH] [--corpus PATH]" >&2; exit 1 ;;
    esac
done

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip binary not found at $SCRIP" >&2
    exit 0
fi
if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS" >&2
    echo "     clone snobol4ever/corpus to $S4E/corpus to run this suite" >&2
    exit 0
fi

# run a single program in a given mode, echoing its stdout
run_prog() {
    local mode="$1" pl="$2" tmo="$3"
    case "$mode" in
        interp)  timeout "$tmo" "$SCRIP" --run "$pl" < /dev/null 2>/dev/null ;;
        run)     timeout "$tmo" "$SCRIP" --run    "$pl" < /dev/null 2>/dev/null ;;
        compile) timeout "$tmo" bash "$HERE/run_prolog_via_x86_backend.sh" "$pl" < /dev/null 2>/dev/null ;;
        *) echo "bad mode $mode" >&2; exit 1 ;;
    esac
}

# return 0 if MODE is currently Stack-Machine-eXcised (probe with a trivial program).
# Capture output first, THEN grep: under `set -o pipefail` the aborting scrip (rc 134) would
# otherwise dominate the pipeline status and mask the match.
mode_is_declined() {
    local mode="$1" out
    case "$mode" in
        interp)  return 1 ;;                              # the reference path is never declined
        run)     out=$("$SCRIP" --run "$PROBE" </dev/null 2>&1 || true) ;;
        compile) out=$("$SCRIP" --compile --target=x86 "$PROBE" </dev/null 2>&1 || true) ;;
    esac
    printf '%s' "$out" | grep -qE "$SMX_SIG"
}

# collect the corpus file list into the FILES array
collect_files() {
    FILES=()
    if [ -n "$RUNG" ]; then
        for pl in "$CORPUS"/${RUNG}_*.pl; do [ -f "$pl" ] && FILES+=("$pl"); done
    else
        for pl in "$CORPUS"/rung0[1-9]_*.pl \
                  "$CORPUS"/rung1[0-9]_*.pl \
                  "$CORPUS"/rung2[0-9]_*.pl \
                  "$CORPUS"/rung3[0-9]_*.pl \
                  "$CORPUS"/rung4[0-9]_*.pl \
                  "$CORPUS"/rung5[0-9]_*.pl \
                  "$CORPUS"/rung6[0-9]_*.pl \
                  "$CORPUS"/rung7[0-9]_*.pl \
                  "$CORPUS"/rung8[0-9]_*.pl; do
            [ -f "$pl" ] && FILES+=("$pl")
        done
    fi
}

# run the whole corpus in one mode; sets MODE_FAIL=1 on any FAIL
run_corpus() {
    local mode="$1"
    local PASS=0 FAIL=0 XFAIL=0 DECLINED=0
    MODE_FAIL=0
    if mode_is_declined "$mode"; then
        local pend=0 f
        for f in "${FILES[@]}"; do [ -f "${f%.pl}.expected" ] && pend=$((pend+1)); done
        echo "--- Prolog ($mode): DECLINED (Stack Machine declined) — $pend files pending regrow ---"
        return 0
    fi
    local pl base name exp got want
    for pl in "${FILES[@]}"; do
        exp="${pl%.pl}.expected"
        [ -f "$exp" ] || continue
        base="${pl%.pl}"
        name=$(basename "$pl" .pl)
        if [ -f "${base}.xfail" ]; then
            [ "$VERBOSE" = 1 ] && echo "XFAIL $name"
            XFAIL=$((XFAIL+1)); continue
        fi
        got=$(run_prog "$mode" "$pl" 8) || true
        # Per-file decline: mode-4 is now PARTIALLY live (PLG-9a hello tier emits+runs); a shape
        # the flat tier does not yet cover declines with the [SMX] banner. Per the testing discipline
        # that is DECLINED (pending regrow), NOT a FAIL — identical to test_smoke_prolog.sh.
        if printf '%s' "$got" | grep -qE "$SMX_SIG"; then
            [ "$VERBOSE" = 1 ] && echo "DECLINED $name"
            DECLINED=$((DECLINED+1)); continue
        fi
        want=$(cat "$exp")
        if [ "$got" = "$want" ]; then
            [ "$VERBOSE" = 1 ] && echo "PASS $name"
            PASS=$((PASS+1))
        else
            if [ "$VERBOSE" = 1 ]; then
                echo "FAIL $name"
                echo "  want: $(echo "$want" | tr '\n' '|')"
                echo "  got:  $(echo "$got"  | tr '\n' '|')"
            fi
            FAIL=$((FAIL+1)); MODE_FAIL=1
        fi
    done
    if [ "$DECLINED" -gt 0 ]; then
        echo "--- Prolog ($mode): PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL DECLINED=$DECLINED TOTAL=$((PASS+FAIL+XFAIL+DECLINED)) ---"
    else
        echo "--- Prolog ($mode): PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL TOTAL=$((PASS+FAIL+XFAIL)) ---"
    fi
}

PROBE="$(mktemp /tmp/plprobe_XXXXXX.pl)"
printf ':- initialization(main).\nmain :- write(ok), nl.\n' > "$PROBE"
trap 'rm -f "$PROBE"' EXIT

collect_files
# verbose per-file output only for single-mode runs; the all-modes sweep prints summaries only
VERBOSE=1; [ "$MODE" = "all" ] && VERBOSE=0

HARD_FAIL=0
case "$MODE" in
    all)
        # MODE COUNT CORRECTED (s160): the loop ran `interp run compile` and reported "x3 modes",
        # but run_prog's `interp` and `run` arms are the IDENTICAL command ($SCRIP --run) — SCRIP has
        # exactly TWO modes since 1 and 2 were deleted (REPO-SCRIP.md), so the third arm measured the
        # same path twice and every "164/164 x3" in the goal-file history was really x2.  `run` is
        # still accepted as an explicit alias below; it is only dropped from the sweep.
        for m in interp compile; do
            run_corpus "$m"
            [ "$m" = interp ] && [ "$MODE_FAIL" -ne 0 ] && HARD_FAIL=1
        done
        ;;
    interp|run|compile)
        run_corpus "$MODE"
        [ "$MODE" = interp ] && [ "$MODE_FAIL" -ne 0 ] && HARD_FAIL=1
        ;;
    *) echo "bad mode $MODE" >&2; exit 1 ;;
esac

[ "$HARD_FAIL" -eq 0 ]
