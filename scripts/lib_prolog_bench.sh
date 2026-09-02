#!/usr/bin/env bash
# lib_prolog_bench.sh -- the ONE implementation of two checks every Prolog loop-benchmark angle needs (hq_P 2026-09-02,
# row prolog-instruments-and-baseline-standup). Sourced by test_bench_prolog_timed.sh (angle 1) and bench_prolog_fixed_iter.sh
# (angle 2); a harness that cannot source it REFUSES rc=2 rather than re-implementing either check -- a rule with two copies
# proves the copies agree, never that either is right (same law as lib_perf_fmt.sh / lib_oracle_flags.sh).
#
# ⛔⛔ THE LOOP MUST BE PROVEN TO HAVE LOOPED BEFORE ITS TIME IS DIVIDED BY N. MEASURED on SCRIP f4532dea, -O0, the pin tree of the
# van Roy baseline: the between/3+fail wrapper vanroy/fib.pl (N=64) prints 64 answers on gprolog and on swipl and TWO on SCRIP m3
# and m4 -- then exits 0. Both angle harnesses gated only on bench_rusage's exit= field, so they divided N=65536 by the CPU time of
# two iterations and printed m3 at 3,102,442 iter/s against gprolog's 485 (fib), a 6,400x multiple for a program that had stopped
# after its second iteration. Angle 1 and angle 2 then AGREED on it to within 1% -- the two angles share the defect, so the
# cross-proof certified the fabrication (the "engines agreed at reps=0" witness of RULES.md THE INSTRUMENT LAWS §2, again).
# The cure is not a SCRIP-specific guard (NO PER-ENGINE FILTER): every engine's loop stdout is compared byte-for-byte against N
# copies of the kernel's .expected, and a run whose output is not exactly N answers is reported as what it IS --
# LOOP-OUTPUT-MISMATCH(lines=<seen>/<wanted>) -- and never as a rate. The observation prints first, the belief nowhere.
gnu_filter() { grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined'; }
# loop_check <engine> <stdout-file> <N> <expected-file>: rc=0 when stdout == N x expected (gnu stdout banner-filtered first, the
# same filter the single-shot correctness gate uses); otherwise echoes the reason and returns 1. No N or no .expected is UNGRADED,
# also rc=1: a loop whose iteration count cannot be verified has no rate.
loop_check() {
  local eng="$1" o="$2" n="$3" exp="$4"
  [ -n "$n" ] && [ -f "$exp" ] || { echo "UNGRADED(no N or no .expected)"; return 1; }
  awk -v n="$n" '{a[NR]=$0} END{for(i=1;i<=n;i++) for(j=1;j<=NR;j++) print a[j]}' "$exp" > "$o.want"
  case "$eng" in gnu) gnu_filter < "$o" > "$o.f" ;; *) cp "$o" "$o.f" ;; esac
  cmp -s "$o.f" "$o.want" && return 0
  echo "LOOP-OUTPUT-MISMATCH(lines=$(wc -l < "$o.f")/$(wc -l < "$o.want"))"; return 1
}
