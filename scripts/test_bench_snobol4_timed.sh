#!/usr/bin/env bash
# test_bench_snobol4_timed.sh -- run the TIME-BASED SNOBOL4 benchmark family and
# report THROUGHPUT (iterations per second) for sbl / scrip m3 / scrip m4.
#
# The legacy runner (test_bench_snobol4_modes.sh) reports wall(ms) for a fixed
# iteration count.  This one reports iterations for a fixed time budget, so:
#   * the numbers are directly comparable ACROSS engines (a ratio, not a delta);
#   * the suite's wall cost is bounded by construction (budget x programs x engines);
#   * a resolution-starved reading ("10 ms") cannot happen -- the elapsed window
#     is always ~BUDGET ms regardless of how fast the engine is.
# CORRECTNESS is still gated: every program's phase-1 "check:" line is
# deterministic and is diffed against the sibling .ref.  Only the measurement
# lines (iters:/ms:) are stripped before the diff.
#
# ⛔ THE NOISE FLOOR IS NOT GLOBAL.  It is a property of the (KERNEL, ENGINE,
# THP-arm) triple and is BAKED PER ROW in NOISE-FLOOR.tsv beside this corpus by
# scripts/bake_noise_floor_snobol4_timed.sh.  Measured range: 0.2% .. 34.6% cv.
# An earlier draft of this file carried a single global "~4%" constant derived
# from one kernel (arith_loop); that is the s148 instrument error in new
# clothes and it is deleted.  The runner prints each row's min-detectable
# difference; a delta smaller than that is WEATHER, not a regression.
#
# ⛔ MEASUREMENT CONDITION: SCRIP engines run with SCRIP_NOHUGE=1 by DEFAULT.
# Transparent huge pages make every allocating row unmeasurable in the shipping
# arm (table_access_t m3: cv 26.9%, min-detectable 80.7% -- you cannot see a 2x
# regression) AND cost throughput outright (2675/s -> 6042/s, 2.26x, with THP
# off).  Set NOHUGE=0 to reproduce the shipping-arm instability.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
SBL="${SBL:-/home/claude/x64/bin/sbl}"
B="${BENCH_DIR:-/home/claude/corpus/benchmarks/snobol4/timed}"
T="${TIMEOUT:-60}"; REPS="${REPS:-1}"; NOHUGE="${NOHUGE:-1}"
FLOORTSV="${FLOORTSV:-$B/NOISE-FLOOR.tsv}"
ENGINES="${ENGINES:-sbl m3 m4}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$B" ]     || { echo "SKIP timed bench corpus missing ($B)"; exit 0; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
rate() { awk -v i="$1" -v m="$2" 'BEGIN{ if(m+0>0) printf "%.0f", i/m*1000; else printf "NA" }'; }
human() { awk -v v="$1" 'BEGIN{ if(v=="NA"){print "NA"; exit}
  if(v>=1e9) printf "%.2fG", v/1e9; else if(v>=1e6) printf "%.1fM", v/1e6;
  else if(v>=1e3) printf "%.1fK", v/1e3; else printf "%d", v }'; }
# ---- one measured run; echoes "iters ms check" ----------------------------
run1() {
  local eng="$1" sno="$2" s; s=$(basename "${sno%.sno}")
  case "$eng" in
    sbl) [ -x "$SBL" ] || { echo "- - ORACLE-MISSING"; return; }
         out=$(timeout "$T" "$SBL" -b "$sno" 2>/dev/null </dev/null) ;;
    m3)  out=$(SCRIP_NOHUGE="$NOHUGE" timeout "$T" "$SCRIP" --run "$sno" 2>/dev/null </dev/null) ;;
    m4)  "$SCRIP" --compile "$sno" > "$W/$s.s" 2>/dev/null
         if [ ! -s "$W/$s.s" ] || ! gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm \
              -Wl,-rpath,"$RT" -o "$W/$s.prog" 2>/dev/null; then echo "- - BUILD-ERR"; return; fi
         out=$(cd "$W" && SCRIP_NOHUGE="$NOHUGE" timeout "$T" "./$s.prog" 2>/dev/null </dev/null) ;;
  esac
  local it ms ck
  it=$(sed -n 's/^iters: //p' <<<"$out"); ms=$(sed -n 's/^ms: //p'   <<<"$out")
  ck=$(sed -n 's/^check: //p' <<<"$out")
  [ -n "$it" ] || { echo "- - CRASH"; return; }
  echo "$it $ms ${ck:-NOCHECK}"
}
# ---- best of REPS (max throughput = least interference) --------------------
best() {
  local eng="$1" sno="$2" bi=0 bm=0 ck="" r i m c
  for _ in $(seq 1 "$REPS"); do
    r=$(run1 "$eng" "$sno"); i=$(awk '{print $1}' <<<"$r"); m=$(awk '{print $2}' <<<"$r"); c=$(awk '{print $3}' <<<"$r")
    [ "$i" = "-" ] && { echo "- - $c"; return; }
    ck="$c"
    if [ "$(awk -v a="$i" -v b="$m" -v x="$bi" -v y="$bm" 'BEGIN{print (b>0 && (y<=0 || a/b > x/y))?1:0}')" = 1 ]; then bi=$i; bm=$m; fi
  done
  echo "$bi $bm $ck"
}
echo "TIME-BASED SNOBOL4 BENCHMARKS -- fixed time budget, iterations counted"
echo "engines: $ENGINES   reps: $REPS   corpus: $B"
echo "measurement condition: SCRIP_NOHUGE=$NOHUGE (sbl unaffected -- separate binary)"
if [ -f "$FLOORTSV" ]; then echo "noise floor: $FLOORTSV (per-row; min-det = 3*cv)"
else echo "noise floor: NOT BAKED -- run scripts/bake_noise_floor_snobol4_timed.sh"; fi
echo
printf "%-20s %12s %12s %12s   %8s %8s %9s  %s\n" BENCHMARK "sbl/s" "m3/s" "m4/s" "m3:sbl" "m4:m3" "min-det" "check"
printf "%-20s %12s %12s %12s   %8s %8s %9s  %s\n" "--------------------" "------------" "------------" "------------" "--------" "--------" "---------" "-----"
tot_ok=0; tot_bad=0
for sno in "$B"/*.sno; do
  [ -e "$sno" ] || continue
  s=$(basename "${sno%.sno}"); ref="${sno%.sno}.ref"
  declare -A R=(); declare -A C=()
  for eng in $ENGINES; do
    res=$(best "$eng" "$sno")
    i=$(awk '{print $1}' <<<"$res"); m=$(awk '{print $2}' <<<"$res"); c=$(cut -d' ' -f3- <<<"$res")
    if [ "$i" = "-" ]; then R[$eng]="NA"; C[$eng]="$c"; else R[$eng]=$(rate "$i" "$m"); C[$eng]="$c"; fi
  done
  # correctness: all engines must agree on the check line, and match the .ref
  ckstat="ok"; base=""
  for eng in $ENGINES; do
    [ "${C[$eng]}" = "CRASH" ] || [ "${C[$eng]}" = "BUILD-ERR" ] && { ckstat="${C[$eng]}"; break; }
    [ -z "$base" ] && base="${C[$eng]}"
    [ "${C[$eng]}" = "$base" ] || ckstat="DISAGREE"
  done
  if [ "$ckstat" = ok ] && [ -f "$ref" ]; then
    grep -q "^check: $base\$" "$ref" 2>/dev/null || ckstat="REF-DIFF"
  fi
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1)) || tot_bad=$((tot_bad+1))
  sp3=$(awk -v a="${R[m3]:-NA}" -v b="${R[sbl]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  sp4=$(awk -v a="${R[m4]:-NA}" -v b="${R[m3]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  md="-"
  if [ -f "$FLOORTSV" ]; then
    md=$(awk -F'\t' -v b="$s" -v h="nohuge=$NOHUGE" '$1==b && $2=="m3" && $3==h {printf "%s%%", $8}' "$FLOORTSV")
    [ -n "$md" ] || md="-"
  fi
  printf "%-20s %12s %12s %12s   %8s %8s %9s  %s\n" "$s" \
    "$(human "${R[sbl]:-NA}")" "$(human "${R[m3]:-NA}")" "$(human "${R[m4]:-NA}")" "$sp3" "$sp4" "$md" "$ckstat"
done
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad"
[ "$tot_bad" -eq 0 ]
