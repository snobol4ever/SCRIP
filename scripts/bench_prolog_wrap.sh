#!/usr/bin/env bash
# bench_prolog_wrap.sh -- BUILD A TIMED / COUNTED PROLOG BENCHMARK ON THE FLY FROM A PRISTINE KERNEL.
#
#   bash scripts/bench_prolog_wrap.sh <kernel.pl> [-o <out.pl>] [--mode=single|time|iter] [--n=<iters>]
#                                     [--engine=scrip|gnu|swi]
#
# ⭐ WHY THIS EXISTS (CEO-567, RULES.md:321, ARCH-PROGRAM-LEDGER.md): the kernel's source holds the
#   COMPUTATION AND NOTHING ELSE; the wrapping and the messaging are GENERATED around a pristine source.
#   This is the Prolog twin of bench_wrap.sh, which has done the same job for SNOBOL4 since s265.
#
#   BEFORE (measured hq_P 2026-09-13, the state this script was written to end):
#     corpus/benchmarks/prolog/bench/ shipped TWO conventions in ONE directory -- 13 kernels were
#     pristine result-signature programs, and 10 baked a wall_us/wall_ms timing bracket into main/0.
#     ⛔ The 10 were UNMEASURABLE ON THE ENGINE UNDER MEASUREMENT: the rival arms consult
#     prelude_gplc.pl / prelude_swipl.pl, which define wall_us/1 and wall_ms/1, but SCRIP registers
#     wall_us and wall_ms at ARITY 0 ONLY (by_name_dispatch.c:2842, `nargs == 0`), so every one of the
#     ten raised existence_error(wall_us/1) -- AND EXITED 0, printing no answer.  A sweep counted that
#     as a run.  So the Prolog speed axis has been silently unmeasurable on SCRIP since the harness was
#     designed, while the rival columns kept reporting real numbers.
#     corpus/benchmarks/prolog/vanroy/ is the same 10+11 kernels with `main :- l__(64).` FROZEN into
#     each source -- the iteration count living inside the artifact under measurement.
#
#   AFTER: every corpus/benchmarks/prolog/bench/*.pl is a real standalone program that prints its own
#   oracle-graded answer and contains no timing and no iteration builtin.  This script builds the timed
#   or counted version into a temp file when someone wants a number.
#
# THE CONTRACT -- two things in the pristine kernel, both of which keep it standalone:
#
#     % *BENCH kernel=<name>                 one comment line, inert to every engine
#     bench_work(Res) :- <the computation>.  the work, and only the work
#     main :- bench_work(Res), write(Res), nl.
#
#   bench_work/1 is the bracket's content: the timing bracket encloses the CALL TO IT and nothing else,
#   so the published number is work-on-work and the write stays outside.  main/0 is what makes the file
#   a program rather than a fragment, and its stdout is what <name>.expected grades.
#
# ⭐ THREE ANGLES OVER ONE PRISTINE SOURCE (the same three bench_wrap.sh gives SNOBOL4):
#     --mode=single  the work ONCE, self-timed.  stdout is the kernel's real answer, so the run is
#                    GRADED and TIMED at once; work_us/work_ms go to user_error and stdout stays
#                    byte-comparable against <name>.expected.
#     --mode=iter    exactly --n=N iterations, no wall-clock deadline anywhere.  This is the arm for
#                    callgrind/cachegrind, where a deadline measures the INSTRUMENT's throughput
#                    instead of the kernel's (FINDING-2026-08-22-bench-harness-unmeasurable).
#                    ⛔ stdout is N copies of the answer ON PURPOSE -- that is what lets loop_check()
#                    in lib_prolog_bench.sh PROVE the loop looped before its time is divided by N.
#     --mode=time    (angle 3's companion) run the work until --bud ms is spent, COUNT the iterations.
#   The process wrapper -- /usr/bin/time, perf stat, callgrind around the whole run -- is angle 3 and
#   needs no generated file at all; its total minus the self-measured work IS the overhead number.
#
# ⛔ --engine names WHO WILL RUN THE OUTPUT, and it changes only the PRELUDE, never the kernel.
#   ⭐ The prelude is here rather than in the kernel because the KERNEL must be byte-identical across
#   every engine -- the kernel is the thing being compared.
#
# ⛔⛔ SCRIP'S PROLOG HAS NO WALL CLOCK AT ALL, AND THAT IS REPORTED, NOT PAPERED OVER
#   (MEASURED by execution, hq_P 2026-09-13, SCRIP 202d8bfff).  The row that sent this work up said the
#   defect was an ARITY mismatch -- wall_us registered at 0, called at 1.  It is worse than that: all
#   SIX spellings the three engines use raise on SCRIP --
#       wall_us/1 . wall_ms/1 . statistics/2 . real_time/1 . get_time/1  -> existence_error(procedure)
#       X is cputime . X is realtime                                     -> type_error(evaluable)
#   The `nargs == 0` wall_us/wall_ms at by_name_dispatch.c:2842 is SNOBOL4's BY-NAME dispatch and is not
#   reachable from Prolog at any arity, so registering a /1 twin there would not reach Prolog either.
#   ⭐ CONSEQUENCE FOR THE BASIS: the two-number WORK/OVERHEAD basis has NO work number on SCRIP's Prolog
#   arm.  Only the process wrapper (angle 3, external total) can produce a Prolog number on SCRIP today,
#   and an external total is a TOTAL -- it carries startup and may never share a column with a rival's
#   self-measured work (RULES.md, apples-to-apples: a SLOPE is not a TOTAL).
#   ⛔ So for --engine=scrip this script emits the counted form WITHOUT a bracket and says
#   work_us=UNAVAILABLE in the BENCH line.  It does NOT emit a bracket that raises and exits 0, which is
#   what the checked-in sources did: existence_error(wall_us/1) printed to stderr, no answer on stdout,
#   rc=0 -- a sweep counted that as a run.  A missing instrument is reported as missing.
set -u
SELF="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SELF/../.." && pwd)"
PRO="$ROOT/corpus/benchmarks/prolog"
MODE=single; N=""; BUD=1000; OUT=""; ENGINE=scrip; KPL=""
for a in "$@"; do
  case "$a" in
    --mode=*)   MODE="${a#*=}" ;;
    --n=*)      N="${a#*=}" ;;
    --bud=*)    BUD="${a#*=}" ;;
    --engine=*) ENGINE="${a#*=}" ;;
    -o)         OUT="__NEXT__" ;;
    *)          if [ "$OUT" = "__NEXT__" ]; then OUT="$a"; else KPL="$a"; fi ;;
  esac
