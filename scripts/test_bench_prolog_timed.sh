#!/usr/bin/env bash
# test_bench_prolog_timed.sh -- ANGLE 1 of the Prolog three-angle triangulation (row bench-rivals-prolog,
# mirroring test_bench_snobol4_timed.sh / ARCH-BENCH-CAMPAIGN-README-TABLES.md THREE-ANGLE TRIANGULATION).
#
# Fixed WALL-TIME budget, iterations counted: a live doubling search (N=1,2,4,8,...) picks the largest N
# whose between+fail loop wrapper completes within TIME_BUDGET_MS, then reports iters/s = N / cpu_seconds.
# This N is DERIVED FRESH every run (never read from a committed file) -- the independence property the
# cross-proof needs against angle 2 (bench_prolog_fixed_iter.sh), which instead EXECUTES a pre-committed,
# historical N from corpus/benchmarks/prolog/vanroy/<k>.pl. Same atomic operation (loop N times, measure),
# different SOURCE for N -- live-derived here, historically-committed there.
#
# ⛔ CORRECTNESS GATES THE TIMING (binding, ARCH-BENCH-CAMPAIGN-README-TABLES.md): this script re-verifies
# each kernel's SINGLE-SHOT output against corpus/benchmarks/prolog/bench/<k>.expected on gnu/swi/m3 before
# ever timing it -- a kernel that fails this is SKIPped, never silently timed anyway.
#
# ⛔⭐ THE exit= FIELD IS THE ONLY TRUSTED CRASH SIGNAL (row bench-rivals-prolog, found live 2026-08-27):
# a segfaulting child still writes a well-formed BENCH_RUSAGE line -- grepping stderr TEXT for
# "segmentation"/"core dumped" missed real crashes (the shell's own job-control notice does not reliably
# land in a redirected stderr capture), and a crashed run's tiny cpu_us then read as an implausibly FAST
# measurement. See FINDING-2026-08-27-seat14-prolog-second-call-into-any-user-predicate-crashes-m3-m4.md.
# Every run here is gated on BENCH_RUSAGE's own exit= field, nothing else.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"
T="${TIMEOUT:-30}"; BUDGET_MS="${TIME_BUDGET_MS:-500}"; NMAX="${NMAX:-65536}"
KERNELS="${KERNELS:-}"   # optional allowlist, same convention as bench_prolog_fixed_iter.sh
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED-TO-GRADE bench corpus missing: $B"; exit 2; }
command -v gprolog >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE gprolog absent"; exit 2; }
command -v swipl   >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE swipl absent"; exit 2; }
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build" >&2; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || true

mkwrap() { printf 'main :- l__(%d).\nl__(N__) :- between(1, N__, _), bench__main, fail.\nl__(_).\n' "$2" > "$3.tail"
  sed 's/^main :-/bench__main :-/' "$1" | grep -v '^:- initialization' > "$3.body"
  { echo ':- initialization(main).'; cat "$3.body"; cat "$3.tail"; } > "$3"; }

