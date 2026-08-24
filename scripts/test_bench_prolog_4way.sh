#!/usr/bin/env bash
# test_bench_prolog_4way.sh — 4-way Prolog benchmark comparison.
# Columns: GNU (gprolog) · SWI (swipl) · m3 (scrip --run, in-process x86 binary)
#          · m4 (scrip --compile x86 -> as+gcc -> exec).
# Each program has a <name>.expected signature (the corpus benchmark convention).
# A cell is PASS iff that engine's stdout equals .expected, else FAIL/FENCE/etc.
# CONSENSUS = all four agree with .expected. The .expected file is itself derived
# from a real Prolog, so PASS means "matches the reference observable semantics."
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
HAVE_GNU=0; command -v gprolog >/dev/null 2>&1 && HAVE_GNU=1
HAVE_SWI=0; command -v swipl   >/dev/null 2>&1 && HAVE_SWI=1
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
printf "%-14s %-7s %-7s %-7s %-7s  %s\n" BENCH GNU SWI m3 m4 "consensus / result"
consensus=0; divergent=0; tot=0
for pl in "$B"/*.pl; do
  s=$(basename "${pl%.pl}"); exp="${pl%.pl}.expected"; tot=$((tot+1))
  [ -f "$exp" ] || { printf "%-14s %-7s\n" "$s" "NO-REF"; continue; }
  want=$(cat "$exp")
  # --- GNU Prolog (gprolog): consult fires :- initialization(main), then halt. ---
  gnu="-"
  if [ "$HAVE_GNU" = 1 ]; then
    go=$(cd "$W" && timeout "$T" gprolog --consult-file "$pl" --query-goal halt 2>/dev/null </dev/null \
         | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined')
    [ "$go" = "$want" ] && gnu=PASS || gnu=FAIL
  fi
  # --- SWI-Prolog (swipl): load file (runs initialization/1), then halt. ---
  swi="-"
  if [ "$HAVE_SWI" = 1 ]; then
    so=$(cd "$W" && timeout "$T" swipl -q -g halt "$pl" 2>/dev/null </dev/null | head -200)
    [ "$so" = "$want" ] && swi=PASS || swi=FAIL
  fi
  # --- SCRIP mode 3 (--run): emit BINARY into RX slab, run in-process. ---
  m3out=$(cd "$W" && timeout "$T" "$SCRIP" --run "$pl" </dev/null 2>"$W/$s.m3err" | head -200)
  if grep -q 'PL-GZ FENCE' "$W/$s.m3err" 2>/dev/null || echo "$m3out" | grep -q 'PL-GZ FENCE'; then m3=FENCE
  elif [ "$m3out" = "$want" ]; then m3=PASS; else m3=FAIL; fi
  # --- SCRIP mode 4 (--compile x86): emit TEXT asm -> as + gcc -> exec. ---
  m4=SKIP
  asm=$(cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 "$pl" </dev/null 2>"$W/$s.m4err")
  if echo "$asm" | grep -qE '^\s*\.(intel_syntax|text|globl)'; then
    printf '%s\n' "$asm" > "$W/$s.s"
    if (cd "$W" && as --64 -o "$s.o" "$s.s" 2>"$W/$s.aserr") \
       && gcc -no-pie -o "$W/$s.bin" "$W/$s.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>"$W/$s.lderr"; then
      m4out=$(cd "$W" && timeout "$T" ./$s.bin </dev/null 2>/dev/null | head -200)
      [ "$m4out" = "$want" ] && m4=PASS || m4=FAIL
    else m4=BUILD; fi
  elif grep -q 'PL-GZ FENCE' "$W/$s.m4err" 2>/dev/null; then m4=FENCE; else m4=NOEMIT; fi
  # --- consensus: every AVAILABLE engine PASS ---
  allpass=1
  [ "$HAVE_GNU" = 1 ] && [ "$gnu" != PASS ] && allpass=0
  [ "$HAVE_SWI" = 1 ] && [ "$swi" != PASS ] && allpass=0
  [ "$m3" != PASS ] && allpass=0
  [ "$m4" != PASS ] && allpass=0
  if [ "$allpass" = 1 ]; then consensus=$((consensus+1)); verdict="CONSENSUS"; else divergent=$((divergent+1)); verdict="DIVERGENT"; fi
  printf "%-14s %-7s %-7s %-7s %-7s  %s %s\n" "$s" "$gnu" "$swi" "$m3" "$m4" "$verdict" "$(echo "$m3out" | head -1 | cut -c1-30)"
done
echo
echo "ENGINES: GNU=$( [ $HAVE_GNU = 1 ] && gprolog --version </dev/null 2>&1 | head -1 || echo absent )"
echo "         SWI=$( [ $HAVE_SWI = 1 ] && swipl --version </dev/null 2>&1 | head -1 || echo absent )"
echo "RESULT: consensus(all-4-agree)=$consensus  divergent=$divergent  total=$tot"
[ "$divergent" -eq 0 ]