done
[ -n "$KPL" ] && [ -f "$KPL" ] || { echo "⛔ REFUSED: no kernel source given (usage in the header)" >&2; exit 2; }
case "$MODE" in single|iter|time) ;; *) echo "⛔ REFUSED: unknown --mode=$MODE" >&2; exit 2 ;; esac
case "$ENGINE" in scrip|gnu|swi) ;; *) echo "⛔ REFUSED: unknown --engine=$ENGINE" >&2; exit 2 ;; esac
[ "$MODE" = iter ] && [ -z "$N" ] && { echo "⛔ REFUSED: --mode=iter needs --n=<iterations>" >&2; exit 2; }
# CLOCK is the per-engine capability the two-number basis needs; see the header. Measured, not assumed.
CLOCK=1; [ "$ENGINE" = scrip ] && CLOCK=0
if [ "$CLOCK" = 0 ] && [ "$MODE" = time ]; then
  echo "⛔ REFUSED: --mode=time needs a wall clock and SCRIP's Prolog has none (see the header)." >&2
  echo "   Use --mode=iter with a fixed --n and time the whole process from OUTSIDE (angle 3)." >&2; exit 2; fi
# ⛔ THE CONTRACT IS CHECKED, NOT ASSUMED: a kernel that does not carry it would silently produce a
# wrapper whose bracket encloses nothing, and the harness would publish the startup cost as the work.
K="$(sed -n 's/^% \*BENCH kernel=\([A-Za-z0-9_]*\).*/\1/p' "$KPL" | head -1)"
[ -n "$K" ] || { echo "⛔ REFUSED: $KPL carries no '% *BENCH kernel=<name>' marker" >&2; exit 2; }
grep -qE '^bench_work\(' "$KPL" || { echo "⛔ REFUSED: $KPL defines no bench_work/1 -- the bracket would enclose nothing" >&2; exit 2; }
if grep -qE '\b(wall_us|wall_ms|statistics|real_time|get_time)\s*\(' "$KPL"; then
  echo "⛔ REFUSED: $KPL is NOT PRISTINE -- it carries a timing builtin (CEO-567)" >&2; exit 2; fi
