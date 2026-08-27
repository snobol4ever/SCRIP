#!/usr/bin/env bash
# bench_prolog_fixed_iter.sh -- ANGLE 2 of the Prolog three-angle triangulation (row bench-rivals-prolog,
# mirroring bench_snobol4_fixed_iter.sh / ARCH-BENCH-CAMPAIGN-README-TABLES.md THREE-ANGLE TRIANGULATION).
#
# Run EXACTLY N iterations per kernel (N fixed, COMMITTED, never a wall-clock deadline) and report
# throughput (iterations/s) for gnu (gprolog) / swi (swipl) / m3 / m4. This is the mirror of
# bench_prolog_vanroy.sh (angle 1: LIVE auto-ranged search each run). Angle 2 instead EXECUTES THE
# ALREADY-COMMITTED corpus/benchmarks/prolog/vanroy/<kernel>.pl directly -- each file already carries a
# frozen `main :- l__(N).` from a prior calibration run (bench_prolog_vanroy.sh's own checked-in-artifact
# convention) -- so N here is HISTORICAL data, not derived live, which is the independence property the
# cross-proof needs (angle 1 = live search this run; angle 2 = pre-committed N from a prior run).
#
# ⛔ CORRECTNESS IS NOT RE-VERIFIED HERE. A kernel only gets a vanroy/<k>.pl file if
# bench_prolog_vanroy.sh's own gnu/swi/m3-vs-.expected correctness gate passed at generation time (see
# that script's SKIP logic) -- this script trusts that provenance and gates only on the run completing
# without crash/timeout. Re-verifying correctness on every angle-2 invocation would require re-running
# the ORIGINAL (non-looped) bench/<k>.pl too, which angle 1 already does every time it regenerates
# vanroy/ -- one authority, not two copies of the correctness check.
#
# External CPU time via tools/bench_rusage (elapsed_ns + user_us + sys_us), same instrument angle 3
# (disk telemetry) and the SNOBOL4 triangulator both use -- never self-timing (row
# bench-external-cpu-and-elapsed-clock's law, applied here too).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
V="${VANROY_DIR:-$S4E/corpus/benchmarks/prolog/vanroy}"
T="${TIMEOUT:-60}"
# KERNELS, if set, restricts the run to this space-separated allowlist (basenames, no .pl) instead of
# every *.pl under $V. Needed because $V can carry STALE wrappers for kernels that no longer pass angle
# 1's correctness gate (a regenerate-in-place tool, not auto-pruned -- see bench_prolog_vanroy.sh's own
# header) -- re-timing a kernel currently known to crash wastes wall-clock for no citable number. The
# triangulator passes this explicitly from angle 1's OWN fresh non-SKIP list each run, so the allowlist
# is never hand-maintained or stale itself. Unset (bare use) keeps the old "every file in $V" behaviour.
KERNELS="${KERNELS:-}"
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -d "$V" ] || { echo "⛔ REFUSED-TO-GRADE vanroy corpus missing: $V (run bench_prolog_vanroy.sh first to populate it)"; exit 2; }
command -v gprolog >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE gprolog absent"; exit 2; }
command -v swipl   >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE swipl absent"; exit 2; }
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build" >&2; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || true

get_n() { sed -n 's/^main :- l__(\([0-9]*\))\.$/\1/p' "$1" | head -1; }
rate() { awk -v n="$1" -v us="$2" 'BEGIN{ if (us+0>0) printf "%.4f", n/(us/1e6); else print "NA" }'; }

