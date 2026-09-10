#!/usr/bin/env bash
# bench_icon_classic_set.sh -- THE CLASSIC ICON BENCHMARK SET: concord deal ipxref queens rsg geddump,
# graded for CORRECTNESS against an iconx cut taken AT RUN TIME, then TIMED against Arizona iconx and
# JCON jcont on the two-number basis.  Lon 2026-09-10, verbatim via ceo CEO-493: "Get the classic Icon
# benchmark set working. Duh!!!"  Basis ruled by ceo CEO-494; written hq_P 2026-09-10.
#
# ⛔⭐⭐ WHY THIS IS NOT A callgrind Ir BOARD, AND THE RULING IS NOT A PREFERENCE (ceo CEO-494).
# Every other benchmark in this tree measures Ir, because Ir is DETERMINISTIC: bench_two_number_ir.sh's
# own header cites three consecutive runs returning byte-identical counts, and that property is what
# buys the economy of a single rep.  ⛔ IT DOES NOT HOLD FOR A JVM.  Measured here 2026-09-10 on the
# int_loop kernel under jcon: 1,069,787,424 Ir then 1,443,104,013 Ir -- a 35% spread on the same program
# and the same box, because JIT compilation decisions vary run to run.  So Ir REFUSES for the jcont arm,
# and jcont is measured on the two remaining angles (fixed iterations, and the process wrapper) by WALL
# and CPU, warm-up discarded, MEDIAN of at least five runs, the spread PRINTED beside every median.
# ⛔ NO MULTIPLE EVER CROSSES INSTRUMENTS: a wall multiple is computed against a wall number and a cpu
# multiple against a cpu number, and every column says which instrument produced it.
#
# ⛔⭐⭐ THE 790x TRAP, RECORDED HERE BECAUSE THIS IS THE FILE THAT MEASURES A JVM (hq_P 2026-09-10).
# If you ever DO put jcont under callgrind: callgrind does NOT follow exec into a child process unless
# --trace-children=yes.  A jcon program is a /bin/sh wrapper that execs java, so an untraced reading of
# an empty Icon program is 367,008 Ir where the truth is 291,132,178.  ⭐ The number it returns is not
# malformed, missing or suspicious -- it is a small, plausible, correctly measured count OF THE WRONG
# PROCESS, and on a board whose whole question is "who does less work" it reads as the rival winning by
# three orders of magnitude.  Every guard on that board -- the client exit-status check, the CEO-173
# overhead refusal -- passes it.
#
# THE TWO-NUMBER BASIS: OVERHEAD is the empty-program constant per engine, measured by this same
# protocol; WORK = median_total - median_overhead.  ⛔ CEO-173: when OVERHEAD is >= 50% of an arm's
# reading, the WORK multiple is REFUSED for that program and the labelled TOTAL-basis multiple is
# printed in its place -- never both, never a work number with a quiet asterisk.
#
# CORRECTNESS GATES THE NUMBER: the oracle output is cut at run time by iconx, and an engine whose
# output differs byte-for-byte gets NO multiple.  A wrong answer is never a fast answer.
#
# EXIT: 0 = board printed, 1 = a program was RED (wrong answer or did not run), 2 = REFUSED (nothing
# measurable -- an empty or plausible table is the failure this guards against, so it never prints one).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
ROOT="$HERE/.."
SCRIP_BIN="${SCRIP:-$ROOT/scrip}"
RTDIR="$ROOT/out"
CORPUS_SRC="${CORPUS_SRC:-$S4E/corpus/benchmarks/icon}"
REPS="${REPS:-5}"          # CEO-494: at least five timed runs, median reported
WARMUP="${WARMUP:-2}"      # CEO-494: warm-up runs discarded (the JVM needs them; everyone else pays nothing)
RUN_TMO="${RUN_TMO:-120}"
TSV_OUT="${TSV_OUT:-}"
TICK_MS="${TICK_MS:-10}"   # /usr/bin/time %e resolution on this box: 10 ms. Measured, not assumed -- every
                           # median on this board is a multiple of it.
