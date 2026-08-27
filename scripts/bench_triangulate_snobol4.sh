#!/usr/bin/env bash
# bench_triangulate_snobol4.sh -- THE CROSS-PROOF, item 3 of row bench-triangulation-3angle
# (Lon 2026-08-24, in-chat, verbatim in substance: "I want to cross prove ... All of it wrapped in
# the perf-like process measuring CPU time, disk time, elapsed time -- program-reported numbers
# triangulated with the PERF telemetry. I'm trusting these numbers."). Full law text:
# .github/ARCH-BENCH-CAMPAIGN-README-TABLES.md § THREE-ANGLE TRIANGULATION.
#
# WHAT THIS DOES: runs angle 1 (test_bench_snobol4_timed.sh -- fixed TIME, iterations counted) and
# angle 2 (bench_snobol4_fixed_iter.sh -- fixed ITERATIONS, time measured) unmodified -- ONE
# AUTHORITY per mechanism, this script does not re-implement either -- then, per kernel per engine
# (sbl/m3/m4), compares the two independently-derived iters/s rates. AGREE if they land within
# TOL_PCT of each other (default 10%, i.e. ratio in [0.90, 1.10]); DISAGREE otherwise. A DISAGREE
# VOIDS that kernel's numbers for publication -- the run still completes and the raw readings still
# land in the TSV (the never-redo record), but the verdict says loudly not to cite them.
#
# THE THIRD ANGLE (disk): one direct tools/bench_rusage sample per kernel (m3, single rep, the raw
# .sno run standalone -- every corpus/benchmarks/snobol4 kernel is directly runnable per its own
# header) reads ru_inblock/ru_oublock (added to bench_rusage.c by this same row, item 2). Expected
# ~0 for these in-memory kernels; nonzero is reported as a finding, not folded into AGREE/DISAGREE --
# block-I/O has no "rate" to cross-prove against, it is a contamination signal, same role as nivcsw.
#
# EXIT: 0 = every kernel AGREE (or UNPROVEN with zero DISAGREE). 1 = at least one DISAGREE present
# (published grid printed anyway, loudly marked). 2 = REFUSED -- missing oracle/binary/corpus, loud,
# never a plausible table (SCRIP unbuilt, clean SPITBOL oracle absent/not -bf-capable, bench_rusage
# fails to build). Never silently narrows the kernel set -- a kernel neither angle could measure is
# UNPROVEN and named, not dropped.
#
# --check-shape: proves the MECHANISM cheaply -- one kernel (default array_sum, override with
# CHECK_SHAPE_KERNEL) run end-to-end through both angles + the disk sample, verdict line printed.
# Does not touch the committed TSV (writes to a tmp file instead) -- a smoke test is not a
# measurement campaign and should not proliferate committed data files.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"          # the REAL corpus dir -- always the SCALE.tsv home
TOL="${TOL_PCT:-10}"
CHECK_SHAPE=0
for a in "$@"; do case "$a" in --check-shape) CHECK_SHAPE=1 ;; esac; done
KERNEL="${CHECK_SHAPE_KERNEL:-array_sum}"

. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSED: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200)." >&2; exit 2; }
. "$HERE/lib_perf_fmt.sh" 2>/dev/null || { echo "⛔ REFUSED: cannot load lib_perf_fmt.sh -- the ONE authority for printing a multiple (s266)." >&2; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSED: scrip not built ($SCRIP) -- a table printed without it would be plausible and false." >&2; exit 2; }
SBL="$(sbl_clean_bin)" || { echo "⛔ REFUSED: clean SPITBOL benchmark oracle missing or not -bf-capable -- see lib_oracle_flags.sh." >&2; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED: benchmark corpus missing ($B)." >&2; exit 2; }
WRAP="$ROOT/tools/bench_rusage"
if [ ! -x "$WRAP" ]; then gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build." >&2; exit 2; }; fi
# item 2's disk columns are load-bearing for this script's own telemetry angle -- refuse rather than
# silently reporting inblock/oublock as empty if a stale pre-item-2 binary is sitting in tools/.
WRAP_SELFTEST="$(mktemp)"; "$WRAP" true >"$WRAP_SELFTEST" 2>&1
if ! grep -q 'inblock=' "$WRAP_SELFTEST"; then
  rm -f "$WRAP_SELFTEST"
  echo "⛔ REFUSED: $WRAP predates the inblock/oublock fields (item 2) -- rebuild: gcc -O2 -o $WRAP $ROOT/tools/bench_rusage.c" >&2
  exit 2
fi
rm -f "$WRAP_SELFTEST"

RUN_B="$B"
CLEANUP=""
if [ "$CHECK_SHAPE" -eq 1 ]; then
  [ -f "$B/$KERNEL.sno" ] || { echo "⛔ REFUSED: --check-shape kernel '$KERNEL' has no $B/$KERNEL.sno" >&2; exit 2; }
  RUN_B="$(mktemp -d)"; CLEANUP="$RUN_B"
  for ext in sno ref dat xfail; do [ -f "$B/$KERNEL.$ext" ] && cp "$B/$KERNEL.$ext" "$RUN_B/"; done
  [ -f "$B/harness.inc" ] && cp "$B/harness.inc" "$RUN_B/"   # bench_wrap.sh looks beside the program for this
fi
trap '[ -n "$CLEANUP" ] && rm -rf "$CLEANUP"' EXIT

TS="$(date -u +%Y%m%dT%H%M%SZ)"
if [ "$CHECK_SHAPE" -eq 1 ]; then OUT_TSV="${OUT_TSV:-$(mktemp)}"; else OUT_TSV="${OUT_TSV:-$B/triangulation-$TS.tsv}"; fi

echo "THREE-ANGLE TRIANGULATION -- angle 1 (fixed time) vs angle 2 (fixed iters) vs disk telemetry"
echo "tolerance: AGREE iff angle2/angle1 in [$(awk -v t="$TOL" 'BEGIN{printf "%.2f", (100-t)/100}'), $(awk -v t="$TOL" 'BEGIN{printf "%.2f", (100+t)/100}')]  (TOL_PCT=$TOL)"
[ "$CHECK_SHAPE" -eq 1 ] && echo "mode: --check-shape (kernel=$KERNEL, RUN_B=$RUN_B, OUT_TSV is scratch, not committed)"
echo

# ---- run both angles, unmodified, against RUN_B --------------------------------------------------
A1_OUT="$(BENCH_DIR="$RUN_B" SCALETSV="$B/SCALE.tsv" bash "$HERE/test_bench_snobol4_timed.sh" 2>/dev/null)"
A2_OUT="$(BENCH_DIR="$RUN_B" SCALETSV="$B/SCALE.tsv" bash "$HERE/bench_snobol4_fixed_iter.sh" 2>/dev/null)"

# ---- parse each angle's printed table into kernel<TAB>engine<TAB>human-rate ----------------------
parse_angle1() { awk '/^-{5,}/{started=1;next} started&&NF==0{started=0} started{print $1"\tsbl\t"$2; print $1"\tm3\t"$3; print $1"\tm4\t"$4}'; }
parse_angle2() { awk '/^-{5,}/{started=1;next} started&&NF==0{started=0} started{print $1"\tsbl\t"$3; print $1"\tm3\t"$4; print $1"\tm4\t"$5}'; }
# ⛔ "Correctness gates the timing" (ARCH-BENCH-CAMPAIGN-README-TABLES.md § THREE-ANGLE, binding): each
# angle already gates its OWN check column (all engines agree + match sbl); a kernel whose rate is
# numeric but whose check column is not ok/ok(x-eng) ran, but its answer is not trusted -- a rate
# agreement on a WRONG answer must never read as AGREE. Last field ($NF) is the check column in both.
parse_angle1_check() { awk '/^-{5,}/{started=1;next} started&&NF==0{started=0} started{print $1"\t"$NF}'; }
parse_angle2_check() { awk '/^-{5,}/{started=1;next} started&&NF==0{started=0} started{print $1"\t"$NF}'; }
dehuman() { awk -v v="$1" 'BEGIN{
  if (v=="NA"||v=="-"||v=="") {print ""; exit}
  u=substr(v,length(v),1)
  if (u=="G") {print substr(v,1,length(v)-1)*1e9; exit}
  if (u=="M") {print substr(v,1,length(v)-1)*1e6; exit}
  if (u=="K") {print substr(v,1,length(v)-1)*1e3; exit}
  print v+0 }'; }

declare -A A1 A2 A1C A2C KSEEN
while IFS=$'\t' read -r k e v; do A1["$k:$e"]="$v"; KSEEN["$k"]=1; done < <(printf '%s\n' "$A1_OUT" | parse_angle1)
while IFS=$'\t' read -r k e v; do A2["$k:$e"]="$v"; KSEEN["$k"]=1; done < <(printf '%s\n' "$A2_OUT" | parse_angle2)
while IFS=$'\t' read -r k c; do A1C["$k"]="$c"; done < <(printf '%s\n' "$A1_OUT" | parse_angle1_check)
while IFS=$'\t' read -r k c; do A2C["$k"]="$c"; done < <(printf '%s\n' "$A2_OUT" | parse_angle2_check)
checkok() { case "$1" in ok|ok\(x-eng\)) return 0 ;; *) return 1 ;; esac; }

{
  echo -e "# triangulation TSV -- $TS -- TOL_PCT=$TOL -- never hand-edit, regenerate via bench_triangulate_snobol4.sh"
  echo -e "kernel\tengine\tangle1_rate\tangle2_rate\tratio\tverdict\tdisk_inblock\tdisk_oublock"
} > "$OUT_TSV"

