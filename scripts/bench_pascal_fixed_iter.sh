#!/usr/bin/env bash
# bench_pascal_fixed_iter.sh -- ANGLE 2 of the Pascal three-angle triangulation (row bench-rivals-raku-pascal,
# mirroring bench_prolog_fixed_iter.sh / ARCH-BENCH-CAMPAIGN-README-TABLES.md THREE-ANGLE TRIANGULATION).
#
# Run each kernel's OWN internal `for rep := 1 to reps do <body>` loop EXACTLY N times, N fixed and
# COMMITTED in corpus/benchmarks/pascal/SCALE.tsv (calibrated once, see that file's own header -- never a
# live search, that is angle 1's job), and report throughput (reps/s) for fpc / m3 / m4. N is fed via
# stdin exactly like angle 1 -- these kernels need no source rewriting, unlike Prolog's vanroy/ wrappers.
# ⛔ TWO N COLUMNS, NOT ONE: fpc (-O2) is 100-3000x faster than SCRIP's -O0 arms on several kernels here --
# a single shared N cannot amortize a fast engine's fixed per-process overhead without being absurdly long
# for the slow one (measured: a shared N gave fpc a spurious 0.30x angle1-vs-angle2 DISAGREE, cured by
# splitting the column -- see SCALE.tsv's own header). m3 and m4 share `scrip_reps` -- empirically close
# enough in speed on every kernel measured so far that splitting them further wasn't warranted.
#
# ⛔ CORRECTNESS IS NOT RE-VERIFIED HERE -- angle 1 (test_bench_pascal_timed.sh) already re-checks every
# kernel's reps=1 output against .ref before timing anything; this script trusts that provenance (same
# split of duty the Prolog angle-2 script uses) and gates only on the run completing without crash/DNF.
#
# External CPU time via tools/bench_rusage, same instrument angle 3 (disk telemetry) and the SNOBOL4/
# Prolog triangulators both use -- never engine self-timing.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/pascal}"
SCALE="${SCALE_TSV:-$B/SCALE.tsv}"
T="${TIMEOUT:-60}"
KERNELS="${KERNELS:-}"   # optional allowlist, space-separated basenames -- same convention as the Prolog scripts
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE libscrip_rt.so not built"; exit 2; }
[ -f "$SCALE" ] || { echo "⛔ REFUSED-TO-GRADE SCALE.tsv missing: $SCALE"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSED: cannot load lib_oracle_flags.sh"; exit 2; }
FPC="$(fpc_bin)" || exit 2
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build" >&2; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT

fpc_build() {
  local pas="$1" k out; k=$(basename "${pas%.pas}"); out="$W/fpc_$k"
  [ -x "$out" ] && { printf '%s\n' "$out"; return 0; }
  { printf '{$mode objfpc}\n'; cat "$pas"; } > "$W/fpc_$k.pas"
  "$FPC" -O2 -o"$out" "$W/fpc_$k.pas" >"$W/fpc_$k.buildlog" 2>&1 || return 1
  printf '%s\n' "$out"
}
rate() { awk -v n="$1" -v us="$2" 'BEGIN{ if (us+0>0) printf "%.4f", n/(us/1e6); else print "NA" }'; }

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

echo "FIXED-ITERATION PASCAL BENCHMARKS -- angle 2: reps fixed per kernel (committed in SCALE.tsv), external cpu time measured"
echo "engines: fpc m3 m4   SCALE: $SCALE   external instrument: tools/bench_rusage (user+sys cpu time)"
echo
printf "%-10s %10s %10s %14s %14s %14s  %s\n" BENCHMARK N_fpc N_scrip fpc/s m3/s m4/s check
printf "%-10s %10s %10s %14s %14s %14s  %s\n" "----------" "----------" "----------" "--------------" "--------------" "--------------" "-----"
tot_ok=0; tot_bad=0
while IFS=$'\t' read -r k nf ns; do
  [[ "$k" == \#* || -z "$k" ]] && continue
  if [ -n "$KERNELS" ]; then case " $KERNELS " in *" $k "*) ;; *) continue ;; esac; fi
  [ -f "$B/$k.pas" ] || { printf "%-10s %10s %10s   MISSING kernel: %s\n" "$k" "$nf" "$ns" "$B/$k.pas"; tot_bad=$((tot_bad+1)); continue; }
  ckstat=ok; declare -A RATE=()
  for eng in fpc m3 m4; do
    n="$ns"; [ "$eng" = fpc ] && n="$nf"
    res=$(run1 "$eng" "$k" "$n"); cpu=$(awk '{print $1}' <<<"$res")
    if [ "$cpu" = "-" ]; then RATE[$eng]="NA"; reason=$(cut -d' ' -f3- <<<"$res"); [ "$ckstat" = ok ] && ckstat="$eng:$reason"
    else RATE[$eng]=$(rate "$n" "$cpu"); fi
  done
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1)) || tot_bad=$((tot_bad+1))
  printf "%-10s %10s %10s %14s %14s %14s  %s\n" "$k" "$nf" "$ns" "${RATE[fpc]}" "${RATE[m3]}" "${RATE[m4]}" "$ckstat"
  unset RATE
done < "$SCALE"
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad   (bad = crash/DNF/build-fail on at least one engine this run; correctness itself is angle 1's job)"
[ "$tot_bad" -eq 0 ]