QUANT_TICKS="${QUANT_TICKS:-5}"  # below this many ticks a multiple is QUANTIZED and is labelled so
refuse() { echo "⛔ CLASSIC-SET BOARD REFUSED (rc=2): $*" >&2; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || refuse "lib_oracle_flags.sh unloadable -- the ONE oracle-path authority (s200)."
. "$HERE/lib_perf_fmt.sh"     2>/dev/null || refuse "lib_perf_fmt.sh unloadable -- the ONE authority for printing a multiple (s266)."
[ -x "$SCRIP_BIN" ] || refuse "scrip not built at $SCRIP_BIN -- a table printed without it would be plausible and false."
ICONT="$(icont_bin)" || refuse "no icont"
ICONX="$(iconx_bin)" || refuse "no iconx"
JCONT="$(jcont_bin 2>/dev/null)" || JCONT=""
HAVE_JCON=0
if [ -n "$JCONT" ] && [ -x "$JCONT" ] && command -v java >/dev/null 2>&1; then HAVE_JCON=1; fi
[ -d "$CORPUS_SRC" ] || refuse "corpus dir $CORPUS_SRC not found."
command -v /usr/bin/time >/dev/null 2>&1 || refuse "/usr/bin/time missing -- it is what produces the CPU number; bash's builtin cannot be trusted across engines."
WORK="$(mktemp -d "${TMPDIR:-/tmp}/icon_classic.XXXXXX")" || refuse "cannot make a work dir."
trap 'rm -rf "$WORK"' EXIT
export PATH="$(dirname "$ICONT"):$PATH"   # jcont shells out to icont; without this it fails talking about icont

# ---------- staging ----------
C="$WORK/corpus"; mkdir -p "$C"
cp "$CORPUS_SRC"/*.icn "$CORPUS_SRC"/*.dat "$C/" 2>/dev/null
( cd "$C" && "$ICONT" -c options.icn post.icn shuffle.icn ) >/dev/null 2>&1   # link units for iconx
IPXIN="$WORK/ipxref_input.icn"; cat "$C"/*.icn > "$IPXIN"
declare -A STDIN ARGS DEPS
STDIN[concord]="$C/concord.dat"; ARGS[concord]="";        DEPS[concord]="options.icn post.icn"
STDIN[deal]="";                  ARGS[deal]="-h 1000";    DEPS[deal]="options.icn post.icn shuffle.icn"
STDIN[ipxref]="$IPXIN";          ARGS[ipxref]="";         DEPS[ipxref]="options.icn post.icn"
STDIN[queens]="";                ARGS[queens]="-n10";     DEPS[queens]="options.icn post.icn"
STDIN[rsg]="$C/rsg.dat";         ARGS[rsg]="";            DEPS[rsg]="options.icn post.icn"
STDIN[geddump]="$C/geddump.dat"; ARGS[geddump]="";        DEPS[geddump]=""
PROGS="${PROGS:-concord deal ipxref queens rsg geddump}"

# ---------- the timing primitive ----------
# Echoes "<wall_ms> <cpu_ms> <rc>" for ONE run.  CPU is user+sys from /usr/bin/time, which is the only
# number that survives a busy box honestly -- wall on a shared machine measures the neighbours too.
time_one() {  # $1 = stdin file or empty; $2.. = argv
  local sin="$1"; shift
  local tf="$WORK/t.$$"; local rc
  if [ -n "$sin" ]; then
    /usr/bin/time -f '%e %U %S' -o "$tf" timeout "$RUN_TMO" "$@" >"$WORK/run.out" 2>"$WORK/run.err" <"$sin"; rc=$?
  else
    /usr/bin/time -f '%e %U %S' -o "$tf" timeout "$RUN_TMO" "$@" >"$WORK/run.out" 2>"$WORK/run.err" </dev/null; rc=$?
  fi
  awk -v rc="$rc" '{printf "%.0f %.0f %d\n", $1*1000, ($2+$3)*1000, rc}' "$tf" 2>/dev/null || echo "0 0 $rc"
}
# Echoes "<median_wall> <median_cpu> <spread_pct> <rc>" over WARMUP+REPS runs, warm-up discarded.
# ⛔ THE SPREAD IS NOT DECORATION AND IS NEVER AVERAGED AWAY: it is the only thing on this board that
# tells a reader whether the median in front of them is a measurement or a coin flip, and on the JVM arm
# it is routinely tens of percent (CEO-494).  A board that hid it would read exactly like a deterministic one.
time_median() {  # $1 = stdin, $2.. = argv
  local sin="$1"; shift
  local i r w c rc lastrc=0
  local ws=() cs=()
  for i in $(seq 1 "$WARMUP"); do time_one "$sin" "$@" >/dev/null 2>&1; done
  for i in $(seq 1 "$REPS"); do
    r="$(time_one "$sin" "$@")"; w="${r%% *}"; c="$(echo "$r" | awk '{print $2}')"; rc="$(echo "$r" | awk '{print $3}')"
    lastrc="$rc"; [ "$rc" = "0" ] || { echo "0 0 0 $rc"; return 0; }
    ws+=("$w"); cs+=("$c")
  done
  printf '%s\n' "${ws[@]}" | sort -n > "$WORK/ws.txt"; printf '%s\n' "${cs[@]}" | sort -n > "$WORK/cs.txt"
  local mw mc lo hi sp
  mw="$(awk 'NR==int((n+1)/2)' n="$REPS" "$WORK/ws.txt")"; mc="$(awk 'NR==int((n+1)/2)' n="$REPS" "$WORK/cs.txt")"
  lo="$(head -1 "$WORK/ws.txt")"; hi="$(tail -1 "$WORK/ws.txt")"
  sp="$(awk -v l="$lo" -v h="$hi" 'BEGIN{printf "%.0f", (l>0? (h-l)*100/l : 0)}')"
  echo "${mw:-0} ${mc:-0} ${sp:-0} $lastrc"
}
# ⛔⭐⭐ THE ANSWER CUT -- WITHOUT THIS THE WHOLE CLASSIC SET GRADES RED FOREVER AND THE REASON IS NOT A
# DEFECT IN ANY ENGINE (hq_P 2026-09-10, and it is why Lon's "get the classic benchmark set working"
# needed an instrument before it needed a cure).
# These programs link `post.icn`, which is a BENCHMARK HARNESS, not a library: `Init__` prints &version,
# &host and every &features, then -- unless the environment variable OUTPUT is set -- ASSIGNS 1 TO write
# AND writes, SUPPRESSING THE PROGRAM'S ENTIRE ANSWER; `Term__` restores output and prints the elapsed
# time, the region sizes, the storage sizes and the collection counts.
# ⭐ SO THE DEFAULT STDOUT OF EVERY PROGRAM IN THIS SET IS PURE ENVIRONMENT AND TIMING WITH THE ANSWER
# DELIBERATELY REMOVED, and a byte-compare against an iconx cut can only ever fail: the version banner
# differs by construction, the elapsed time differs by construction, and the GC statistics differ by
# construction.  That is also what the vendored .std files are -- dumps of that report, which is why this
# tree's older runner header calls them "NOT diffable oracles".
# ✅ THE CUT: run with OUTPUT=1 and keep only the lines strictly BETWEEN the two markers the harness
# itself prints -- `*** Benchmarking with output ***` (Init__'s last line) and ` elapsed time = `
# (Term__'s first).  Those lines are the program's own answer and nothing else.  MEASURED: queens -n10
# grades RED by raw compare and its 16,653 answer lines are BYTE-IDENTICAL to iconx once cut.
# ⛔ TIMING IS RUN WITHOUT OUTPUT -- the suite's own convention, output suppressed -- so the timed arm
# measures computation and not terminal I/O.  The two arms therefore run the program twice, on purpose.
answer_cut() { awk '/^\*\*\* Benchmarking with output \*\*\*$/{on=1;next} / elapsed time = /{on=0} on' "$1"; }

# ---------- per-engine builders: each echoes a runnable argv, or nothing ----------
build_iconx() { ( cd "$C" && "$ICONT" -s -o "$WORK/$1.icx" "$1.icn" ) >/dev/null 2>&1 && echo "$ICONX $WORK/$1.icx"; }
build_jcont() { [ "$HAVE_JCON" = "1" ] || return 0
  local deps="${DEPS[$1]:-}"
  ( cd "$C" && bash "$JCONT" -s -o "$WORK/$1.jxe" "$1.icn" $deps ) >/dev/null 2>&1 && [ -x "$WORK/$1.jxe" ] && echo "$WORK/$1.jxe"; }
build_m4()    { local deps="${DEPS[$1]:-}"
  ( cd "$C" && "$SCRIP_BIN" --compile --target=x86 "$1.icn" $deps > "$WORK/$1.s" ) 2>"$WORK/$1.m4c.err"
  [ -s "$WORK/$1.s" ] || return 0
  gcc -no-pie "$WORK/$1.s" -L"$RTDIR" -lscrip_rt -Wl,-rpath,"$RTDIR" -lm -lpthread -o "$WORK/$1.bin" 2>"$WORK/$1.m4l.err" || return 0
  echo "$WORK/$1.bin"; }
# ⛔ m3 COMPILES ON EVERY RUN BY CONSTRUCTION -- it is `scrip --run`, so its total carries the compile
# where m4's does not.  That is a real difference between the modes, not an instrument artefact, and it
# is why the m3 column is labelled compile+run and may not be read as m4's rival.
m3_argv() { local deps="${DEPS[$1]:-}"; echo "$SCRIP_BIN --run $C/$1.icn ${deps:+$(for d in $deps; do echo -n "$C/$d "; done)}"; }

# ---------- OVERHEAD: the empty-program constant, per engine, same protocol ----------
printf 'procedure main()\nend\n' > "$C/zz_empty.icn"
STDIN[zz_empty]=""; ARGS[zz_empty]=""; DEPS[zz_empty]=""
declare -A OVW OVC
for eng in iconx jcont m3 m4; do OVW[$eng]="" ; OVC[$eng]=""; done
ov_probe() {  # $1 = engine
  local av
  case "$1" in
    iconx) av="$(build_iconx zz_empty)";;
    jcont) av="$(build_jcont zz_empty)";;
    m4)    av="$(build_m4 zz_empty)";;
    m3)    av="$(m3_argv zz_empty)";;
  esac
  [ -n "$av" ] || return 0
  local r; r="$(time_median "" $av)"
  [ "$(echo "$r" | awk '{print $4}')" = "0" ] || return 0
  OVW[$1]="$(echo "$r" | awk '{print $1}')"; OVC[$1]="$(echo "$r" | awk '{print $2}')"
}
for eng in iconx jcont m3 m4; do [ "$eng" = jcont ] && [ "$HAVE_JCON" != 1 ] && continue; ov_probe "$eng"; done

TREE="$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null || echo '?')"
CTREE="$(git -C "$S4E/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
echo "ICON_CLASSIC_SET tree=SCRIP $TREE corpus $CTREE RT_OPT=-O0 date=$(date -u +%Y-%m-%dT%H:%MZ) reps=$REPS warmup=$WARMUP"
echo "  ⛔ SHARED AXES (RULES.md § FACT RULES, APPLES TO APPLES) -- every row below shares ALL of these:"
echo "     instrument = WALL ms and CPU ms (user+sys) from /usr/bin/time, MEDIAN of $REPS runs after $WARMUP discarded warm-ups"
echo "     basis = two-number; OVERHEAD is the empty-program constant per engine; WORK = median - OVERHEAD"
echo "     RT_OPT=-O0 · oracle = $ICONX · rival = ${JCONT:-<absent>} · corpus $CTREE · one box, one sitting"
echo "     ⛔ CORRECTNESS is graded on the ANSWER CUT (OUTPUT=1, the lines between post.icn's own two markers),"
echo "        never on raw stdout: post.icn SUPPRESSES the answer unless OUTPUT is set and prints &version,"
echo "        &features, elapsed time and GC statistics instead -- a raw compare grades RED by construction."
echo "     ⛔ TIMING runs WITHOUT OUTPUT, the suite's own convention, so it measures computation not terminal I/O."
echo "     ⛔ RESOLUTION: /usr/bin/time reports wall to ${TICK_MS} ms. A multiple whose arms are under $QUANT_TICKS ticks"
echo "        (${qf:-$((TICK_MS*QUANT_TICKS))} ms) is printed with a QNT: prefix -- the figure is real, the precision it"
echo "        LOOKS like is not. Off this floor by a bigger input or a self-timed hook, never by a tighter format."
echo "     ⛔ callgrind Ir REFUSES on the jcont arm (ceo CEO-494): JIT makes a JVM's Ir vary ~35% run to run,"
echo "        so Ir is the SCRIP-vs-iconx instrument only and NO multiple here crosses instruments."
echo "  OVERHEAD (empty Icon program): iconx ${OVW[iconx]:-NA}ms/${OVC[iconx]:-NA}ms · jcont ${OVW[jcont]:-NA}ms/${OVC[jcont]:-NA}ms · m3 ${OVW[m3]:-NA}ms/${OVC[m3]:-NA}ms · m4 ${OVW[m4]:-NA}ms/${OVC[m4]:-NA}ms  (wall/cpu)"
if [ "$HAVE_JCON" != "1" ]; then
  echo "  ⚠ JCON RIVAL ABSENT -- every jcont cell below is EMPTY BECAUSE IT WAS NOT MEASURED, not because"
  echo "    jcont was slow: jcont=${JCONT:-<accessor refused>} java=$(command -v java || echo '<not on PATH>')"
fi
printf '%-9s %-7s %10s %10s %8s %10s %10s %8s %10s %10s %8s %10s %10s %8s %8s %14s %14s\n' \
  program verdict ix_wall ix_cpu ix_sp jc_wall jc_cpu jc_sp m3_wall m3_cpu m3_sp m4_wall m4_cpu m4_sp basis 'm4 x vs iconx' 'm4 x vs jcont'
if [ -n "$TSV_OUT" ]; then
  {
    printf '# ICON CLASSIC SET -- tree=SCRIP %s corpus %s RT_OPT=-O0 date=%s reps=%s warmup=%s\n' "$TREE" "$CTREE" "$(date -u +%Y-%m-%d)" "$REPS" "$WARMUP"
    printf '# instrument: WALL ms and CPU ms (user+sys), MEDIAN of %s runs after %s discarded warm-ups. Spread%%=(max-min)/min on wall.\n' "$REPS" "$WARMUP"
    printf '# basis: two-number. OVERHEAD = empty Icon program per engine (wall/cpu ms): iconx %s/%s jcont %s/%s m3 %s/%s m4 %s/%s. WORK = median - OVERHEAD.\n' "${OVW[iconx]:-NA}" "${OVC[iconx]:-NA}" "${OVW[jcont]:-NA}" "${OVC[jcont]:-NA}" "${OVW[m3]:-NA}" "${OVC[m3]:-NA}" "${OVW[m4]:-NA}" "${OVC[m4]:-NA}"
    printf '# multiple = rival WORK / SCRIP m4 WORK on the SAME instrument (FASTER axis, >1 = SCRIP ahead). CEO-173: OVERHEAD >= 50%% of an arm => basis=TOTAL.\n'
    printf '# ⛔ callgrind Ir REFUSES for the jcont arm (ceo CEO-494): a JVM Ir varies ~35%% run to run. No multiple crosses instruments.\n'
    printf '# verdict: output byte-compared against an iconx cut taken AT RUN TIME. A wrong answer gets no multiple.\n'
    printf 'program\tverdict\ticonx_wall_ms\ticonx_cpu_ms\ticonx_spread_pct\tjcont_wall_ms\tjcont_cpu_ms\tjcont_spread_pct\tm3_wall_ms\tm3_cpu_ms\tm3_spread_pct\tm4_wall_ms\tm4_cpu_ms\tm4_spread_pct\ticonx_WORK_wall\tjcont_WORK_wall\tm4_WORK_wall\tbasis\tm4_x_vs_iconx\tm4_x_vs_jcont\n'
  } > "$TSV_OUT"
fi

RC=0; N=0
for p in $PROGS; do
  sin="${STDIN[$p]:-}"; av_args="${ARGS[$p]:-}"
  # ---- the oracle cut, AT RUN TIME (never a vendored .std: those are icont self-benchmark dumps) ----
  ix_av="$(build_iconx "$p")"
  if [ -z "$ix_av" ]; then printf '%-9s %-7s  ORACLE-COMPILE-FAILED -- no ground truth, nothing graded\n' "$p" "UNPROVEN"; RC=1; continue; fi
  if [ -n "$sin" ]; then OUTPUT=1 timeout "$RUN_TMO" $ix_av $av_args >"$WORK/$p.oracle.raw" 2>"$WORK/$p.oracle.err" <"$sin"
  else OUTPUT=1 timeout "$RUN_TMO" $ix_av $av_args >"$WORK/$p.oracle.raw" 2>"$WORK/$p.oracle.err" </dev/null; fi
  orc=$?
  # ⭐ WHICH GRADING MODE THIS PROGRAM GETS IS DECIDED BY THE ORACLE'S OWN OUTPUT, NOT BY A LIST.
  # Five of the six link post.icn and carry its markers; geddump comes from jcon's bmark, links nothing,
  # and its stdout IS its answer.  ⛔ The decision is made on the ORACLE's raw text and PRINTED per row,
  # because a silent fallback is the whole trap: an absent marker would make the cut empty, two empty
  # files compare equal, and every engine would PASS a program nobody ran.
  if grep -q '^\*\*\* Benchmarking with output \*\*\*$' "$WORK/$p.oracle.raw" 2>/dev/null; then
    cutmode=CUT; answer_cut "$WORK/$p.oracle.raw" > "$WORK/$p.oracle.out"
  else
    cutmode=RAW; cp "$WORK/$p.oracle.raw" "$WORK/$p.oracle.out"
  fi
  if [ "$orc" = "0" ] && [ ! -s "$WORK/$p.oracle.out" ]; then
    printf '%-9s %-7s  ORACLE-ANSWER-EMPTY (mode=%s) -- nothing to grade; a green here would be two empty files agreeing\n' "$p" "UNPROVEN" "$cutmode"; RC=1; continue
  fi
  if [ "$orc" != "0" ]; then printf '%-9s %-7s  ORACLE-RAN-RED rc=%s -- no ground truth, nothing graded\n' "$p" "UNPROVEN" "$orc"; RC=1; continue; fi
  # ---- build the other engines and grade each against that cut ----
  jc_av="$(build_jcont "$p")"; m4_av="$(build_m4 "$p")"; m3_av="$(m3_argv "$p")"
  declare -A OUTOK
  for eng in jcont m3 m4; do
    case "$eng" in jcont) av="$jc_av";; m3) av="$m3_av";; m4) av="$m4_av";; esac
    OUTOK[$eng]="-"
    [ -n "$av" ] || { OUTOK[$eng]="BUILDFAIL"; continue; }
    # ⛔ m3 takes the program's own args after `--`; the others take them bare.
    if [ "$eng" = m3 ]; then run_av="$av -- $av_args"; else run_av="$av $av_args"; fi
    if [ -n "$sin" ]; then OUTPUT=1 timeout "$RUN_TMO" $run_av >"$WORK/$p.$eng.raw" 2>"$WORK/$p.$eng.err" <"$sin"
    else OUTPUT=1 timeout "$RUN_TMO" $run_av >"$WORK/$p.$eng.raw" 2>"$WORK/$p.$eng.err" </dev/null; fi
    erc=$?
    if [ "$cutmode" = CUT ]; then answer_cut "$WORK/$p.$eng.raw" > "$WORK/$p.$eng.out"
    else cp "$WORK/$p.$eng.raw" "$WORK/$p.$eng.out"; fi
    if [ "$erc" != "0" ]; then OUTOK[$eng]="rc=$erc"
    elif [ ! -s "$WORK/$p.$eng.out" ]; then OUTOK[$eng]="EMPTY-CUT"
    elif cmp -s "$WORK/$p.$eng.out" "$WORK/$p.oracle.out"; then OUTOK[$eng]="PASS"
    else OUTOK[$eng]="DIFF"; fi
  done
  verdict="PASS"; { [ "${OUTOK[m3]}" = PASS ] && [ "${OUTOK[m4]}" = PASS ]; } || { verdict="RED"; RC=1; }
  verdict="$verdict/$cutmode"
  # ---- timing, only on arms that answered correctly (a wrong answer is never a fast answer) ----
  ixr="$(time_median "$sin" $ix_av $av_args)"
  ix_w="$(echo "$ixr"|awk '{print $1}')"; ix_c="$(echo "$ixr"|awk '{print $2}')"; ix_s="$(echo "$ixr"|awk '{print $3}')"
  jc_w=NA; jc_c=NA; jc_s="-"; if [ "${OUTOK[jcont]}" = PASS ]; then r="$(time_median "$sin" $jc_av $av_args)"; jc_w="$(echo "$r"|awk '{print $1}')"; jc_c="$(echo "$r"|awk '{print $2}')"; jc_s="$(echo "$r"|awk '{print $3}')"; fi
  m3_w=NA; m3_c=NA; m3_s="-"; if [ "${OUTOK[m3]}"    = PASS ]; then r="$(time_median "$sin" $m3_av -- $av_args)"; m3_w="$(echo "$r"|awk '{print $1}')"; m3_c="$(echo "$r"|awk '{print $2}')"; m3_s="$(echo "$r"|awk '{print $3}')"; fi
  m4_w=NA; m4_c=NA; m4_s="-"; if [ "${OUTOK[m4]}"    = PASS ]; then r="$(time_median "$sin" $m4_av $av_args)"; m4_w="$(echo "$r"|awk '{print $1}')"; m4_c="$(echo "$r"|awk '{print $2}')"; m4_s="$(echo "$r"|awk '{print $3}')"; fi
  # ---- the two-number basis and the CEO-173 refusal, per arm, on WALL ----
  work() { local t="$1" o="$2"; case "$t" in NA|''|*[!0-9]*) echo ""; return;; esac; case "$o" in ''|*[!0-9]*) echo ""; return;; esac; echo $((t - o)); }
  ixw="$(work "$ix_w" "${OVW[iconx]:-}")"; jcw="$(work "$jc_w" "${OVW[jcont]:-}")"; m4w="$(work "$m4_w" "${OVW[m4]:-}")"
  frac() { awk -v o="$1" -v t="$2" 'BEGIN{printf "%d", (t>0? o*100/t : 100)}'; }
  # ⛔⭐⭐ THE INSTRUMENT'S FLOOR IS NAMED PER ROW, BECAUSE FOUR OF THESE SIX PROGRAMS SIT ON IT.
  # /usr/bin/time reports wall to 10 ms, so a 20 ms reading is TWO TICKS and a 10 ms reading is ONE.
  # A multiple built from 20 ms against 40 ms prints as a clean 2.000x and is really "two ticks against
  # four, plus or minus a tick either way" -- which spans 1.5x to 3x. ⭐ The number is not wrong; the
  # PRECISION a bare multiple implies is, and a reader has no way to see it from the multiple alone.
  # So any row where either arm is under QUANT_TICKS ticks is labelled QNT: the figure still prints (the
  # number is the number, ceo CEO-494) and it prints wearing its own error bar. ⛔ The way off this floor
  # is a BIGGER INPUT or a self-timed hook in the program -- never a tighter-looking format.
  quant=""; qfloor=$((TICK_MS * QUANT_TICKS))
  case "$ix_w" in ''|*[!0-9]*) ;; *) [ "$ix_w" -lt "$qfloor" ] && quant="QNT:";; esac
  case "$m4_w" in ''|*[!0-9]*) ;; *) [ "$m4_w" -lt "$qfloor" ] && quant="QNT:";; esac
  basis="-"; mx=" - "; mj=" - "
  if [ -n "$m4w" ] && [ -n "$ixw" ] && [ "$m4w" -gt 0 ] && [ "$ixw" -gt 0 ]; then
    if [ "$(frac "${OVW[m4]:-0}" "$m4_w")" -ge 50 ] || [ "$(frac "${OVW[iconx]:-0}" "$ix_w")" -ge 50 ]; then
      basis="TOTAL"; mx="$quant$(perf_mult "$ix_w" "$m4_w")"
    else basis="WORK"; mx="$quant$(perf_mult "$ixw" "$m4w")"; fi
  fi
  if [ -n "$m4w" ] && [ -n "$jcw" ] && [ "$m4w" -gt 0 ] && [ "$jcw" -gt 0 ]; then
    if [ "$(frac "${OVW[m4]:-0}" "$m4_w")" -ge 50 ] || [ "$(frac "${OVW[jcont]:-0}" "$jc_w")" -ge 50 ]; then
      mj="${quant}TOTAL:$(perf_mult "$jc_w" "$m4_w")"
    else mj="$quant$(perf_mult "$jcw" "$m4w")"; fi
  elif [ "${OUTOK[jcont]}" != PASS ]; then mj="(jcont ${OUTOK[jcont]})"; fi
  case "$verdict" in PASS/*) ;; *) mx="(m3 ${OUTOK[m3]} m4 ${OUTOK[m4]})"; mj="-";; esac
  printf '%-9s %-7s %10s %10s %8s %10s %10s %8s %10s %10s %8s %10s %10s %8s %8s %14s %14s\n' \
    "$p" "$verdict" "$ix_w" "$ix_c" "$ix_s" "$jc_w" "$jc_c" "$jc_s" "$m3_w" "$m3_c" "$m3_s" "$m4_w" "$m4_c" "$m4_s" "$basis" "$mx" "$mj"
  [ -z "$TSV_OUT" ] || printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "$p" "$verdict" "$ix_w" "$ix_c" "$ix_s" "$jc_w" "$jc_c" "$jc_s" "$m3_w" "$m3_c" "$m3_s" "$m4_w" "$m4_c" "$m4_s" "${ixw:-NA}" "${jcw:-NA}" "${m4w:-NA}" "$basis" "$mx" "$mj" >> "$TSV_OUT"
  N=$((N+1))
done
echo "ICON_CLASSIC_BOARD programs=$N of $(echo $PROGS|wc -w) graded"
[ "$N" -gt 0 ] || refuse "no program measured -- a grid of nothing is not a measurement."
exit $RC
