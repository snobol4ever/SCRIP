#!/usr/bin/env bash
# util_bench_snobol4_engines.sh — run the 16-benchmark SNOBOL4 suite under ANY external engine,
# verifying output against .ref (ms: lines ignored) and normalizing self-reported timing to ms.
#
# Usage: util_bench_snobol4_engines.sh <label> <timeout_s> <divisor_to_ms> <cmd...>
#   <cmd...> is invoked as: cmd <bench.sno>  (stdin=/dev/null)
#
# Known engines + divisors (TIME() units differ per engine — measured 2026-07-10, s13):
#   official spitbol/x64:      1000000   .../official/spitbol-x64/sbl -b        (TIME() diffs = ns)
#   fork snobol4ever/x64:      1         /home/claude/x64/bin/sbl -b            (patched: CPU-time ms)
#   official/fork csnobol4:    1         .../official/csnobol4/snobol4 -f       (ms)
# SCRIP itself: use test_bench_snobol4_modes.sh (mode-4 compile+run vs the same .ref set).
# Build the official engines first: scripts/build_official_oracles.sh
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"
[ $# -ge 4 ] || { sed -n '2,12p' "$0"; exit 2; }
L="$1"; T="$2"; DIV="$3"; shift 3
printf "%-22s %-7s %10s %12s\n" "BENCH($L)" STATUS "wall(ms)" "self(ms)"
ok=0; fail=0; crash=0
for sno in "$B"/*.sno; do
  s=$(basename "${sno%.sno}"); ref="${sno%.sno}.ref"
  t0=$(date +%s.%N); out=$(timeout "$T" "$@" "$sno" </dev/null 2>/tmp/_be.err); rc=$?; t1=$(date +%s.%N)
  wall=$(awk "BEGIN{printf \"%.0f\",($t1-$t0)*1000}")
  raw=$(echo "$out" | grep -i 'ms:' | head -1 | sed -E 's/.*[Mm][Ss]:[[:space:]]*//' | awk '{print $1}')
  ms="-"; [ -n "$raw" ] && ms=$(awk "BEGIN{printf \"%.1f\", $raw/$DIV}" 2>/dev/null)
  if [ $rc -ne 0 ]; then st=CRASH; crash=$((crash+1)); ms="rc=$rc"
  elif [ -f "$ref" ] && ! diff -q <(echo "$out" | grep -vi 'ms:') <(grep -vi 'ms:' "$ref") >/dev/null 2>&1; then st=FAIL; fail=$((fail+1))
  else st=OK; ok=$((ok+1)); fi
  printf "%-22s %-7s %10s %12s\n" "$s" "$st" "$wall" "$ms"
done
echo "RESULT($L): OK=$ok FAIL=$fail CRASH=$crash"
[ "$crash" -eq 0 ] && [ "$fail" -eq 0 ]
