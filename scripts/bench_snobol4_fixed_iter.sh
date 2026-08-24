#!/usr/bin/env bash
# bench_snobol4_fixed_iter.sh -- ANGLE 2 of the three-angle triangulation (Lon 2026-08-24, row
# bench-triangulation-3angle): run EXACTLY N iterations per kernel (N fixed per kernel, never a
# wall-clock deadline) and report throughput (iterations per second) for sbl / scrip m3 / scrip m4.
#
# This is the mirror of test_bench_snobol4_timed.sh (angle 1: fix TIME, count iterations). Angle 2
# fixes ITERATIONS, measures TIME -- same "rate" metric (iters/s), computed the other way, so the
# triangulator can diff angle-1's rate against angle-2's rate per kernel per engine and expect them
# to AGREE. Deliberately mirrors angle 1's plumbing (oracle flags, tools/bench_rusage external-cpu
# wrapping, XFAIL lane, UNGRADED census, correctness gate) rather than inventing a second private
# copy of any of it -- ONE AUTHORITY per mechanism (lib_oracle_flags.sh, tools/bench_rusage), same
# law this project already applies to flags and formatting.
#
# ⛔ N IS NOT COMPUTED HERE. Per-kernel iteration counts live in the COMMITTED data file
# corpus/benchmarks/snobol4/SCALE.tsv (kernel<TAB>N), chosen once so every kernel's window is
# multi-second on its FASTEST engine (a slower engine on the same N simply runs longer -- that is
# expected, not a defect) and never re-derived by hand at measurement time -- exactly the "durable
# tooling, never redo from scratch" instruction this row was minted under. Re-tune with
# scripts/calibrate_bench_scale.sh (bakes SCALE.tsv from a fresh angle-1 run) if the tree moves
# enough that a kernel's window drifts far from multi-second; do not hand-edit N without re-baking.
#
# ⛔ MEASUREMENT CONDITIONS mirror angle 1 exactly (same knobs, same reasons -- see
# test_bench_snobol4_timed.sh's own header for the full rationale): SCRIP_NOHUGE=1 by default (THP
# makes allocating rows unmeasurable), HEAP sized past the run's total allocation so gc>0 marks a row
# untrusted rather than silently averaging a stall in, and every arm is forked through
# tools/bench_rusage so the reported rate is EXTERNAL cpu(user+sys) time, never engine self-timing.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"
SCALETSV="${SCALETSV:-$B/SCALE.tsv}"
T="${TIMEOUT:-120}"; REPS="${REPS:-1}"; NOHUGE="${NOHUGE:-1}"; HEAP="${HEAP:-4096}"
# ⛔ HEAP DEFAULT DIFFERS FROM ANGLE 1's 1024MB, MEASURED NOT GUESSED: angle 2's committed N values
# target a multi-second window (vs angle 1's 500ms budget), so total allocation per run is larger by
# construction. At 1024MB, 7 of 18 kernels (array_sum/mixed_workload/roman/string_manip/
# string_pattern/table_access/table_variety) collected inside the window on first measurement;
# 4096MB cleared all 7 to gc=0 with correctness unchanged (18/18 ok). Same law as angle 1: size the
# arena past the window's allocation, don't average a stall into a rate.
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200). A private fallback would time a DIFFERENT LANGUAGE (s189: -bf is the only correct arm). Fix the checkout; do not work around this." >&2; exit 3; }
SBL="${SBL:-$(sbl_clean_bin)}"   # BENCHMARK oracle (s255): never x64/bin/sbl (monitor-IPC overhead)
SBLFLAGS="${SBLFLAGS:--s16m}"
ENGINES="${ENGINES:-sbl m3 m4}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$B" ]     || { echo "SKIP fixed-iter bench corpus missing ($B)"; exit 0; }
[ -f "$SCALETSV" ] || { echo "REFUSING: no $SCALETSV -- angle 2 needs a committed per-kernel N; run scripts/calibrate_bench_scale.sh once to bake it, then commit it. Not deriving one on the fly (that would be re-deriving from scratch on every run, exactly what this row exists to stop)." >&2; exit 3; }
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "FAIL build $WRAP" >&2; exit 1; }
NIVCSW_FLAG="${BENCH_NIVCSW_THRESHOLD:-20}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
rate() { awk -v n="$1" -v m="$2" 'BEGIN{ if(m+0>0) printf "%.0f", n/m*1000; else printf "NA" }'; }
human() { awk -v v="$1" 'BEGIN{ if(v=="NA"){print "NA"; exit}
  if(v>=1e9) printf "%.2fG", v/1e9; else if(v>=1e6) printf "%.1fM", v/1e6;
  else if(v>=1e3) printf "%.1fK", v/1e3; else printf "%d", v }'; }