kernels=$(printf '%s\n' "${!KSEEN[@]}" | sort)
any_disagree=0
for k in $kernels; do
  # ---- angle 3 first: direct disk telemetry sample (m3, single rep, raw standalone .sno) ----
  ib=""; ob=""
  if [ -f "$RUN_B/$k.sno" ]; then
    dline=$("$WRAP" "$SCRIP" --run "$RUN_B/$k.sno" < /dev/null 2>&1 >/dev/null | grep '^BENCH_RUSAGE:' | tail -1)
    ib=$(echo "$dline" | grep -oE 'inblock=[0-9]+' | cut -d= -f2)
    ob=$(echo "$dline" | grep -oE 'oublock=[0-9]+' | cut -d= -f2)
  fi
  parts=""
  [ -n "$ib" ] && [ "$ib" -gt 0 ] && parts="inblock=$ib"
  [ -n "$ob" ] && [ "$ob" -gt 0 ] && parts="${parts:+$parts,}oublock=$ob"
  diskflag=""; [ -n "$parts" ] && diskflag=" disk($parts)"

  # ---- correctness gates the timing (binding, ARCH-BENCH-CAMPAIGN-README-TABLES.md § THREE-ANGLE):
  # a numeric rate on top of a failed/disagreeing check column is a wrong answer, never a fast one --
  # gate BEFORE rate agreement, don't let a coincidentally-matching rate read as AGREE.
  a1c="${A1C["$k"]:-MISSING}"; a2c="${A2C["$k"]:-MISSING}"
  if ! checkok "$a1c" || ! checkok "$a2c"; then
    any_disagree=1
    printf '%s\t-\t-\t-\t-\tCHECK-FAIL\t%s\t%s\n' "$k" "$ib" "$ob" >> "$OUT_TSV"
    printf '%-20s %-9s angle1_check=%s angle2_check=%s%s\n' "$k" "CHECK-FAIL" "$a1c" "$a2c" "$diskflag"
    continue
  fi

  # ---- angle-1 vs angle-2 per engine ----
  k_disagree=0; k_measured=0; row_bits=""
  for eng in sbl m3 m4; do
    r1h="${A1["$k:$eng"]:-}"; r2h="${A2["$k:$eng"]:-}"
    r1=$(dehuman "$r1h"); r2=$(dehuman "$r2h")
    if [ -n "$r1" ] && [ -n "$r2" ] && [ "$r1" != "0" ]; then
      k_measured=1
      verdict=$(awk -v a="$r1" -v b="$r2" -v t="$TOL" 'BEGIN{ ratio=b/a; lo=(100-t)/100; hi=(100+t)/100; print (ratio>=lo && ratio<=hi) ? "AGREE" : "DISAGREE" }')
      ratio=$(awk -v a="$r1" -v b="$r2" 'BEGIN{printf "%.4f", b/a}')
      [ "$verdict" = DISAGREE ] && k_disagree=1
      row_bits="$row_bits $eng=$verdict(${ratio}x)"
      printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$k" "$eng" "$r1" "$r2" "$ratio" "$verdict" "$ib" "$ob" >> "$OUT_TSV"
    else
      row_bits="$row_bits $eng=UNPROVEN"
      printf '%s\t%s\t%s\t%s\t\tUNPROVEN\t%s\t%s\n' "$k" "$eng" "${r1:-NA}" "${r2:-NA}" "$ib" "$ob" >> "$OUT_TSV"
    fi
  done
  if [ "$k_measured" -eq 0 ]; then kverdict="UNPROVEN"; elif [ "$k_disagree" -eq 1 ]; then kverdict="DISAGREE"; any_disagree=1; else kverdict="AGREE"; fi
  printf '%-20s %-9s%s%s\n' "$k" "$kverdict" "$row_bits" "$diskflag"
done

echo
# ⛔ lib_perf_fmt.sh's perf_mult(reference,ours) is authored for COST metrics (lower=better: time,
# instructions) -- reference/ours > 1 means we're faster. sbl/s, m3/s, m4/s are RATE metrics (higher
# is better), the reciprocal polarity, so the two call args are swapped here (ours first, reference
# second) to keep ">= 1.00x GREEN means we're faster" true for a rate too. Same m3-vs-sbl, m4-vs-m3
# comparisons the two angle scripts already print as sp3/sp4 (hand-rolled there); this is angle 1's
# numbers through the ONE authority instead.
echo "-- FACT-RULE grid: m3 vs sbl, m4 vs m3 (angle 1 numbers; axis named once per row group here) --"
for k in $kernels; do
  r1sbl=$(dehuman "${A1["$k:sbl"]:-}"); r1m3=$(dehuman "${A1["$k:m3"]:-}"); r1m4=$(dehuman "${A1["$k:m4"]:-}")
  [ -n "$r1sbl" ] && [ -n "$r1m3" ] && perf_row "$k  m3 vs sbl" "$r1m3" "$r1sbl"
  [ -n "$r1m3" ] && [ -n "$r1m4" ] && perf_row "$k  m4 vs m3"  "$r1m4" "$r1m3"
done

echo
echo "TSV: $OUT_TSV"
if [ "$any_disagree" -eq 1 ]; then
  echo "⛔ DISAGREE or CHECK-FAIL present -- VOID: do not publish or cite those kernels' numbers until re-measured (or their correctness fixed)."
  exit 1
fi
echo "all measured kernels AGREE (within ${TOL}%)."
exit 0
