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
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
SBL="${SBL:-/home/claude/x64/bin/sbl}"
B="${BENCH_DIR:-/home/claude/corpus/benchmarks/snobol4}"  # BM-ONE (s153): promoted, see test_bench_snobol4_timed.sh
REPS="${REPS:-5}"; T="${TIMEOUT:-60}"; ENGINES="${ENGINES:-sbl m3 m4}"
APPEND="${APPEND:-0}"; THP="${SCRIP_NOHUGE:-0}"
OUT="${OUT:-$B/NOISE-FLOOR.tsv}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if [ "$APPEND" != 1 ]; then
{
  echo "# TIME-based SNOBOL4 benchmarks -- MEASURED run-to-run noise floor"
  echo "# baked $(date -u +%Y-%m-%dT%H:%M:%SZ) by scripts/$(basename "$0")  reps=$REPS"
  echo "# SCRIP_NOHUGE=${SCRIP_NOHUGE:-unset}  (transparent huge pages dominate the"
  echo "#   aggregate rows: table_access_t cv 37.7% -> 4.0% with SCRIP_NOHUGE=1)"
  echo "# min_detectable_pct = 3*cv; a single-run delta smaller than this is WEATHER."
  printf "bench\tengine\tthp\treps\tmean_per_s\tcv_pct\tmaxmin\tmin_detectable_pct\n"
} > "$OUT"
fi
for sno in "$B"/*.sno; do
  [ -e "$sno" ] || continue
  s=$(basename "${sno%.sno}")
  # build the mode-4 program ONCE, not once per rep
  m4ok=0
  if [ -x "$SCRIP" ]; then
    "$SCRIP" --compile "$sno" > "$W/$s.s" 2>/dev/null
    [ -s "$W/$s.s" ] && gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" \
        -o "$W/$s.prog" 2>/dev/null && m4ok=1
  fi
  for eng in $ENGINES; do
    if [ "$eng" = sbl ]; then thparm="n/a"; else thparm="nohuge=$THP"; fi
    : > "$W/r.txt"
    for _ in $(seq 1 "$REPS"); do
      case "$eng" in
        sbl) o=$(timeout "$T" "$SBL" -b "$sno" 2>/dev/null </dev/null) ;;
        m3)  o=$(timeout "$T" "$SCRIP" --run "$sno" 2>/dev/null </dev/null) ;;
        m4)  [ "$m4ok" = 1 ] || { o=""; }; [ "$m4ok" = 1 ] && o=$(cd "$W" && timeout "$T" "./$s.prog" 2>/dev/null </dev/null) ;;
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
