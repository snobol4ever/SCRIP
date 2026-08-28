#!/usr/bin/env bash
# test_bench_pascal_timed.sh -- ANGLE 1 of the Pascal three-angle triangulation (row bench-rivals-raku-pascal,
# mirroring test_bench_prolog_timed.sh / ARCH-BENCH-CAMPAIGN-README-TABLES.md THREE-ANGLE TRIANGULATION).
#
# Fixed WALL-TIME budget, iterations counted: a live doubling search over the kernel's OWN `reps` value
# (fed via stdin -- every kernel here opens with `readln(reps); for rep := 1 to reps do <body>`, so this
# needs no source rewriting the way the Prolog triangulator's mkwrap does) picks the largest reps whose
# single bench_rusage-wrapped run completes within TIME_BUDGET_MS, then reports rate = reps / cpu_seconds.
# ⛔ SCOPED TO THE 7 REPS-CAPABLE KERNELS ONLY (bubble/intmm/perm/queens/quick/sieve/towers). `uplevel2`/
# `uplevel3` have no `reps` knob -- fixed 30000x8000 workload baked into the source -- and need a
# PROCESS-LEVEL repeat mechanism this script does not implement; see corpus/benchmarks/pascal/EXCLUDED.tsv.
# `fbench` SIGSEGVs at compile time (row pascal-fbench-nested-function-self-assign-null-name) and `whet`
# has no committed `.ref` -- both excluded, not silently dropped.
#
# ⛔ CORRECTNESS GATES THE TIMING: every kernel's reps=1 output is verified against its `.ref` before ever
# being timed -- a kernel that fails this is SKIPped, never silently timed anyway. Per the row's own
# measured invariant (hq_C, 2026-08-27: "reps=1,2,3 -> 10414 every time" on the pre-fix `quick`), the
# PRINTED output does not vary with reps -- these kernels loop internally for cost, not for a changing
# answer -- so reps=1's output is the correctness oracle at every reps value, not just reps=1.
#
# ⛔⭐ THE exit= FIELD IS THE ONLY TRUSTED CRASH SIGNAL (same lesson as the Prolog triangulator, row
# bench-rivals-prolog, seat14 FINDING) -- a segfaulting child still writes a well-formed BENCH_RUSAGE
# line; only bench_rusage's own exit= field is trusted, never stderr text-sniffing.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/pascal}"
T="${TIMEOUT:-30}"; BUDGET_MS="${TIME_BUDGET_MS:-800}"; NMAX="${NMAX:-200000}"
KERNELS="${KERNELS:-bubble intmm perm queens quick sieve towers}"   # reps-capable set, measured not guessed (see header)
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED-TO-GRADE pascal corpus missing: $B"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSED: cannot load lib_oracle_flags.sh"; exit 2; }
FPC="$(fpc_bin)" || exit 2
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build" >&2; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT

# fpc needs {$mode objfpc} prepended (default 16-bit `integer` silently corrupts the Hennessy RNG --
# corpus/benchmarks/pascal/README.md, hq_C 2026-08-27) and its own binary compiled once per kernel.
fpc_build() {
  local pas="$1" k out; k=$(basename "${pas%.pas}"); out="$W/fpc_$k"
  [ -x "$out" ] && { printf '%s\n' "$out"; return 0; }
  { printf '{$mode objfpc}\n'; cat "$pas"; } > "$W/fpc_$k.pas"
  "$FPC" -O2 -o"$out" "$W/fpc_$k.pas" >"$W/fpc_$k.buildlog" 2>&1 || return 1
  printf '%s\n' "$out"
}