# one bench_rusage-wrapped run at N iterations; echoes "cpu_us" or "- REASON" -- exit= is the ONLY signal trusted
run1() {
  local eng="$1" pl="$2" out rl xc user sys
  case "$eng" in
    gnu) out=$("$WRAP" timeout "$T" gprolog --consult-file "$pl" --query-goal halt >/dev/null 2>"$W/e.$$") ;;
    swi) out=$("$WRAP" timeout "$T" swipl -q -g halt "$pl" >/dev/null 2>"$W/e.$$") ;;
    m3)  out=$("$WRAP" timeout "$T" "$SCRIP" --run "$pl" >/dev/null 2>"$W/e.$$") ;;
    m4)  local s="$W/$$.s" b="$W/$$.bin"
         if ! (cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 "$pl" </dev/null >"$s" 2>/dev/null) || [ ! -s "$s" ]; then echo "- BUILD-ERR"; return; fi
         if ! (as --64 -o "$W/$$.o" "$s" 2>/dev/null && gcc -no-pie -o "$b" "$W/$$.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null); then echo "- LINKFAIL"; return; fi
         out=$("$WRAP" timeout "$T" "$b" >/dev/null 2>"$W/e.$$") ;;
  esac
  rl=$(grep '^BENCH_RUSAGE:' "$W/e.$$" 2>/dev/null | tail -1)
  [ -n "$rl" ] || { echo "- DNF"; return; }
  xc=$(echo "$rl" | grep -oE 'exit=[0-9-]+' | cut -d= -f2)
  case "$xc" in ''|*[!0-9]*) echo "- CRASH(exit=${xc:-?})"; return ;; esac
  [ "$xc" -ge 128 ] && { echo "- CRASH(signal $((xc-128)))"; return; }
  [ "$xc" -ne 0 ] && { echo "- NONZERO($xc)"; return; }
  user=$(echo "$rl" | grep -oE 'user_us=[0-9]+' | cut -d= -f2); sys=$(echo "$rl" | grep -oE 'sys_us=[0-9]+' | cut -d= -f2)
  echo "$(( ${user:-0} + ${sys:-0} ))"
}
# doubling search: largest N whose cpu time >= BUDGET_MS, cap NMAX. Echoes "N rate" or "N -" + reason on the FIRST N that fails.
search() {
  local eng="$1" src="$2" N=1 cpu
  while :; do
    mkwrap "$src" "$N" "$W/s.pl"
    cpu=$(run1 "$eng" "$W/s.pl")
    case "$cpu" in
      -\ *) echo "$N $cpu"; return ;;   # first failure at this N -- report it, caller decides
    esac
    if [ "$cpu" -ge $((BUDGET_MS*1000)) ] || [ "$N" -ge "$NMAX" ]; then
      awk -v n="$N" -v us="$cpu" 'BEGIN{printf "%d %.4f", n, n/(us/1e6)}'; return
    fi
    N=$((N*4)); [ "$N" -gt "$NMAX" ] && N=$NMAX
  done
}

echo "TIME-BASED PROLOG BENCHMARKS -- angle 1: fixed wall-time budget (${BUDGET_MS}ms), iterations counted via live doubling search"
echo "engines: gnu swi m3 m4   corpus: $B   budget: TIME_BUDGET_MS=$BUDGET_MS cap NMAX=$NMAX   external instrument: tools/bench_rusage"
echo
printf "%-14s %14s %14s %14s %14s  %s\n" BENCHMARK gnu/s swi/s m3/s m4/s check
printf "%-14s %14s %14s %14s %14s  %s\n" "--------------" "--------------" "--------------" "--------------" "--------------" "-----"
tot_ok=0; tot_skip=0
for pl in "$B"/*.pl; do
  [ -e "$pl" ] || continue
  k=$(basename "${pl%.pl}"); exp="${pl%.pl}.expected"
  [ -f "$exp" ] || continue
  if [ -n "$KERNELS" ]; then case " $KERNELS " in *" $k "*) ;; *) continue ;; esac; fi
  want=$(cat "$exp")
  go=$(cd "$W" && timeout 15 gprolog --consult-file "$pl" --query-goal halt 2>/dev/null </dev/null \
       | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined')
  so=$(cd "$W" && timeout 15 swipl -q -g halt "$pl" 2>/dev/null </dev/null | head -200)
  m3o=$(cd "$W" && timeout 15 "$SCRIP" --run "$pl" </dev/null 2>/dev/null | head -200)
  if [ "$go" != "$want" ] || [ "$so" != "$want" ] || [ "$m3o" != "$want" ]; then
    printf "%-14s %14s %14s %14s %14s  %s\n" "$k" SKIP SKIP SKIP SKIP "correctness-fail(single-shot)"; tot_skip=$((tot_skip+1)); continue
  fi
  declare -A R=() C=()
  for eng in gnu swi m3 m4; do
    res=$(search "$eng" "$pl"); n=$(awk '{print $1}' <<<"$res")
    r=$(awk '{print $2}' <<<"$res")
    case "$r" in -) R[$eng]="NA"; C[$eng]="$eng@N=$n:$(cut -d' ' -f3- <<<"$res")" ;; *) R[$eng]="$r" ;; esac
  done
  ckstat=ok; for eng in gnu swi m3 m4; do [ -n "${C[$eng]:-}" ] && ckstat="${C[$eng]}"; done
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1))
  printf "%-14s %14s %14s %14s %14s  %s\n" "$k" "${R[gnu]:-NA}" "${R[swi]:-NA}" "${R[m3]:-NA}" "${R[m4]:-NA}" "$ckstat"
done
echo
echo "CHECK RESULT: measured=$tot_ok correctness-skip=$tot_skip"
[ "$tot_skip" -eq 0 ]