scale_n() { awk -F'\t' -v k="$1" '$1==k{print $2; found=1} END{if(!found) print ""}' "$SCALETSV"; }
# ---- one measured run at fixed N; echoes "cpu_ms nivcsw gc check" (iters is the input, not the output) --
run1() {
  local eng="$1" twin="$2" s="$3" in
  in="$(dirname "$twin")/$(sed 's/-match\(-fence\)\?$//' <<<"$s").dat"; [ -f "$in" ] || in=/dev/null
  case "$eng" in
    sbl) [ -x "$SBL" ] || { echo "- - - ORACLE-MISSING"; return; }
         out=$("$WRAP" timeout "$T" "$SBL" $(sbl_lang_flags) $SBLFLAGS "$twin" 2>"$W/gc.err" <"$in") ;;
    m3)  out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 "$WRAP" timeout "$T" "$SCRIP" --run "$twin" 2>"$W/gc.err" <"$in") ;;
    m4)  "$SCRIP" --compile "$twin" > "$W/$s.s" 2>/dev/null
         if [ ! -s "$W/$s.s" ] || ! gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm \
              -Wl,-rpath,"$RT" -o "$W/$s.prog" 2>/dev/null; then echo "- - - BUILD-ERR"; return; fi
         out=$(cd "$W" && SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 "$WRAP" timeout "$T" "./$s.prog" 2>"$W/gc.err" <"$in") ;;
  esac
  local ck gc rusage_line user_us sys_us nivcsw cpu_ms
  ck=$(sed -n 's/^check: //p' <<<"$out")
  gc=$(grep -c 'regeneration #' "$W/gc.err" 2>/dev/null); gc="${gc:-0}"
  rusage_line=$(grep '^BENCH_RUSAGE:' "$W/gc.err" | tail -1)
  [ -n "$rusage_line" ] || { echo "- - - CRASH"; return; }
  user_us=$(echo "$rusage_line" | grep -oE 'user_us=[0-9]+' | cut -d= -f2)
  sys_us=$(echo "$rusage_line" | grep -oE 'sys_us=[0-9]+' | cut -d= -f2)
  nivcsw=$(echo "$rusage_line" | grep -oE 'nivcsw=[0-9]+' | cut -d= -f2); nivcsw="${nivcsw:-0}"
  cpu_ms=$(awk -v u="${user_us:-0}" -v y="${sys_us:-0}" 'BEGIN{printf "%.1f", (u+y)/1000}')
  echo "$cpu_ms $nivcsw $gc ${ck:-NOCHECK}"
}
best() {
  local eng="$1" twin="$2" s="$3" bm="" bn=0 bg=0 ck="" r m n g c
  for _ in $(seq 1 "$REPS"); do
    r=$(run1 "$eng" "$twin" "$s"); m=$(awk '{print $1}' <<<"$r")
    [ "$m" = "-" ] && { c=$(cut -d' ' -f4- <<<"$r"); echo "- - - $c"; return; }
    n=$(awk '{print $2}' <<<"$r"); g=$(awk '{print $3}' <<<"$r"); c=$(cut -d' ' -f4- <<<"$r")
    ck="$c"
    if [ -z "$bm" ] || [ "$(awk -v a="$m" -v b="$bm" 'BEGIN{print (a<b)?1:0}')" = 1 ]; then bm="$m"; bn=$n; bg=$g; fi
  done
  echo "$bm $bn $bg $ck"
}
echo "FIXED-ITERATION SNOBOL4 BENCHMARKS -- N fixed per kernel (SCALE.tsv), elapsed cpu time measured"
echo "engines: $ENGINES   reps: $REPS   corpus: $B   scale: $SCALETSV"
echo "measurement condition: SCRIP_NOHUGE=$NOHUGE  SCRIP_HEAP_MB=$HEAP (sbl unaffected -- separate binary)"
echo
printf "%-20s %10s %12s %12s %12s   %8s %8s %5s %7s  %s\n" BENCHMARK "N" "sbl/s" "m3/s" "m4/s" "m3:sbl" "m4:m3" "gc" "nivcsw" "check"
printf "%-20s %10s %12s %12s %12s   %8s %8s %5s %7s  %s\n" "--------------------" "----------" "------------" "------------" "------------" "--------" "--------" "-----" "-------" "-----"
tot_ok=0; tot_bad=0; tot_gc=0; tot_load=0; UNGRADED=""; UNSCALED=""
for sno in "$B"/*.sno; do
  [ -e "$sno" ] || continue
  s=$(basename "${sno%.sno}"); ref="${sno%.sno}.ref"
  N=$(scale_n "$s")
  if [ -z "$N" ]; then UNSCALED="$UNSCALED $s"; continue; fi
  if ! TWIN=$(bash "$HERE/bench_wrap.sh" "$sno" -o "$W/$s.bench.sno" --mode=iter --n="$N" 2>&1); then
    UNGRADED="$UNGRADED $s"; echo "  ⛔ $s: $TWIN" >&2; continue; fi
  declare -A R=(); declare -A C=(); declare -A G=(); declare -A NV=()
  for eng in $ENGINES; do
    res=$(best "$eng" "$TWIN" "$s")
    m=$(awk '{print $1}' <<<"$res")
    NV[$eng]=$(awk '{print $2}' <<<"$res"); G[$eng]=$(awk '{print $3}' <<<"$res"); c=$(cut -d' ' -f4- <<<"$res")
    if [ "$m" = "-" ]; then R[$eng]="NA"; C[$eng]="$c"; else R[$eng]=$(rate "$N" "$m"); C[$eng]="$c"; fi
  done
  g3="${G[m3]:-0}"; g4="${G[m4]:-0}"
  case "$g3" in ''|*[!0-9]*) g3=0 ;; esac
  case "$g4" in ''|*[!0-9]*) g4=0 ;; esac
  gcn=$(( g3 + g4 )); [ "$gcn" -gt 0 ] && tot_gc=$((tot_gc+1))
  nmax=0
  for eng in $ENGINES; do
    nv="${NV[$eng]:-0}"; case "$nv" in ''|*[!0-9]*) nv=0 ;; esac
    [ "$nv" -gt "$nmax" ] && nmax=$nv
  done
  nivstr="$nmax"; if [ "$nmax" -gt "$NIVCSW_FLAG" ]; then nivstr="${nmax}!"; tot_load=$((tot_load+1)); fi
  ckstat="ok"; base=""
  for eng in $ENGINES; do
    [ "${C[$eng]}" = "CRASH" ] || [ "${C[$eng]}" = "BUILD-ERR" ] && { ckstat="${C[$eng]}"; break; }
    [ -z "$base" ] && base="${C[$eng]}"
    [ "${C[$eng]}" = "$base" ] || ckstat="DISAGREE"
  done
  case " $ENGINES " in *" sbl "*) : ;; *) [ "$ckstat" = ok ] && ckstat="ok(x-eng)" ;; esac
  [ -f "$ref" ] || ckstat="${ckstat}/NO-REF"
  [ "$ckstat" = ok ] && tot_ok=$((tot_ok+1)) || tot_bad=$((tot_bad+1))
  sp3=$(awk -v a="${R[m3]:-NA}" -v b="${R[sbl]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  sp4=$(awk -v a="${R[m4]:-NA}" -v b="${R[m3]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  printf "%-20s %10s %12s %12s %12s   %8s %8s %5s %7s  %s\n" "$s" "$N" \
    "$(human "${R[sbl]:-NA}")" "$(human "${R[m3]:-NA}")" "$(human "${R[m4]:-NA}")" "$sp3" "$sp4" \
    "$([ "$gcn" -gt 0 ] && echo "GC$gcn" || echo 0)" "$nivstr" "$ckstat"
done
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad"
if [ "$tot_load" -gt 0 ]; then
  echo "⛔ $tot_load row(s) have nivcsw > $NIVCSW_FLAG -- winning rep still scheduler-contaminated; re-measure quieter."
fi
if [ -n "$UNSCALED" ]; then
  echo "⛔ UNSCALED (no row in $SCALETSV, not measured):$UNSCALED"
  echo "   Every benchmark needs a committed N in SCALE.tsv -- run scripts/calibrate_bench_scale.sh to add it."
fi
if [ -n "$UNGRADED" ]; then
  echo "⛔ UNGRADED (has a SCALE.tsv row but bench_wrap.sh refused it):$UNGRADED"
fi
[ -z "$UNSCALED" ] && [ -z "$UNGRADED" ] && echo "coverage: every SCALE.tsv-listed kernel measured."
if [ "$tot_gc" -gt 0 ]; then
  echo "⛔ $tot_gc row(s) COLLECTED inside the measurement window -- raise HEAP (currently ${HEAP}MB)."
else
  echo "gc: 0 rows collected inside the window at HEAP=${HEAP}MB -- every rate above is stall-free."
fi
[ "$tot_bad" -eq 0 ] && [ -z "$UNGRADED" ]