[ -n "$OUT" ] || OUT="$(mktemp -t "bench_${K}_XXXXXX.pl")"
{
  echo "% GENERATED by scripts/bench_prolog_wrap.sh from $KPL -- DO NOT CHECK IN."
  echo "% mode=$MODE engine=$ENGINE${N:+ n=$N}${MODE:+ }kernel=$K"
  echo "% ⛔ The kernel source below is VERBATIM. Everything outside it is the wrapper, and the wrapper"
  echo "% is the only thing that knows about time or about iteration counts (CEO-567)."
  case "$ENGINE" in
    gnu) cat "$PRO/prelude_gplc.pl" ;;
    swi) cat "$PRO/prelude_swipl.pl" ;;
    scrip) echo "% no prelude: SCRIP's Prolog has no wall clock at any spelling (measured; see the header),"
           echo "% so this generated program carries NO timing bracket and reports work_us=UNAVAILABLE." ;;
  esac
  # the kernel verbatim, minus its own main/0 and its initialization directive -- the wrapper supplies both
  sed -e '/^:- *initialization(main)\./d' -e '/^main *:- *bench_work(Res), *write(Res), *nl\./d' "$KPL"
  echo ":- initialization(main)."
  # ⛔ one write per iteration ON PURPOSE in --mode=iter: lib_prolog_bench.sh's loop_check() compares
  # stdout against N copies of the .expected, which is the ONLY proof the loop ran N times before the
  # time is divided by N. MEASURED s-history: without it, a wrapper that stopped after TWO iterations was
  # divided by N=65536 and published m3 at 3,102,442 iter/s against gprolog's 485 -- and angle 1 and
  # angle 2 AGREED on the fabrication to within 1%, because they shared the defect.
  case "$MODE" in
    single) [ "$CLOCK" = 1 ] \
      && { echo "main :- wall_us(T0), wall_ms(M0), bench_work(Res), wall_us(T1), wall_ms(M1),"
           echo "        write(Res), nl, W is T1 - T0, WM is M1 - M0,"
           echo "        format(user_error, \"BENCH kernel=$K mode=single work_us=~w work_ms=~w~n\", [W, WM])."; } \
      || { echo "main :- bench_work(Res), write(Res), nl,"
           echo "        format(user_error, \"BENCH kernel=$K mode=single work_us=UNAVAILABLE work_ms=UNAVAILABLE engine=$ENGINE-has-no-wall-clock~n\", [])."; } ;;
    iter)
      echo "bench__loop(N) :- between(1, N, _), bench_work(Res), write(Res), nl, fail."
      echo "bench__loop(_)."
      [ "$CLOCK" = 1 ] \
      && { echo "main :- wall_us(T0), wall_ms(M0), bench__loop($N), wall_us(T1), wall_ms(M1),"
           echo "        W is T1 - T0, WM is M1 - M0,"
           echo "        format(user_error, \"BENCH kernel=$K mode=iter n=$N work_us=~w work_ms=~w~n\", [W, WM])."; } \
      || { echo "main :- bench__loop($N),"
           echo "        format(user_error, \"BENCH kernel=$K mode=iter n=$N work_us=UNAVAILABLE work_ms=UNAVAILABLE engine=$ENGINE-has-no-wall-clock~n\", [])."; } ;;
    time)
      # ⛔ between/3 + cut, NOT a recursive counter. MEASURED hq_P 2026-09-13: the recursive form
      # (bench__until(D,I0,I) :- ... bench__until(D,I1,I)) builds one frame per iteration and gprolog
      # died at "global stack overflow (reached: 32765 Kb)" on nrev before the 1000 ms budget was spent --
      # the deadline arm was measuring how fast the engine could exhaust its own stack. between/3 is
      # constant space on all three engines and yields the iteration count for free, with no assert.
      echo "bench__until(Deadline, I) :- between(1, 1000000000, I), bench_work(_), wall_ms(M), M >= Deadline, !."
      echo "main :- wall_ms(M0), wall_us(T0), Deadline is M0 + $BUD,"
      echo "        ( bench__until(Deadline, I) -> true ; I = exhausted ),"
      echo "        wall_us(T1), wall_ms(M1), W is T1 - T0, WM is M1 - M0,"
      echo "        format(user_error, \"BENCH kernel=$K mode=time bud_ms=$BUD iters=~w work_us=~w work_ms=~w~n\", [I, W, WM])." ;;
  esac
} > "$OUT" || exit 2
echo "$OUT"
