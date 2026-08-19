#!/usr/bin/env bash
# bench_prolog_vanroy.sh — PL-SPEED-0 MEASUREMENT RAIL (van Roy loop harness).
# Per-iteration compute (ms) for GNU (gprolog consult) · SWI (swipl) · SCRIP m3
# (--run) · SCRIP m4 (compiled binary), each bench looped via the PROVEN
# rename-wrapper shape (FINDING-2026-07-10):
#   sed 's/^main :-/bench__main :-/'  +  appended  main :- l__(N).  recursion loop.
# N is AUTO-RANGED PER ENGINE (doubling until wall-floor >= MIN_WALL_MS, cap NMAX),
# so fast engines get big N (precision) and slow ones small N (bounded wall);
# per-iteration ms = (wall - engine startup floor) / N, which is N-independent at
# steady state. The gprolog-calibrated wrapper is left in VANROY_DIR as the
# checked-in driver artifact. queensn (broken) auto-SKIPs via consensus pre-flight.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"
V="${VANROY_DIR:-$S4E/corpus/benchmarks/prolog/vanroy}"
T="${TIMEOUT:-240}"; MIN_WALL_MS="${MIN_WALL_MS:-300}"; NMAX="${NMAX:-65536}"
ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || true
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -f "$RT/libscrip_rt.so" ] || { echo "SKIP libscrip_rt.so not built"; exit 0; }
[ -d "$B" ] || { echo "SKIP bench corpus missing: $B"; exit 0; }
command -v gprolog >/dev/null 2>&1 || { echo "SKIP gprolog absent"; exit 0; }
command -v swipl   >/dev/null 2>&1 || { echo "SKIP swipl absent"; exit 0; }
mkdir -p "$V"; W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
now_ms() { echo $(( $(date +%s%N) / 1000000 )); }
wall_ms() { local t0 t1; t0=$(now_ms); (cd "$W" && timeout -k 5 "$T" "$@" </dev/null >/dev/null 2>&1); t1=$(now_ms); echo $((t1 - t0)); }
med5() { local a=() i; for i in 1 2 3 4 5; do a+=( "$(wall_ms "$@")" ); done; printf '%s\n' "${a[@]}" | sort -n | sed -n 3p; }
mkwrap() { # $1=src $2=N $3=out — FAILURE-DRIVEN loop: backtracking reclaims the
  # global/copy stacks every iteration (gprolog has NO heap GC — a recursion loop
  # silently aborts on allocation-heavy benches after tens of iterations; measured
  # 2026-07-18: fib x1000 recursion-loop stopped at 42 iters). NO once/1 fence:
  # SCRIP's once does not hold under external redo (measured 2026-07-18, queens_8
  # x50 emitted 1111 lines vs GNU 50 — filed) — so each iteration enumerates ALL
  # solutions via plain fail; identical work on every engine, still comparable.
  { sed 's/^main :-/bench__main :-/' "$1"
    printf 'main :- l__(%d).\nl__(N__) :- between(1, N__, _), bench__main, fail.\nl__(_).\n' "$2"
  } > "$3"
}
# auto_range <src.pl> <floor> <engine> [m4bin-prefix] -> "periter N" or "DNF N"
auto_range() {
  local src="$1" fl="$2" eng="$3" s="$4" N=1 w=0 c
  while :; do
    mkwrap "$src" "$N" "$W/ar.pl"
    case "$eng" in
      gnu) w=$(wall_ms gprolog --consult-file "$W/ar.pl" --query-goal halt) ;;
      swi) w=$(wall_ms swipl -q -g halt "$W/ar.pl") ;;
      m3)  w=$(wall_ms "$SCRIP" --run "$W/ar.pl") ;;
      m4)  asm=$(cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 ar.pl </dev/null 2>/dev/null)
           printf '%s\n' "$asm" > "$W/ar.s"
           (cd "$W" && as --64 -o ar.o ar.s 2>/dev/null && gcc -no-pie -o ar.bin ar.o "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null) || { echo "DNF $N"; return; }
           w=$(wall_ms ./ar.bin) ;;
    esac
    [ "$w" -ge $((T*1000-500)) ] && { echo "DNF $N"; return; }
    c=$((w - fl)); [ "$c" -lt 0 ] && c=0
    if [ "$c" -ge "$MIN_WALL_MS" ] || [ "$N" -ge "$NMAX" ]; then
      awk -v c="$c" -v n="$N" 'BEGIN{printf "%.4f %d", c/n, n}'; return
    fi
    N=$((N*4)); [ "$N" -gt "$NMAX" ] && N=$NMAX
  done
}
# --- startup floors (warm once, then median of 5 on an empty program) ---
printf ':- initialization(main).\nmain.\n' > "$W/empty.pl"
gprolog --consult-file "$W/empty.pl" --query-goal halt >/dev/null 2>&1
swipl -q -g halt "$W/empty.pl" >/dev/null 2>&1
FG=$(med5 gprolog --consult-file "$W/empty.pl" --query-goal halt)
FS=$(med5 swipl -q -g halt "$W/empty.pl"); F3=$(med5 "$SCRIP" --run "$W/empty.pl")
asm=$(cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 empty.pl </dev/null 2>/dev/null); printf '%s\n' "$asm" > "$W/empty.s"
(cd "$W" && as --64 -o empty.o empty.s 2>/dev/null && gcc -no-pie -o empty.bin empty.o "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null)
F4=$(med5 ./empty.bin)
echo "floors_ms: GNU=$FG SWI=$FS m3=$F3 m4=$F4  (median of 5 after warm-up, empty program)"
echo
printf "%-12s %9s %9s %9s %9s %9s %9s   %s\n" BENCH GNU_it SWI_it m3_it m4_it m4/GNU m4/SWI "N(g/s/3/4)"
declare -a G_RATIO=() S_RATIO=()
for pl in "$B"/*.pl; do
  s=$(basename "${pl%.pl}"); exp="${pl%.pl}.expected"; [ -f "$exp" ] || continue
  want=$(cat "$exp")
  go=$(cd "$W" && timeout 60 gprolog --consult-file "$pl" --query-goal halt 2>/dev/null </dev/null \
       | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined')
  so=$(cd "$W" && timeout 60 swipl -q -g halt "$pl" 2>/dev/null </dev/null | head -200)
  m3o=$(cd "$W" && timeout 60 "$SCRIP" --run "$pl" </dev/null 2>/dev/null | head -200)
  if [ "$go" != "$want" ] || [ "$so" != "$want" ] || [ "$m3o" != "$want" ]; then
    printf "%-12s %9s %9s %9s %9s %9s %9s   %s\n" "$s" SKIP SKIP SKIP SKIP - - -; continue; fi
  read gi NG <<< "$(auto_range "$pl" "$FG" gnu)"
  read si NS <<< "$(auto_range "$pl" "$FS" swi)"
  read m3i N3 <<< "$(auto_range "$pl" "$F3" m3)"
  read m4i N4 <<< "$(auto_range "$pl" "$F4" m4 "$s")"
  mkwrap "$pl" "$NG" "$V/$s.pl"   # checked-in driver = gprolog-calibrated wrapper
  rg=-; rs=-
  if [ "$m4i" != DNF ] && [ "$gi" != DNF ]; then
    rg=$(awk -v a="$m4i" -v b="$gi" 'BEGIN{ if(b<=0.0001)b=0.0001; printf "%.2f", a/b }'); G_RATIO+=("$rg"); fi
  if [ "$m4i" != DNF ] && [ "$si" != DNF ]; then
    rs=$(awk -v a="$m4i" -v b="$si" 'BEGIN{ if(b<=0.0001)b=0.0001; printf "%.2f", a/b }'); S_RATIO+=("$rs"); fi
  printf "%-12s %9s %9s %9s %9s %9s %9s   %s\n" "$s" "$gi" "$si" "$m3i" "$m4i" "$rg" "$rs" "$NG/$NS/$N3/$N4"
done
echo
gm() { [ $# -eq 0 ] && { printf '%s' -; return; }; printf '%s\n' "$@" | awk '{s+=log($1); n++} END{printf "%.2f", exp(s/n)}'; }
echo "GEOMEAN m4/GNU = $(gm "${G_RATIO[@]}")   m4/SWI = $(gm "${S_RATIO[@]}")   (over non-DNF rows)"
echo "ENGINES: GNU=$(gprolog --version </dev/null 2>&1 | head -1)"
echo "         SWI=$(swipl --version </dev/null 2>&1 | head -1)"
echo "METHOD: per-iteration ms = (wall - startup floor)/N; N auto-ranged per engine"
echo "        (x4 until compute >= ${MIN_WALL_MS}ms, cap ${NMAX}); loop wrapper (gprolog N)"
echo "        regenerated into $V; timeout ${T}s => DNF."
