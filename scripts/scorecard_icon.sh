#!/usr/bin/env bash
# scorecard_icon.sh — THE Icon instrument (GOAL-ICON-100.md). Sibling of scorecard_snobol4.sh.
# Usage:
#   bash scripts/scorecard_icon.sh run    [--suites a,b] [--out DIR]   # runs suites, writes "<pass> <total>" per suite file
#   bash scripts/scorecard_icon.sh report [DIR]                        # weighted META = 100 * Σ(w·pass/total) / Σw  (suites present)
# WEIGHTS (Lon's knob — edit here, ONE authority):
#   rungs_m3 25 · rungs_m4 20 · rungs_m3_cells 10 · bench_correct 15 · smoke 10 · crosscheck 5 · gates 10   (Σ=95)
# Grading laws: XFAIL counts in the denominator (end state 293/0/0); bench graded by honest_icon_correctness.sh
# (IDENTICAL only — never rc/non-empty, the s164 rsg lesson); a missing suite file drops from BOTH sums and the
# report says PARTIAL. FALSIFIABILITY: report recomputes from files only — edit one file, META must move.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
SD="$(cd "$(dirname "$0")/.." && pwd)"; CORPUS="${CORPUS:-$S4E/corpus}"; ORACLE_BIN="${ORACLE_BIN:-$S4E/icon-build/bin}"
SUITES_ALL="rungs_m3 rungs_m3_cells rungs_m4 bench_correct smoke crosscheck gates"
w_of(){ case "$1" in rungs_m3) echo 25;; rungs_m4) echo 20;; rungs_m3_cells) echo 10;; bench_correct) echo 15;; smoke) echo 10;; crosscheck) echo 5;; gates) echo 10;; *) echo 0;; esac; }
parse_rungs(){ sed -n 's/.*PASS=\([0-9]*\) FAIL=\([0-9]*\) XFAIL=\([0-9]*\) TOTAL=\([0-9]*\).*/\1 \4/p' | tail -1; }
run_suite(){ local s="$1" out="$2" r=""
  case "$s" in
    rungs_m3)       r="$(cd "$SD" && bash scripts/test_icon_all_rungs.sh 2>/dev/null | parse_rungs)";;
    rungs_m3_cells) r="$(cd "$SD" && SCRIP_ICN_CELLS=1 bash scripts/test_icon_all_rungs.sh 2>/dev/null | parse_rungs)";;
    rungs_m4)       r="$(cd "$SD" && bash scripts/test_icon_x64_all_rungs.sh --corpus "$CORPUS/programs/icon" 2>/dev/null | parse_rungs)";;
    bench_correct)  r="$( { [ -d "$ORACLE_BIN" ] && export PATH="$ORACLE_BIN:$PATH"; cd "$SD" && SCRIPDIR="$SD" timeout 900 bash scripts/honest_icon_correctness.sh 2>/dev/null; } | awk -F'|' '/^[a-z0-9_]+ *\|/{t++; if ($0 ~ /IDENTICAL/) p++} END{print p+0, t+0}')";;
    smoke)          r="$(cd "$SD" && timeout 600 bash scripts/test_smoke_icon.sh 2>/dev/null | sed -n 's/.*PASS=\([0-9]*\) FAIL=\([0-9]*\) *\/ *\([0-9]*\).*/\1 \3/p' | awk '{p+=$1; t+=$2} END{print p+0, t+0}')";;
    crosscheck)     r="$(cd "$SD" && timeout 600 bash scripts/test_crosscheck_icon.sh 2>/dev/null | sed -n 's/.*PASS=\([0-9]*\) FAIL=\([0-9]*\).*/\1 \2/p' | tail -1 | awk '{print $1, $1+$2}')";;
    gates)          local p=0 t=0 g; for g in no_stack one_reg_frame semicolon_required rbp_census_ratchet zk5_gva zcells_gva local_no_nv global_no_nv_m3 scan var; do t=$((t+1)); (cd "$SD" && timeout 240 bash "scripts/test_gate_icn_$g.sh" >/dev/null 2>&1) && p=$((p+1)); done; r="$p $t";;
  esac
  [ -n "$r" ] && echo "$r" > "$out/$s" && echo "  $s: $r" || echo "  $s: NO RESULT (suite errored — file not written)"; }
cmd="${1:-report}"; shift || true
if [ "$cmd" = run ]; then
  OUT="$SD/test-results/scorecard-icon-$(date +%Y%m%d-%H%M%S)"; SEL="$SUITES_ALL"
  while [ $# -gt 0 ]; do case "$1" in --suites) SEL="$(echo "$2" | tr ',' ' ')"; shift 2;; --out) OUT="$2"; shift 2;; *) shift;; esac; done
  mkdir -p "$OUT"; echo "scorecard_icon run → $OUT"; for s in $SEL; do run_suite "$s" "$OUT"; done
  bash "$0" report "$OUT"
elif [ "$cmd" = report ]; then
  DIR="${1:-$(ls -dt "$SD"/test-results/scorecard-icon-* 2>/dev/null | head -1)}"
  [ -d "${DIR:-}" ] || { echo "no scorecard dir"; exit 1; }
  echo "=== ICON SCORECARD ($DIR) ==="; miss=0
  awk_in=""; for s in $SUITES_ALL; do
    if [ -f "$DIR/$s" ]; then read -r p t < "$DIR/$s"; w=$(w_of "$s"); printf "  %-15s %5s / %-5s  w=%-3s %6.1f%%\n" "$s" "$p" "$t" "$w" "$(awk -v p="$p" -v t="$t" 'BEGIN{print (t>0)?100*p/t:0}')"; awk_in="$awk_in$s $p $t $w\n"
    else printf "  %-15s MISSING\n" "$s"; miss=1; fi; done
  printf "%b" "$awk_in" | awk '{n+=$4*(($3>0)?$2/$3:0); d+=$4} END{printf "META SCORE = %.1f  (Σw=%d%s)\n", (d>0)?100*n/d:0, d, ""}'
  [ "$miss" = 1 ] && echo "⚠ PARTIAL — one or more suites missing from this run dir."
else echo "usage: scorecard_icon.sh run|report"; exit 2; fi