# one bench_rusage-wrapped run; echoes "cpu_us nivcsw" or "- -" + reason on crash/DNF/missing rusage line
run1() {
  local eng="$1" pl="$2" out rl user sys nivcsw
  case "$eng" in
    gnu) out=$("$WRAP" timeout "$T" gprolog --consult-file "$pl" --query-goal halt >/dev/null 2>"$W/e.$$") ;;
    swi) out=$("$WRAP" timeout "$T" swipl -q -g halt "$pl" >/dev/null 2>"$W/e.$$") ;;
    m3)  out=$("$WRAP" timeout "$T" "$SCRIP" --run "$pl" >/dev/null 2>"$W/e.$$") ;;
    m4)  local s="$W/$$.s" b="$W/$$.bin"
         if ! (cd "$W" && timeout "$T" "$SCRIP" --compile --target=x86 "$pl" </dev/null >"$s" 2>/dev/null) || [ ! -s "$s" ]; then
           echo "- - BUILD-ERR"; return; fi
         if ! (as --64 -o "$W/$$.o" "$s" 2>/dev/null && gcc -no-pie -o "$b" "$W/$$.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null); then
           echo "- - LINKFAIL"; return; fi
         out=$("$WRAP" timeout "$T" "$b" >/dev/null 2>"$W/e.$$") ;;
  esac
  rl=$(grep '^BENCH_RUSAGE:' "$W/e.$$" 2>/dev/null | tail -1)
  if [ -z "$rl" ]; then echo "- - DNF"; return; fi
  # ⛔ THE exit= FIELD IS THE ONLY RELIABLE CRASH SIGNAL -- NOT stderr TEXT (found live, this row: a
  # segfaulting child still prints a well-formed BENCH_RUSAGE line with exit=139, and the shell's own
  # "Segmentation fault" job-control notice does NOT reliably land in a redirected/captured stderr file
  # the way `timeout`'s own "dumped core" message does -- grepping for crash TEXT silently passed a
  # segfaulting run through as a real (implausibly fast) measurement. bench_rusage.c always emits this
  # line and always encodes the true outcome in exit= (128+signal on a signal death), so that field --
  # never text-sniffing -- is what gates whether a cpu_us number is trusted.
  local xc; xc=$(echo "$rl" | grep -oE 'exit=[0-9-]+' | cut -d= -f2)
  case "$xc" in ''|*[!0-9]*) echo "- - CRASH(exit=${xc:-?})"; return ;; esac
  [ "$xc" -ge 128 ] && { echo "- - CRASH(signal $((xc-128)))"; return; }
  [ "$xc" -ne 0 ] && { echo "- - NONZERO($xc)"; return; }
  user=$(echo "$rl" | grep -oE 'user_us=[0-9]+' | cut -d= -f2); sys=$(echo "$rl" | grep -oE 'sys_us=[0-9]+' | cut -d= -f2)
  nivcsw=$(echo "$rl" | grep -oE 'nivcsw=[0-9]+' | cut -d= -f2)
  echo "$(( ${user:-0} + ${sys:-0} )) ${nivcsw:-0}"
}

echo "FIXED-ITERATION PROLOG BENCHMARKS -- angle 2: N fixed per kernel (committed in vanroy/<k>.pl), external cpu time measured"
echo "engines: gnu swi m3 m4   vanroy: $V   external instrument: tools/bench_rusage (user+sys cpu time)"
echo
printf "%-14s %10s %14s %14s %14s %14s  %s\n" BENCHMARK N gnu/s swi/s m3/s m4/s check
printf "%-14s %10s %14s %14s %14s %14s  %s\n" "--------------" "----------" "--------------" "--------------" "--------------" "--------------" "-----"
tot_ok=0; tot_bad=0
for pl in "$V"/*.pl; do
  [ -e "$pl" ] || continue
  k=$(basename "${pl%.pl}")
  if [ -n "$KERNELS" ]; then case " $KERNELS " in *" $k "*) ;; *) continue ;; esac; fi
  N=$(get_n "$pl")
  if [ -z "$N" ]; then printf "%-14s %10s   MISSING l__(N) IN COMMITTED FILE -- regenerate via bench_prolog_vanroy.sh\n" "$k" "-"; tot_bad=$((tot_bad+1)); continue; fi
  ckstat=ok; declare -A RATE=()
  for eng in gnu swi m3 m4; do
    res=$(run1 "$eng" "$pl"); cpu=$(awk '{print $1}' <<<"$res")
    if [ "$cpu" = "-" ]; then RATE[$eng]="NA"; reason=$(cut -d' ' -f3- <<<"$res"); [ "$ckstat" = ok ] && ckstat="$eng:$reason"
    else RATE[$eng]=$(rate "$N" "$cpu"); fi
  done
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1)) || tot_bad=$((tot_bad+1))
  printf "%-14s %10s %14s %14s %14s %14s  %s\n" "$k" "$N" "${RATE[gnu]}" "${RATE[swi]}" "${RATE[m3]}" "${RATE[m4]}" "$ckstat"
done
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad   (bad = crash/DNF/build-fail on at least one engine this run; correctness itself is angle 1's job)"
[ "$tot_bad" -eq 0 ]
