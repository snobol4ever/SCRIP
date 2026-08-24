#!/usr/bin/env bash
# bench_prolog_perf.sh — 4-way Prolog PERFORMANCE comparison (wall-clock, median of 3).
# Columns: GNU (gprolog --consult-file, bytecode consult mode)
#          SWI (swipl -q -g halt, script mode)
#          m3  (scrip --run: in-process x86 codegen + execution, end-to-end)
#          m4r (precompiled scrip --compile binary, execution only)
#          m4c (scrip --compile + as + gcc, one-shot compile+link time)
# Programs are the corpus signature benches (single-shot; no internal timing loop),
# so small entries are startup-dominated — comparable across engines, stated up front.
# Only programs whose 4-way CORRECTNESS is consensus-green are timed; a FAIL in any
# engine prints SKIP for that row (timing wrong answers is meaningless).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"; T="${TIMEOUT:-60}"; N="${RUNS:-3}"
ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || true
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED-TO-GRADE bench corpus missing: $B"; exit 2; }
command -v gprolog >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE gprolog absent"; exit 2; }
command -v swipl   >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE swipl absent"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
now_ms() { echo $(( $(date +%s%N) / 1000000 )); }
# median_ms <cmd...> : run N times, print median wall ms (captures stdout to $W/last.out)
median_ms() {
  local times=() t0 t1 i
  for i in $(seq 1 "$N"); do
    t0=$(now_ms); (cd "$W" && timeout "$T" "$@" </dev/null >"$W/last.out" 2>/dev/null); t1=$(now_ms)
    times+=( $((t1 - t0)) )
  done
  printf '%s\n' "${times[@]}" | sort -n | awk -v n="$N" 'NR==int((n+1)/2){print; exit}'
}
printf "%-14s %8s %8s %8s %8s %8s\n" BENCH GNU_ms SWI_ms m3_ms m4r_ms m4c_ms
for pl in "$B"/*.pl; do
  s=$(basename "${pl%.pl}"); exp="${pl%.pl}.expected"
  [ -f "$exp" ] || continue
  want=$(cat "$exp")
  # correctness pre-flight (one run per engine); any FAIL => SKIP row
  go=$(cd "$W" && timeout "$T" gprolog --consult-file "$pl" --query-goal halt 2>/dev/null </dev/null \
       | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined')
  so=$(cd "$W" && timeout "$T" swipl -q -g halt "$pl" 2>/dev/null </dev/null | head -200)
  m3o=$(cd "$W" && timeout "$T" "$SCRIP" --run "$pl" </dev/null 2>/dev/null | head -200)
  if [ "$go" != "$want" ] || [ "$so" != "$want" ] || [ "$m3o" != "$want" ]; then
    printf "%-14s %8s %8s %8s %8s %8s\n" "$s" SKIP SKIP SKIP SKIP SKIP; continue
  fi
  # m4: compile+link once (timed), then time binary alone
  c0=$(now_ms)
  asm=$(cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 "$pl" </dev/null 2>/dev/null)
  printf '%s\n' "$asm" > "$W/$s.s"
  (cd "$W" && as --64 -o "$s.o" "$s.s" 2>/dev/null) \
    && gcc -no-pie -o "$W/$s.bin" "$W/$s.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null
  c1=$(now_ms); m4c=$((c1 - c0))
  m4o=$(cd "$W" && timeout "$T" ./$s.bin </dev/null 2>/dev/null | head -200)
  if [ "$m4o" != "$want" ]; then
    printf "%-14s %8s %8s %8s %8s %8s\n" "$s" SKIP SKIP SKIP SKIP SKIP; continue
  fi
  gnu=$(median_ms gprolog --consult-file "$pl" --query-goal halt)
  swi=$(median_ms swipl -q -g halt "$pl")
  m3=$(median_ms "$SCRIP" --run "$pl")
  m4r=$(median_ms ./$s.bin)
  printf "%-14s %8s %8s %8s %8s %8s\n" "$s" "$gnu" "$swi" "$m3" "$m4r" "$m4c"
done
echo
echo "ENGINES: GNU=$(gprolog --version </dev/null 2>&1 | head -1)"
echo "         SWI=$(swipl --version </dev/null 2>&1 | head -1)"
echo "METHOD: wall-clock median of $N; GNU=consult mode; SWI=script mode; m3=--run end-to-end"
echo "        (in-process codegen+exec); m4r=precompiled binary exec only; m4c=compile+as+gcc."
