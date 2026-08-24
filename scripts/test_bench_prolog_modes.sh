#!/usr/bin/env bash
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"; T="${TIMEOUT:-30}"
# Deep-recursion benches (fib/tak/meta_qsort) nest one C frame per Prolog call
# (rt_proc_call_gen_h trampoline; real fix = PL-SPEED-3/7 DET no-C-frame spine).
# Until that lands, raise the soft C-stack limit so they run to completion.
ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || true
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED-TO-GRADE bench corpus missing: $B"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
printf "%-16s %-8s %-8s %-8s  %s\n" BENCH m3 m4 ORACLE "result/status"
ok=0; fence=0; fail=0; tot=0
for pl in "$B"/*.pl; do
  s=$(basename "${pl%.pl}"); exp="${pl%.pl}.expected"; tot=$((tot+1))
  [ -f "$exp" ] || { printf "%-16s %-8s\n" "$s" "NO-REF"; continue; }
  want=$(cat "$exp")
  # mode 3 (--run): EMIT BINARY -> RX slab, in-process.
  m3out=$(cd "$W" && timeout "$T" "$SCRIP" --run "$pl" </dev/null 2>"$W/$s.m3err" | head -50)
  if grep -q 'PL-GZ FENCE' "$W/$s.m3err" 2>/dev/null || echo "$m3out" | grep -q 'PL-GZ FENCE'; then m3=FENCE
  elif [ "$m3out" = "$want" ]; then m3=PASS; else m3=FAIL; fi
  # mode 4 (--compile x86): EMIT TEXT -> as+gcc -> exec, separate process.
  m4=SKIP
  asm=$(cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 "$pl" </dev/null 2>"$W/$s.m4err")
  if echo "$asm" | grep -qE '^\s*\.(intel_syntax|text|globl)'; then
    printf '%s\n' "$asm" > "$W/$s.s"
    if (cd "$W" && as --64 -o "$s.o" "$s.s" 2>"$W/$s.aserr") \
       && gcc -no-pie -o "$W/$s.bin" "$W/$s.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>"$W/$s.lderr"; then
      m4out=$(cd "$W" && timeout "$T" ./$s.bin </dev/null 2>/dev/null | head -50)
      if [ "$m4out" = "$want" ]; then m4=PASS; else m4=FAIL; fi
    else m4=BUILD; fi
  elif grep -q 'PL-GZ FENCE' "$W/$s.m4err" 2>/dev/null; then m4=FENCE; else m4=NOEMIT; fi
  # oracle cross-check (informational): does a real prolog agree with .expected?
  orc="-"
  if command -v gprolog >/dev/null 2>&1; then
    go=$(timeout "$T" gprolog --consult-file "$pl" --query-goal halt 2>/dev/null </dev/null \
         | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined')
    [ "$go" = "$want" ] && orc=ok || orc=DIFF
  fi
  status="$m3out"; [ "$m3" = FENCE ] && status="(pl_gz_admit fence)"
  [ "$m3" = PASS ] && [ "$m4" = PASS ] && ok=$((ok+1))
  { [ "$m3" = FENCE ] || [ "$m4" = FENCE ] || [ "$m4" = NOEMIT ]; } && fence=$((fence+1))
  { [ "$m3" = FAIL ] || [ "$m4" = FAIL ] || [ "$m4" = BUILD ]; } && fail=$((fail+1))
  printf "%-16s %-8s %-8s %-8s  %s\n" "$s" "$m3" "$m4" "$orc" "$(echo "$status" | cut -c1-44)"
done
echo; echo "RESULT: green(m3&m4)=$ok  frontier(fenced)=$fence  broken=$fail  total=$tot"
[ "$fail" -eq 0 ]
