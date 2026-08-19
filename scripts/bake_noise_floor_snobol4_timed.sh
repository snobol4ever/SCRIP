#!/usr/bin/env bash
# bake_noise_floor_snobol4_timed.sh -- measure and record the RUN-TO-RUN
# dispersion of every (benchmark, engine) row in the TIME-based family.
#
# ⛔ WHY THIS EXISTS.  A benchmark instrument that does not know its own noise
# floor cannot tell a regression from weather.  s148 convicted the md5 blast
# -radius sweep for exactly this.  The trap repeated itself here in a subtler
# form: the floor was first measured on ONE kernel (arith_loop, cv 1.3%) and
# was about to be written into the runner as a GLOBAL "~4%" constant.  Measured
# per row, the real spread is 0.7% .. 37.7% -- the floor is a property of the
# (KERNEL, ENGINE) PAIR, not of the harness.  Generalising one row's floor to
# the suite is the same error in new clothes, so the floor is baked per row.
#
# Output: corpus/benchmarks/snobol4/NOISE-FLOOR.tsv
#   bench  engine  reps  mean_per_s  cv_pct  maxmin  min_detectable_pct
# min_detectable_pct = 3*cv, the difference below which a single-run delta on
# that row must NOT be called real.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
SBL="${SBL:-$S4A/x64/bin/sbl}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"  # BM-ONE (s153): promoted, see test_bench_snobol4_timed.sh
REPS="${REPS:-5}"; T="${TIMEOUT:-60}"; ENGINES="${ENGINES:-sbl m3 m4}"
APPEND="${APPEND:-0}"; NOHUGE="${NOHUGE:-1}"
HEAP="${HEAP:-1024}"   # BM-3: the floor must be baked in a GC-FREE window or it measures the stall lottery, not dispersion (see test_bench_snobol4_timed.sh)
OUT="${OUT:-$B/NOISE-FLOOR.tsv}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if [ "$APPEND" != 1 ]; then
{
  echo "# TIME-based SNOBOL4 benchmarks -- MEASURED run-to-run noise floor"
  echo "# baked $(date -u +%Y-%m-%dT%H:%M:%SZ) by scripts/$(basename "$0")  reps=$REPS"
  echo "# SCRIP_NOHUGE=$NOHUGE  SCRIP_HEAP_MB=$HEAP  (arena sized past the window so no"
  echo "#   collection fires inside it -- a GC row measures an ~835ms stall, not dispersion)"
  echo "# ⛔ THE ALLOCATING ROWS' DISPERSION WAS THE GC STALL LOTTERY, NOT THP (BM-3 correction):"
  echo "#   whether an ~835ms regeneration lands inside the window is a coin flip, and it was being"
  echo "#   baked as variance.  GC-free, table_access m3 goes cv 12.4%->1.9% and array_sum 10.9%->0.5%."
  echo "# min_detectable_pct = 3*cv; a single-run delta smaller than this is WEATHER."
  printf "bench\tengine\tthp\treps\tmean_per_s\tcv_pct\tmaxmin\tmin_detectable_pct\n"
} > "$OUT"
fi
for sno in "$B"/*.sno; do
  [ -e "$sno" ] || continue
  grep -q "INCLUDE '.*harness.inc'" "$sno" || continue   # BM-ONE (s153): legacy programs share the directory; only harness.inc programs emit iters:/ms:
  s=$(basename "${sno%.sno}")
  # build the mode-4 program ONCE, not once per rep
  m4ok=0
  if [ -x "$SCRIP" ]; then
    "$SCRIP" --compile "$sno" > "$W/$s.s" 2>/dev/null
    [ -s "$W/$s.s" ] && gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" \
        -o "$W/$s.prog" 2>/dev/null && m4ok=1
  fi
  for eng in $ENGINES; do
    if [ "$eng" = sbl ]; then thparm="n/a"; else thparm="nohuge=$NOHUGE"; fi
    : > "$W/r.txt"
    for _ in $(seq 1 "$REPS"); do
      case "$eng" in
        sbl) o=$(timeout "$T" "$SBL" -b "$sno" 2>/dev/null </dev/null) ;;
        m3)  o=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" timeout "$T" "$SCRIP" --run "$sno" 2>/dev/null </dev/null) ;;
        m4)  [ "$m4ok" = 1 ] || { o=""; }; [ "$m4ok" = 1 ] && o=$(cd "$W" && SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" timeout "$T" "./$s.prog" 2>/dev/null </dev/null) ;;
      esac
      it=$(printf '%s\n' "$o" | sed -n 's/^iters: //p')
      ms=$(printf '%s\n' "$o" | sed -n 's/^ms: //p')
      awk -v i="${it:-0}" -v m="${ms:-0}" 'BEGIN{ if(m+0>0) printf "%.3f\n", i/m*1000 }' >> "$W/r.txt"
    done
    n=$(wc -l < "$W/r.txt")
    if [ "$n" -lt 2 ]; then
      printf "%s\t%s\t%s\t%s\tNA\tNA\tNA\tNA\n" "$s" "$eng" "$thparm" "$n" >> "$OUT"; continue
    fi
    awk -v b="$s" -v e="$eng" -v h="$thparm" '{a[NR]=$1; t+=$1}
      END{ m=t/NR; for(i=1;i<=NR;i++){d=a[i]-m; v+=d*d} v=sqrt(v/NR)
           lo=a[1]; hi=a[1]; for(i=1;i<=NR;i++){ if(a[i]<lo)lo=a[i]; if(a[i]>hi)hi=a[i] }
           cv=100*v/m
           printf "%s\t%s\t%s\t%d\t%.0f\t%.1f\t%.2f\t%.1f\n", b, e, h, NR, m, cv, (lo>0?hi/lo:0), 3*cv }' \
      "$W/r.txt" >> "$OUT"
  done
  echo "  floored $s"
done
echo
column -t -s $'\t' "$OUT" 2>/dev/null || cat "$OUT"
