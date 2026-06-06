#!/usr/bin/env bash
# scripts/test_snobol4_all_modes.sh — unified SNOBOL4 all-modes summary runner
# Runs every SNOBOL4 test suite, collects PASS counts for M2/M3/M4,
# and displays a clear progress table with per-mode and total elapsed times.
# Usage: bash scripts/test_snobol4_all_modes.sh
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6  DATE: 2026-06-06

set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
export SCRIP

if [ ! -x "$SCRIP" ]; then echo "SKIP: scrip not built at $SCRIP"; exit 1; fi
export SCRIP RT_DIR
SUITE_TIMEOUT="${SUITE_TIMEOUT:-300}"

# Widths for display
W_NAME=38; W_NUM=7

sep() { printf '%*s\n' "$((W_NAME+W_NUM*7+10))" '' | tr ' ' '-'; }

header() {
    printf "%-${W_NAME}s  %${W_NUM}s  %${W_NUM}s  %${W_NUM}s  %${W_NUM}s  %8s  %8s  %8s  %9s\n" \
        "Suite" "M2 PASS" "M3 PASS" "M4 PASS" "TOTAL" "Time M2" "Time M3" "Time M4" "Elapsed"
}

# parse_results <output_text> → sets P2 P3 P4 TOTAL TM2 TM3 TM4 TELAPSED
_num() { printf '%s\n' "$1" | grep -oE '[0-9]+' | tail -1; }
parse_results() {
    local out="$1"
    # mode-2/3/4 lines: "mode-2 (--interp):  PASS=7 FAIL=0"
    P2=$(_num "$(printf '%s\n' "$out" | grep 'mode-2' | grep -oE 'PASS=[0-9]+' | head -1)"); P2=${P2:-0}
    P3=$(_num "$(printf '%s\n' "$out" | grep 'mode-3' | grep -oE 'PASS=[0-9]+' | head -1)"); P3=${P3:-0}
    P4=$(_num "$(printf '%s\n' "$out" | grep 'mode-4' | grep -oE 'PASS=[0-9]+' | head -1)"); P4=${P4:-0}
    TOTAL=$(_num "$(printf '%s\n' "$out" | grep -oE '\([0-9]+ total\)' | head -1)"); TOTAL=${TOTAL:-0}
    # pat rung suite: "PASS-M2=18 FAIL-M2=1 ..."
    if [ "$P2" -eq 0 ] && printf '%s\n' "$out" | grep -q 'PASS-M2='; then
        local rung; rung=$(printf '%s\n' "$out" | grep 'PASS-M2=')
        P2=$(_num "$(printf '%s\n' "$rung" | grep -oE 'PASS-M2=[0-9]+')"); P2=${P2:-0}
        P3=$(_num "$(printf '%s\n' "$rung" | grep -oE 'PASS-M3=[0-9]+')"); P3=${P3:-0}
        P4=$(_num "$(printf '%s\n' "$rung" | grep -oE 'PASS-M4=[0-9]+')"); P4=${P4:-0}
        TOTAL=$((P2+P3+P4))
    fi
    # crosscheck / smoke_run: "  --interp  PASS=N" lines
    if [ "$P2" -eq 0 ] && printf '%s\n' "$out" | grep -q -- '--interp.*PASS='; then
        P2=$(_num "$(printf '%s\n' "$out" | grep -- '--interp' | grep -oE 'PASS=[0-9]+' | head -1)"); P2=${P2:-0}
        P3=$(_num "$(printf '%s\n' "$out" | grep -- '--run'    | grep -oE 'PASS=[0-9]+' | head -1)"); P3=${P3:-0}
        P4=$(_num "$(printf '%s\n' "$out" | grep -- '--compile'| grep -oE 'PASS=[0-9]+' | head -1)"); P4=${P4:-0}
        TOTAL=$((P2+P3+P4))
    fi
    # TIME line: "TIME M2=Xs M3=Xs M4=Xs TOTAL=Xs"
    local tl; tl=$(printf '%s\n' "$out" | grep '^TIME M2=' | head -1)
    TM2=$(_num "$(printf '%s\n' "$tl" | grep -oE 'M2=[0-9]+')"); TM2=${TM2:-0}
    TM3=$(_num "$(printf '%s\n' "$tl" | grep -oE 'M3=[0-9]+')"); TM3=${TM3:-0}
    TM4=$(_num "$(printf '%s\n' "$tl" | grep -oE 'M4=[0-9]+')"); TM4=${TM4:-0}
    # TOTAL= is last field; may be M4 slot or direct TOTAL
    TELAPSED=$(_num "$(printf '%s\n' "$tl" | grep -oE 'TOTAL=[0-9]+')"); TELAPSED=${TELAPSED:-0}
    # crosscheck outputs single "PASS=N" total (all modes must agree)
    if [ "$P2" -eq 0 ] && printf '%s\n' "$out" | grep -qE '^PASS=[0-9]+ FAIL='; then
        local cp; cp=$(_num "$(printf '%s\n' "$out" | grep -oE '^PASS=[0-9]+' | head -1)")
        P2=${cp:-0}; P3=${P2}; P4=${P2}
    fi
    [ "$TOTAL" -eq 0 ] && [ "$((P2+P3+P4))" -gt 0 ] && TOTAL=$((P2+P3+P4))
}

