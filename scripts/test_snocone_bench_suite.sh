#!/usr/bin/env bash
source "$(dirname "${BASH_SOURCE[0]}")/lib_one_runner.sh" && one_runner_guard "${0##*/}" || exit 2
# test_snocone_bench_suite.sh -- THE SNOCONE BENCHMARK SUITE, GRADED AS A TEST UNDER ALL THREE ANGLES, IN BOTH MODES
#
# LON 2026-09-23 17:2x-17:4x CDT, in-chat to the ceo, verbatim (CEO-1221): "You messed up and forgot to have the benchmarks in the
# test suite grid. Also work was never completed to make proper REF, IN, and OUT files and make all the benchmarks double as tests.
# And all language tests need to be run through the three-angle harness: (1) wrapper process, (2) iterations, and (3) time limit."
# -- "The reason to force a benchmark to also be a test is to guarantee it is not vacuous." -- "The source of the benchmark is the
# apple-core of the test, the wrapper code can be added to do both the test and the measurements." Law: RULES.md FACT RULE THE
# KERNEL CONVENTION, 2026-09-23 addendum. Row: snocone-benchmarks-double-as-tests-ref-in-out-through-the-three-angle-harness-...
#
# THE POPULATION is every .sc under corpus/benchmarks/snocone, recursively -- no exclusions, printed as the denominator.
# THE SIDECARS, per program NAME: REF = NAME.ref (cut from the oracle: sbl -bf on the SNOBOL4 twin, same input); IN = NAME.input,
#   else the family's .input (NAME less -match / -match-fence), else /dev/null; MEMORY = NAME.heap_kb, one integer in KB, exported
#   as SCRIP_HEAP_KB for THAT program only (RULES.md heap-cap clause 7: a declared need is an attribute of the test).
# THE THREE ANGLES, each run in m3 (scrip --run) and m4 (--compile, as, gcc, run), the program's STDOUT diffed against its REF
#   EVERY TIME -- a benchmark whose answer is wrong under any angle is not a benchmark:
#   process  the PRISTINE source under tools/bench_rusage (the process wrapper: elapsed, CPU, RSS, context switches)
#   iter     the generated wrapper (bench_wrap_snocone.py --mode iter): the test once, then N repetitions timed from inside
#   time     the generated wrapper (--mode time): the test once, then repetitions counted inside a fixed budget
#   The wrappers write their one BENCH line to TERMINAL (stderr); a wrapper run that prints no BENCH line is UNMEASURED, not a pass.
# OUT: every run's stdout is kept as <out-dir>/<program>.<mode>.<angle>.out -- the file diffed against the REF (the ceo's reading of
#   OUT until Lon rules on it). --out-dir keeps them; without it they live in a temp dir removed at exit.
# THE VERDICT: a program PASSES when all six runs (two modes x three angles) print its REF. The board line counts programs.
# ⛔ THE TIMING COLUMNS ARE SCOUTING DATA, NEVER A GRID: timing grids wait on the quiet box (CEO-1219); the correctness verdict does not.
#
# USAGE: bash scripts/test_snocone_bench_suite.sh [--out-dir DIR] [--n N] [--bud-ms MS] [--timeout S]
#   --n N        iter-angle repetitions (default: each marked program's check=, else 1)   --bud-ms MS  time-angle budget (default 200)
# ⛔ PUBLISHING IS BY HAND UNTIL THE ONE PUBLISHER CAN TAKE A BENCHMARK ROW: util_score_row.py writes a suite-table row only
#   beside a grid cell, and the one cell named bench is the grid's B cell -- the two-number TIMING reading -- which this CORRECTNESS
#   board must not overwrite. The path is the coo's row instruments-benchmarks-enter-the-suite-grid-one-row-per-language-...; until
#   it lands, the runner's operator copies the SUITE_BOARD line into .github SUITES.tsv row snc-bench (nick SncBench), SCORE.md's
#   suite table and board-lines, in the same push.
# EXIT 0 every program PASS; 1 any program not PASS; 2 REFUSED (no binary, no population, a tool that cannot be built).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="$ROOT/scrip"; RT="$ROOT/out"; BD="$S4E/corpus/benchmarks/snocone"; GEN="$HERE/bench_wrap_snocone.py"
OUTDIR=""; NITER=""; BUD=200; TMO=300
while [ $# -gt 0 ]; do case "$1" in
  --out-dir) OUTDIR="$2"; shift 2;; --n) NITER="$2"; shift 2;; --bud-ms) BUD="$2"; shift 2;; --timeout) TMO="$2"; shift 2;;
  *) echo "⛔ REFUSE(2): unknown argument '$1' (usage in the header)"; exit 2;; esac; done
