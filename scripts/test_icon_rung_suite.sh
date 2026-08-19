#!/usr/bin/env bash
# scripts/test_icon_rung_suite.sh — Icon rung ladder runner, ALL THREE MODES (GOAL-ICON-BB discipline).
# Self-contained. Run from anywhere with no env vars.
# Usage: bash scripts/test_icon_rung_suite.sh [--rung RUNG] [--mode MODE] [--scrip PATH] [--corpus PATH]
#
# GOAL-ICON-BB mandates running ALL modes on every gate run (the "ALWAYS TEST ALL THREE MODES" policy,
# adopted from GOAL-PROLOG-BB's session-sync three-mode stepping). With no --mode (or --mode all, the
# DEFAULT) every corpus program is run through all three engine paths against its .expected:
#   interp  (Mode 2, --run)                 — reference oracle — HARD GATE (PASS must be >= previous).
#   run     (Mode 3, --run, stackless native)  — TRACKED. A shape with no native template DECLINES LOUD
#                                                 with the [SMX] banner -> counted DECLINED (NOT a FAIL).
#   compile (Mode 4, --compile --target=x86)   — emit .s -> as -> link out/libscrip_rt.so -> exec.
#                                                 TRACKED, same [SMX] -> DECLINED rule.
# A mode-3/4 run whose stderr carries the [SMX] decline banner is reported DECLINED (expected mid-Ground
# -Zero, NOT FAIL) and auto-resumes counting toward PASS the moment that box family gets a native template.
# This is the Icon twin of test_prolog_rung_suite.sh; the [SMX]->DECLINED mechanism is identical.
# Pass --mode interp|run|compile to run a single mode. This is the THREE-MODE source of truth for the Icon
# rung ladder (test_icon_all_rungs.sh remains the mode-2-only category-tally view).
#
# Authors: LCherryholmes · Jeffrey Cooper M.D. · Claude Opus 4.8
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
RT_SO="${RT_SO:-$ROOT/out/libscrip_rt.so}"
CORPUS="${CORPUS:-$S4E/corpus/programs/icon}"
RUNG=""
MODE="all"                              # DEFAULT: run all three modes
SMX_SIG='\[SMX\]'                       # the loud-decline banner — same token Prolog uses

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
    echo "SKIP scrip binary not found at $SCRIP — run scripts/build_scrip.sh" >&2
    exit 0
fi
if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS" >&2
    echo "     clone snobol4ever/corpus to $S4E/corpus to run this suite" >&2
    exit 0
fi

OUTDIR="$(dirname "$RT_SO")"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# run a single program in a given mode, echoing its stdout; stderr captured separately to /tmp banner probe.
# For compile: emit .s -> assemble -> link libscrip_rt.so -> run. Any toolchain failure yields empty stdout
# (so a non-[SMX] toolchain failure shows up as a FAIL, exactly as silent-wrong would — honest).
run_prog() {
    local mode="$1" icn="$2" tmo="$3" errf="$4"
    local base name s o bin stdin_file
    name=$(basename "$icn" .icn)
    base="${icn%.icn}"
    stdin_file="${base}.stdin"
    local IN=/dev/null
    [ -f "$stdin_file" ] && IN="$stdin_file"
    case "$mode" in
        interp)  timeout "$tmo" "$SCRIP" --run "$icn" < "$IN" 2>"$errf" ;;
        run)     timeout "$tmo" "$SCRIP" --run    "$icn" < "$IN" 2>"$errf" ;;
        compile)
            s="$WORK/$name.s"; o="$WORK/$name.o"; bin="$WORK/${name}_bin"
            if ! timeout "$tmo" "$SCRIP" --compile --target=x86 "$icn" < /dev/null > "$s" 2>"$errf"; then
                return 1   # emit failed; a loud [SMX] banner in errf still wins (DECLINED) in run_corpus
            fi
            # a loud [SMX] decline prints to stderr and emits no usable .s — surface the banner, no asm step
            if grep -qE "$SMX_SIG" "$errf"; then return 0; fi
            if ! as "$s" -o "$o" 2>>"$errf"; then return 1; fi
            if ! gcc -no-pie "$o" -L"$OUTDIR" -lscrip_rt -Wl,-rpath,"$OUTDIR" -lm -o "$bin" 2>>"$errf"; then return 1; fi
            timeout "$tmo" "$bin" < "$IN" 2>>"$errf"
            ;;
        *) echo "bad mode $mode" >&2; exit 1 ;;
    esac
}

declare -a FILES
collect_files() {
    FILES=()
    if [ -n "$RUNG" ]; then
        local f
        for f in "$CORPUS"/${RUNG}_*.icn; do [ -f "$f" ] && FILES+=("$f"); done
    else
        local f
        for f in "$CORPUS"/rung0[1-9]_*.icn \
                 "$CORPUS"/rung1[0-9]_*.icn \
                 "$CORPUS"/rung2[0-9]_*.icn \
                 "$CORPUS"/rung3[0-5]_*.icn \
                 "$CORPUS"/rung36_*.icn \
                 "$CORPUS"/rung37_*.icn; do
            [ -f "$f" ] && FILES+=("$f")
        done
    fi
}

# run the whole collected set in one mode; sets MODE_FAIL=1 on any FAIL
run_corpus() {
    local mode="$1"
    local PASS=0 FAIL=0 XFAIL=0 DECLINED=0
    MODE_FAIL=0
    local icn base name exp got want errf rc
    errf="$WORK/err.txt"
    for icn in "${FILES[@]}"; do
        exp="${icn%.icn}.expected"
        [ -f "$exp" ] || continue
        base="${icn%.icn}"
        name=$(basename "$icn" .icn)
        if [ -f "${base}.xfail" ]; then
            [ "$VERBOSE" = 1 ] && echo "XFAIL $name"
            XFAIL=$((XFAIL+1)); continue
        fi
        : > "$errf"
        got=$(run_prog "$mode" "$icn" 8 "$errf"); rc=$?
        # loud-decline -> DECLINED (expected mid-Ground-Zero, NOT a FAIL). interp never declines.
        if [ "$mode" != interp ] && grep -qE "$SMX_SIG" "$errf"; then
            [ "$VERBOSE" = 1 ] && echo "DECLINED $name"
            DECLINED=$((DECLINED+1)); continue
        fi
        # SUITE-HONESTY (GOAL-ICON-BB 2026-06-03): a nonzero exit without the [SMX] banner is a FAIL in
        # EVERY mode (m2 included), even when stdout happens to match .expected — kills the vacuous pass
        # where an aborting program with empty stdout matched an empty .expected (rung36_jcon_proto).
        if [ "$rc" -ne 0 ]; then
            [ "$VERBOSE" = 1 ] && echo "FAIL $name (rc=$rc)"
            FAIL=$((FAIL+1)); MODE_FAIL=1; continue
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
        echo "--- Icon ($mode): PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL DECLINED=$DECLINED TOTAL=$((PASS+FAIL+XFAIL+DECLINED)) ---"
    else
        echo "--- Icon ($mode): PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL TOTAL=$((PASS+FAIL+XFAIL)) ---"
    fi
}

collect_files
# verbose per-file output only for single-mode runs; the all-modes sweep prints summaries only
VERBOSE=1; [ "$MODE" = "all" ] && VERBOSE=0

HARD_FAIL=0
case "$MODE" in
    all)
        for m in interp run compile; do
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
