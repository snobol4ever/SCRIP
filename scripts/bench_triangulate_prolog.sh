#!/usr/bin/env bash
# bench_triangulate_prolog.sh -- THE CROSS-PROOF for Prolog (row bench-rivals-prolog), mirroring
# bench_triangulate_snobol4.sh's architecture per ARCH-BENCH-CAMPAIGN-README-TABLES.md § THREE-ANGLE
# TRIANGULATION. Runs angle 1 (test_bench_prolog_timed.sh -- fixed TIME, live-searched iters) and angle 2
# (bench_prolog_fixed_iter.sh -- fixed ITERS, committed N, measured time) UNMODIFIED -- one authority per
# mechanism, this script does not re-implement either -- then compares the two independently-derived
# iters/s rates per kernel per engine (gnu/swi/m3/m4). AGREE within TOL_PCT (flat 10%, UNBAKED -- no
# NOISE-FLOOR.tsv exists for prolog yet; a future bake_noise_floor_prolog.sh could add the same per-row
# floor the SNOBOL4 side uses), DISAGREE otherwise. A kernel/engine either angle could not measure
# (correctness-skip, crash, DNF) is UNPROVEN, never silently dropped -- the gate
# (test_gate_bench_rivals_coverage.sh) requires every corpus kernel to be MEASURED (AGREE row here) or
# DECLARED (a named line in EXCLUDED.tsv); an UNPROVEN row satisfies neither, by design, so a kernel stuck
# at UNPROVEN must get its own EXCLUDED.tsv line naming why -- this script never writes that file itself.
#
# ⛔ AS OF THIS ROW'S FIRST RUN (2026-08-27), EXPECT ZERO AGREE ROWS FOR m3/m4: SCRIP's Prolog backend
# crashes on ANY repeated entry into a compiled user predicate (backtrack-driven, flat-sequential, or
# tail-recursive alike) -- see FINDING-2026-08-27-seat14-prolog-second-call-into-any-user-predicate-
# crashes-m3-m4.md. This is not a harness defect: the harness is correctly reporting CRASH/UNPROVEN for
# every m3/m4 cell because that is what is actually happening. Re-run this script (no code change needed)
# once the underlying compiler gap (GOAL-PROLOG-100.md PZ-4) lands -- coverage should improve automatically.
#
# THE THIRD ANGLE (disk): one direct tools/bench_rusage sample per kernel (m3, single rep, the raw
# bench/<k>.pl standalone -- every kernel is directly runnable per corpus/benchmarks/prolog/README.md's
# own convention) reads ru_inblock/ru_oublock. Expected ~0 for these in-memory kernels; nonzero is a
# finding, not folded into AGREE/DISAGREE (same role as the SNOBOL4 triangulator's disk angle).
#
# EXIT: 0 = every measured kernel/engine AGREEs (or all UNPROVEN, zero DISAGREE). 1 = at least one
# DISAGREE. 2 = REFUSED -- missing scrip/oracle/corpus, loud, never a plausible table.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"
# ⛔ THE TSV LIVES AT THE LANGUAGE ROOT, NOT IN bench/ -- test_gate_bench_rivals_coverage.sh globs
# "$CORPUS/benchmarks/$lang"/triangulation-*.tsv (non-recursive; DIR is corpus/benchmarks/prolog, one
# level above $B). A TSV written into $B would be invisible to the gate the moment a real AGREE row
# needs it (found live, this row, before it could bite -- both dirs happened to be interchangeable for
# an EXCLUDED-everything run, which is why it didn't fail loudly the first time).
PDIR="${PROLOG_DIR:-$S4E/corpus/benchmarks/prolog}"
TOL="${TOL_PCT:-10}"
. "$HERE/lib_perf_fmt.sh" 2>/dev/null || { echo "⛔ REFUSED: cannot load lib_perf_fmt.sh -- the ONE authority for printing a multiple (s266)." >&2; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSED: scrip not built ($SCRIP)." >&2; exit 2; }
[ -d "$B" ] || { echo "⛔ REFUSED: bench corpus missing ($B)." >&2; exit 2; }
WRAP="$ROOT/tools/bench_rusage"; [ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "⛔ REFUSED: bench_rusage failed to build." >&2; exit 2; }

TS="$(date -u +%Y%m%dT%H%M%SZ)"
OUT_TSV="${OUT_TSV:-$PDIR/triangulation-$TS.tsv}"

echo "PROLOG THREE-ANGLE TRIANGULATION -- angle 1 (fixed time) vs angle 2 (fixed iters) vs disk telemetry"
echo "tolerance: flat TOL_PCT=$TOL% (UNBAKED -- no NOISE-FLOOR.tsv for prolog yet)"
echo

# KERNELS, if set by the caller, restricts angle 1's OWN correctness-recheck+search to this allowlist too
# (not just angle 2) -- useful to re-verify a known-live subset fast without re-walking the whole corpus's
# correctness gate (which, on a corpus this broken, spends most of its wall-clock timing out on kernels
# already known to crash). Unset (bare use) keeps discovering the live set from the full corpus each run.
A1_OUT="$(KERNELS="${KERNELS:-}" bash "$HERE/test_bench_prolog_timed.sh" 2>/dev/null)"
# angle 1's own correctness gate names the live kernel set (bench/ with .expected, minus correctness-skips) --
# angle 2 is restricted to exactly that set so it never wastes wall-clock re-timing a kernel angle 1 already
# knows is unmeasurable, and the two tables describe the identical kernel set by construction.
measured_kernels=$(printf '%s\n' "$A1_OUT" | awk '/^-{5,}/{started=1;next} started&&NF==0{started=0} started&&NF>=6{print $1}')
A2_OUT=""
if [ -n "$measured_kernels" ]; then
  A2_OUT="$(KERNELS="$(printf '%s' "$measured_kernels" | tr '\n' ' ')" bash "$HERE/bench_prolog_fixed_iter.sh" 2>/dev/null)"
fi

# state-machine parse, same shape as bench_triangulate_snobol4.sh's parse_angle1/2: start on the dashes
# separator line, stop on the first blank line after -- NEVER a fixed line-number offset (a trailing
# "CHECK RESULT: ok=.. bad=.." summary line has enough whitespace-separated fields to slip past a bare
# NF>=ncol filter and get misread as a fake kernel named "CHECK"; a hardcoded NR>N is equally fragile
# against either script's header growing by a line).
parse() { awk -v ncol="$1" '/^-{5,}/{started=1;next} started&&NF==0{started=0} started&&NF>=ncol{print}' ; }
dehuman() { awk -v v="$1" 'BEGIN{ if (v=="NA"||v=="-"||v=="") {print ""; exit} print v+0 }'; }

declare -A A1 A2 KSEEN
while read -r k g s m3 m4 rest; do
  [ -z "$k" ] && continue
  A1["$k:gnu"]="$g"; A1["$k:swi"]="$s"; A1["$k:m3"]="$m3"; A1["$k:m4"]="$m4"; KSEEN["$k"]=1
done < <(printf '%s\n' "$A1_OUT" | parse 5)
while read -r k n g s m3 m4 rest; do
  [ -z "$k" ] && continue
  A2["$k:gnu"]="$g"; A2["$k:swi"]="$s"; A2["$k:m3"]="$m3"; A2["$k:m4"]="$m4"; KSEEN["$k"]=1
done < <(printf '%s\n' "$A2_OUT" | parse 6)

{
  echo -e "# triangulation TSV -- $TS -- TOL_PCT=$TOL (flat, UNBAKED) -- never hand-edit, regenerate via bench_triangulate_prolog.sh"
  echo -e "kernel\tengine\tangle1_rate\tangle2_rate\tratio\tverdict\tdisk_inblock\tdisk_oublock"
} > "$OUT_TSV"

kernels=$(printf '%s\n' "${!KSEEN[@]}" | sort)
any_disagree=0
for k in $kernels; do
  ib=""; ob=""
  if [ -f "$B/$k.pl" ]; then
    dline=$("$WRAP" "$SCRIP" --run "$B/$k.pl" < /dev/null 2>&1 >/dev/null | grep '^BENCH_RUSAGE:' | tail -1)
    ib=$(echo "$dline" | grep -oE 'inblock=[0-9]+' | cut -d= -f2); ob=$(echo "$dline" | grep -oE 'oublock=[0-9]+' | cut -d= -f2)
  fi
  diskflag=""; { [ -n "$ib" ] && [ "$ib" -gt 0 ] 2>/dev/null; } && diskflag=" disk(inblock=$ib)"

  row_bits=""
  for eng in gnu swi m3 m4; do
    r1=$(dehuman "${A1["$k:$eng"]:-}"); r2=$(dehuman "${A2["$k:$eng"]:-}")
    if [ -n "$r1" ] && [ -n "$r2" ] && [ "$r1" != "0" ]; then
      verdict=$(awk -v a="$r1" -v b="$r2" -v t="$TOL" 'BEGIN{ ratio=b/a; lo=(100-t)/100; hi=(100+t)/100; print (ratio>=lo && ratio<=hi) ? "AGREE" : "DISAGREE" }')
      ratio=$(awk -v a="$r1" -v b="$r2" 'BEGIN{printf "%.4f", b/a}')
      [ "$verdict" = DISAGREE ] && any_disagree=1
      row_bits="$row_bits $eng=$verdict(${ratio}x)"
      printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$k" "$eng" "$r1" "$r2" "$ratio" "$verdict" "${ib:-}" "${ob:-}" >> "$OUT_TSV"
    else
      row_bits="$row_bits $eng=UNPROVEN"
      printf '%s\t%s\t%s\t%s\t\tUNPROVEN\t%s\t%s\n' "$k" "$eng" "${r1:-NA}" "${r2:-NA}" "${ib:-}" "${ob:-}" >> "$OUT_TSV"
    fi
  done
  printf '%-14s%s%s\n' "$k" "$row_bits" "$diskflag"
done

echo
echo "-- FACT-RULE grid: m3 vs gnu, m3 vs swi, m4 vs gnu, m4 vs swi (angle 1 numbers; rate metric, axis named once here) --"
for k in $kernels; do
  rg=$(dehuman "${A1["$k:gnu"]:-}"); rs=$(dehuman "${A1["$k:swi"]:-}"); r3=$(dehuman "${A1["$k:m3"]:-}"); r4=$(dehuman "${A1["$k:m4"]:-}")
  [ -n "$rg" ] && [ -n "$r3" ] && perf_row "$k  m3 vs gnu" "$r3" "$rg"
  [ -n "$rs" ] && [ -n "$r3" ] && perf_row "$k  m3 vs swi" "$r3" "$rs"
  [ -n "$rg" ] && [ -n "$r4" ] && perf_row "$k  m4 vs gnu" "$r4" "$rg"
  [ -n "$rs" ] && [ -n "$r4" ] && perf_row "$k  m4 vs swi" "$r4" "$rs"
done
[ -z "$kernels" ] && echo "  (no kernel had both angle-1 and angle-2 numeric rates for any SCRIP engine this run)"

echo
echo "TSV: $OUT_TSV"
if [ "$any_disagree" -eq 1 ]; then
  echo "⛔ DISAGREE present -- VOID: do not publish or cite those kernel/engine cells until re-measured."
  exit 1
fi
echo "no DISAGREE (measured cells, if any, all AGREE; UNPROVEN cells need an EXCLUDED.tsv line, not a citation)."
exit 0