refuse() { echo "⛔ SNOCONE BENCH SUITE REFUSE(2): $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip is not built at $SCRIP -- run make"
[ -f "$RT/libscrip_rt.so" ] || refuse "no runtime at $RT/libscrip_rt.so"
[ -f "$GEN" ] || refuse "the wrapper generator $GEN is missing"
[ -d "$BD" ] || refuse "no benchmark tree at $BD"
"$HERE/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" >/dev/null || refuse "the binary is older than the tree -- run make (util_require_fresh.sh)"
W="$(mktemp -d "${TMPDIR:-/tmp}/sncbench.XXXXXX")" || refuse "cannot make a work dir"; trap 'rm -rf "$W"' EXIT
[ -n "$OUTDIR" ] || OUTDIR="$W/out"; mkdir -p "$OUTDIR" || refuse "cannot make $OUTDIR"
RUSAGE="$W/bench_rusage"; gcc -o "$RUSAGE" "$ROOT/tools/bench_rusage.c" 2>"$W/rusage.err" || refuse "tools/bench_rusage.c did not build: $(head -1 "$W/rusage.err")"
mapfile -t POP < <(find "$BD" -type f -name '*.sc' | LC_ALL=C sort)
[ "${#POP[@]}" -gt 0 ] || refuse "an empty population under $BD -- a suite that grades nothing is not a pass"
stdin_of() { local b="${1%.sc}" d fam; [ -f "$b.input" ] && { echo "$b.input"; return; }
  d="$(dirname "$1")"; fam="$(basename "$b")"; fam="${fam%-match-fence}"; fam="${fam%-match}"
  [ -f "$d/$fam.input" ] && { echo "$d/$fam.input"; return; }; echo /dev/null; }
# run_one <mode> <program.sc> <stdin> <tag> <prefix...> -> stdout in $W/o, stderr in $W/e, rc in RC; m4 compiles and links first
run_one() { local mode="$1" prog="$2" in="$3" tag="$4"; shift 4
  if [ "$mode" = m3 ]; then "$@" timeout "$TMO" "$SCRIP" "$prog" <"$in" >"$W/o" 2>"$W/e"; RC=$?; return; fi
  if ! "$SCRIP" --compile -o "$W/$tag.s" "$prog" </dev/null >"$W/c.err" 2>&1 || ! gcc -no-pie -o "$W/$tag.x" "$W/$tag.s" -L"$RT" -Wl,-rpath,"$RT" -lscrip_rt -lm >>"$W/c.err" 2>&1; then
    : >"$W/o"; cp "$W/c.err" "$W/e"; RC=97; return; fi
  "$@" timeout "$TMO" "$W/$tag.x" <"$in" >"$W/o" 2>"$W/e"; RC=$?; }
echo "=== SNOCONE BENCHMARK SUITE: ${#POP[@]} programs x 2 modes x 3 angles, graded against each REF (SCRIP $(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY), corpus $(git -C "$S4E/corpus" rev-parse --short HEAD 2>/dev/null)$(git -C "$S4E/corpus" diff --quiet 2>/dev/null || echo -DIRTY), RT_OPT from the Makefile, iter n=${NITER:-marker check}, time budget ${BUD} ms) ==="
PASS=0; P3=0; P4=0; REFLESS=0; UNWRAP=0; HEAPD=0; NOTES=""; SCOUT=""
for f in "${POP[@]}"; do
  rel="${f#$BD/}"; name="${rel%.sc}"; ref="${f%.sc}.ref"; in="$(stdin_of "$f")"; heap=()
  if [ -s "${f%.sc}.heap_kb" ]; then heap=(env -u SCRIP_HEAP_MB "SCRIP_HEAP_KB=$(head -1 "${f%.sc}.heap_kb" | tr -dc 0-9)"); HEAPD=$((HEAPD + 1)); else heap=(env); fi
  if [ ! -s "$ref" ]; then REFLESS=$((REFLESS + 1)); printf '  %-36s REFLESS -- no non-empty .ref, not a pass\n' "$rel"; continue; fi
  want="$(cat "$ref")"; ok3=1; ok4=1; cells=""
  for angle in process iter time; do
    prog="$f"
    if [ "$angle" != process ]; then
      if ! python3 "$GEN" "$f" --mode "$angle" ${NITER:+--n "$NITER"} --bud-ms "$BUD" -o "$W/w_$angle.sc" 2>"$W/g.err"; then
        UNWRAP=$((UNWRAP + 1)); NOTES="$NOTES\n    $rel $angle: UNWRAPPABLE -- $(head -1 "$W/g.err")"; ok3=0; ok4=0; cells="$cells ${angle}=UNWRAPPABLE"; continue; fi
      prog="$W/w_$angle.sc"; fi
    for mode in m3 m4; do
      if [ "$angle" = process ]; then run_one "$mode" "$prog" "$in" "p" "${heap[@]}" "$RUSAGE"; else run_one "$mode" "$prog" "$in" "$angle" "${heap[@]}"; fi
      mkdir -p "$(dirname "$OUTDIR/$name")"; cp "$W/o" "$OUTDIR/$name.$mode.$angle.out"
      got="$(cat "$W/o")"; v=PASS
      if [ "$got" != "$want" ]; then v="FAIL(rc=$RC)"; NOTES="$NOTES\n    $rel $mode $angle: stdout differs from the REF (rc=$RC) $(grep -v '^BENCH' "$W/e" | head -1 | cut -c1-120)"
      elif [ "$angle" != process ] && ! grep -q '^BENCH mode=' "$W/e"; then v="UNMEASURED(rc=$RC)"; NOTES="$NOTES\n    $rel $mode $angle: the REF matched but no BENCH line reached stderr"; fi
      [ "$v" = PASS ] || { [ "$mode" = m3 ] && ok3=0 || ok4=0; }
      cells="$cells $mode.$angle=$v"
      case "$angle" in process) SCOUT="$SCOUT\n  $rel $mode process $(grep -o 'elapsed_ns=[0-9]*' "$W/e" | head -1) $(grep -o 'maxrss_kb=[0-9]*' "$W/e" | head -1)";;
        *) SCOUT="$SCOUT\n  $rel $mode $angle $(grep -o 'iters=[0-9]* ns=[0-9]*' "$W/e" | head -1)";; esac
    done
  done
  [ "$ok3" = 1 ] && P3=$((P3 + 1)); [ "$ok4" = 1 ] && P4=$((P4 + 1))
  if [ "$ok3" = 1 ] && [ "$ok4" = 1 ]; then PASS=$((PASS + 1)); printf '  %-36s PASS  (6 of 6 runs print the REF)%s\n' "$rel" "$([ ${#heap[@]} -gt 1 ] && echo "  heap ${heap[3]#SCRIP_HEAP_KB=} KB declared")"
  else printf '  %-36s FAIL %s\n' "$rel" "$cells"; fi
done
N=${#POP[@]}
[ -n "$NOTES" ] && printf 'NOT PASSING, NAMED:%b\n' "$NOTES"
printf 'SCOUTING DATA -- one run each, a loaded box, NOT A GRID (timing waits for the quiet box, CEO-1219):%b\n' "$SCOUT"
LINE="SUITE_BOARD family=snocone-bench total=$N shipped=$N all_pass=$PASS all_n=$N m3_pass=$P3 m4_pass=$P4 angles=process,iter,time refless=$REFLESS unwrappable=$UNWRAP heap_declared=$HEAPD"
echo "$LINE"
[ "$PASS" = "$N" ] && exit 0
exit 1