# one bench_rusage-wrapped run feeding reps via stdin; echoes "cpu_us" or "- REASON"
run1() {
  local eng="$1" k="$2" reps="$3" out rl xc user sys bin
  case "$eng" in
    fpc) bin=$(fpc_build "$B/$k.pas") || { echo "- BUILD-ERR"; return; }
         out=$(printf '%d\n' "$reps" | "$WRAP" timeout "$T" "$bin" >/dev/null 2>"$W/e.$$") ;;
    m3)  out=$(printf '%d\n' "$reps" | "$WRAP" timeout "$T" "$SCRIP" --run "$B/$k.pas" >/dev/null 2>"$W/e.$$") ;;
    m4)  local s="$W/$k.s" b="$W/$k.bin"
         if [ ! -x "$b" ]; then
           if ! (timeout "$T" "$SCRIP" --compile "$B/$k.pas" </dev/null >"$s" 2>/dev/null) || [ ! -s "$s" ]; then echo "- BUILD-ERR"; return; fi
           if ! (as --64 -o "$W/$k.o" "$s" 2>/dev/null && gcc -no-pie -o "$b" "$W/$k.o" "$RT/libscrip_rt.so" -lm -Wl,-rpath,"$RT" 2>/dev/null); then echo "- LINKFAIL"; return; fi
         fi
         out=$(printf '%d\n' "$reps" | "$WRAP" timeout "$T" "$b" >/dev/null 2>"$W/e.$$") ;;
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
# ⛔ PER-KERNEL, PER-ENGINE REPS CEILING (row pascal-quick-m3-recursive-reps-cliff-13, seat03 2026-08-28):
# `quick`'s recursive `qsort` SIGSEGVs under m3 (and m4, same SCRIP lowering) at a sharp, deterministic
# cliff -- reps=12 clean, reps=13+ crash, every trial -- found live by THIS script's own doubling search
# (nobody had timed `quick` past reps=3 before). Capping m3/m4's search below the cliff lets the OTHER
# engine/angle for `quick` still measure honestly instead of the whole kernel going UNPROVEN.
# ⛔ ENGINE-SCOPED, NOT KERNEL-WIDE -- FOUND LIVE THIS PASS: an earlier version of this function capped
# ALL THREE engines at 12, including fpc. fpc has no such defect, runs ~10000x faster than the cap needs,
# and at reps=12 its measurement is almost entirely fixed process-startup overhead (~1.3ms fixed vs
# ~10us/rep real cost) rather than steady-state throughput -- angle 1 and angle 2 disagreed 12-18x on
# fpc/quick as a direct, reproducible result (not noise -- it repeated in the same direction both times,
# unlike the genuine load-contention DISAGREEs elsewhere in this campaign, which flip direction run to
# run). The cap now applies only to the engines that actually crash.
kernel_reps_cap() { local k="$1" eng="$2"; case "$k:$eng" in quick:m3|quick:m4) echo 12;; *) echo "$NMAX";; esac; }
# doubling search: largest reps whose cpu time >= BUDGET_MS, cap NMAX or the kernel+engine's own lower ceiling.
# Echoes "reps rate" or "reps -" + reason.
search() {
  local eng="$1" k="$2" reps=1 cpu cap; cap="$(kernel_reps_cap "$k" "$eng")"
  while :; do
    cpu=$(run1 "$eng" "$k" "$reps")
    case "$cpu" in -\ *) echo "$reps $cpu"; return ;; esac
    if [ "$cpu" -ge $((BUDGET_MS*1000)) ] || [ "$reps" -ge "$cap" ]; then
      awk -v n="$reps" -v us="$cpu" 'BEGIN{printf "%d %.4f", n, n/(us/1e6)}'; return
    fi
    reps=$((reps*4)); [ "$reps" -gt "$cap" ] && reps=$cap
  done
}

echo "TIME-BASED PASCAL BENCHMARKS -- angle 1: fixed wall-time budget (${BUDGET_MS}ms), reps counted via live doubling search"
echo "engines: fpc m3 m4   corpus: $B   budget: TIME_BUDGET_MS=$BUDGET_MS cap NMAX=$NMAX   external instrument: tools/bench_rusage"
echo
printf "%-10s %14s %14s %14s  %s\n" BENCHMARK fpc/s m3/s m4/s check
printf "%-10s %14s %14s %14s  %s\n" "----------" "--------------" "--------------" "--------------" "-----"
tot_ok=0; tot_skip=0
for k in $KERNELS; do
  pas="$B/$k.pas"; ref="$B/$k.ref"
  [ -f "$pas" ] && [ -f "$ref" ] || { printf "%-10s MISSING kernel or .ref\n" "$k"; tot_skip=$((tot_skip+1)); continue; }
  want=$(cat "$ref")
  m3o=$(printf '1\n' | timeout 15 "$SCRIP" --run "$pas" 2>/dev/null)
  if [ "$m3o" != "$want" ]; then
    printf "%-10s %14s %14s %14s  %s\n" "$k" SKIP SKIP SKIP "correctness-fail(reps=1 m3 != .ref)"; tot_skip=$((tot_skip+1)); continue
  fi
  declare -A R=() C=()
  for eng in fpc m3 m4; do
    res=$(search "$eng" "$k"); n=$(awk '{print $1}' <<<"$res"); r=$(awk '{print $2}' <<<"$res")
    case "$r" in -) R[$eng]="NA"; C[$eng]="$eng@reps=$n:$(cut -d' ' -f3- <<<"$res")" ;; *) R[$eng]="$r" ;; esac
  done
  ckstat=ok; for eng in fpc m3 m4; do [ -n "${C[$eng]:-}" ] && ckstat="${C[$eng]}"; done
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1))
  printf "%-10s %14s %14s %14s  %s\n" "$k" "${R[fpc]:-NA}" "${R[m3]:-NA}" "${R[m4]:-NA}" "$ckstat"
  unset R C
done
echo
echo "CHECK RESULT: measured=$tot_ok correctness-skip=$tot_skip"
[ "$tot_skip" -eq 0 ]