run_suite() {
    local label="$1" script="$2"
    printf "  %-${W_NAME}s  running..." "$label"
    local T0=$SECONDS
    local out rc
    out=$(timeout "$SUITE_TIMEOUT" bash "$HERE/$script" 2>/dev/null) || rc=$?
    local WALL=$((SECONDS-T0))
    parse_results "$out"
    # If TELAPSED came from script use it; else use wall time
    [ "$TELAPSED" -eq 0 ] && TELAPSED=$WALL
    printf '\r  %-'"${W_NAME}s  %${W_NUM}d  %${W_NUM}d  %${W_NUM}d  %${W_NUM}d  %7ds  %7ds  %7ds  %8ds\n" \
        "$label" "$P2" "$P3" "$P4" "$TOTAL" "$TM2" "$TM3" "$TM4" "$TELAPSED"
    # Accumulate totals
    SUM_P2=$((SUM_P2+P2)); SUM_P3=$((SUM_P3+P3)); SUM_P4=$((SUM_P4+P4))
    SUM_TOT=$((SUM_TOT+TOTAL))
    SUM_TM2=$((SUM_TM2+TM2)); SUM_TM3=$((SUM_TM3+TM3)); SUM_TM4=$((SUM_TM4+TM4))
    SUM_WALL=$((SUM_WALL+WALL))
}

# ── Suites to run ─────────────────────────────────────────────────────────────
SUM_P2=0; SUM_P3=0; SUM_P4=0; SUM_TOT=0
SUM_TM2=0; SUM_TM3=0; SUM_TM4=0; SUM_WALL=0
T_ALL=$SECONDS

echo ""
echo "SNOBOL4 all-modes test summary  $(date '+%Y-%m-%d %H:%M:%S')"
sep
header
sep

run_suite "smoke"                           "test_smoke_snobol4.sh"
run_suite "pat_rung_suite (038-057)"        "test_snobol4_pat_rung_suite.sh"
run_suite "broad_corpus+beauty (M2+M3)"    "test_interp_broad_corpus_and_beauty.sh"
run_suite "crosscheck (3-mode agree)"       "test_crosscheck_snobol4.sh"
# On-demand only (slow; run manually when needed):
#   bash scripts/test_smoke_snobol4_run.sh     (~2 min, M2+M3+M4 on 262 crosscheck programs)
#   bash scripts/test_csnobol4_budne_suite.sh  (~2 min, M2+M3 on 126 Budne programs)

sep
T_ELAPSED=$((SECONDS-T_ALL))
printf "  %-${W_NAME}s  %${W_NUM}d  %${W_NUM}d  %${W_NUM}d  %${W_NUM}d  %7ds  %7ds  %7ds  %8ds\n" \
    "TOTAL" "$SUM_P2" "$SUM_P3" "$SUM_P4" "$SUM_TOT" "$SUM_TM2" "$SUM_TM3" "$SUM_TM4" "$T_ELAPSED"
sep
echo ""
printf "  M2 (--interp):  %d passing\n" "$SUM_P2"
printf "  M3 (--run):     %d passing\n" "$SUM_P3"
printf "  M4 (--compile): %d passing\n" "$SUM_P4"
printf "  Elapsed: %ds total  (M2 %ds  M3 %ds  M4 %ds)\n" \
    "$T_ELAPSED" "$SUM_TM2" "$SUM_TM3" "$SUM_TM4"
echo ""
