#!/usr/bin/env bash
# bench_snobol4_timed_direct.sh -- INTERIM direct re-baseline of the 15 canonical
# corpus/benchmarks/snobol4 kernels against the CLEAN oracle, bypassing
# bench_wrap.sh/test_bench_snobol4_timed.sh's '*BENCH kernel=...' marker requirement.
#
# ⛔ WHY THIS EXISTS (task bench-rebaseline-15-kernels-clean-oracle, 2026-08-23): the s265
# STANDALONE REVAMP (Lon in-chat, see bench_wrap.sh header) updated test_bench_snobol4_timed.sh
# to require every corpus/benchmarks/snobol4/*.sno to carry a '*BENCH kernel=... check=...'
# marker before it will be measured -- but NONE of the 15 shipped kernels have been migrated to
# that shape yet (verified: `bash scripts/test_bench_snobol4_timed.sh` reports CHECK RESULT:
# ok=0 bad=0, all 15 UNGRADED, each with "carries no '*BENCH kernel=...' marker"). Migrating 15
# kernels to the new standalone shape is a real per-kernel rewrite (new .ref values reflecting
# real program output, not a harness check artifact) -- out of scope for a measure-only sweep.
# The kernels are STILL fully functional, unmodified harness.inc TIME-mode programs (this file
# does not touch corpus/ or harness.inc), so this script measures them directly instead.
#
# DELETE THIS SCRIPT once the 15 kernels carry '*BENCH' markers and
# test_bench_snobol4_timed.sh reports CHECK RESULT: ok=15 again -- it is a bridge, not a
# permanent second harness.
#
# Methodology mirrors test_bench_snobol4_timed.sh's run1()/best() (external cpu via
# tools/bench_rusage, best-of-REPS by external cpu ms, gc/nivcsw counted, sbl via
# lib_oracle_flags.sh's sbl_clean_bin/sbl_lang_flags) so numbers stay apples-to-apples with
# established practice -- the only difference is the kernel .sno is run directly instead of
# through a bench_wrap.sh-built twin.
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"
T="${TIMEOUT:-60}"; REPS="${REPS:-3}"; NOHUGE="${NOHUGE:-1}"; HEAP="${HEAP:-1024}"
. "$HERE/lib_oracle_flags.sh" || { echo "REFUSING: cannot load lib_oracle_flags.sh" >&2; exit 3; }
SBL="${SBL:-$(sbl_clean_bin)}" || { echo "REFUSING: clean oracle not sbl_bf_capable" >&2; exit 3; }
SBLFLAGS="${SBLFLAGS:--s16m}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$B" ]     || { echo "SKIP timed bench corpus missing ($B)"; exit 0; }
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" || { echo "FAIL build $WRAP" >&2; exit 1; }
NIVCSW_FLAG="${BENCH_NIVCSW_THRESHOLD:-20}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
rate() { awk -v i="$1" -v m="$2" 'BEGIN{ if(m+0>0) printf "%.0f", i/m*1000; else printf "NA" }'; }
human() { awk -v v="$1" 'BEGIN{ if(v=="NA"){print "NA"; exit}
  if(v>=1e9) printf "%.2fG", v/1e9; else if(v>=1e6) printf "%.1fM", v/1e6;
  else if(v>=1e3) printf "%.1fK", v/1e3; else printf "%d", v }'; }
# ⛔ RULES.md FACT RULE s266: multiple = ours_rate/reference_rate, NEVER inverted; >=1.0 FASTER, <1.0 SLOWER.
ratiofmt() { awk -v a="$1" -v b="$2" 'BEGIN{
  if(a=="NA"||b=="NA"||b+0==0){print "-"; exit}
  m=a/b; dir=(m>=1.0)?"FASTER":"SLOWER"; printf "%.3f %s", m, dir }'; }
run1() {
  local eng="$1" run="$2"
  case "$eng" in
    sbl) out=$("$WRAP" timeout "$T" "$SBL" $(sbl_lang_flags) $SBLFLAGS "$run" 2>"$W/gc.err" </dev/null) ;;
    m3)  out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 "$WRAP" timeout "$T" "$SCRIP" --run "$run" 2>"$W/gc.err" </dev/null) ;;
    m4)  local s; s=$(basename "${run%.sno}")
         "$SCRIP" --compile "$run" > "$W/$s.s" 2>"$W/compile.err"
         if [ ! -s "$W/$s.s" ] || ! gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm \
              -Wl,-rpath,"$RT" -o "$W/$s.prog" 2>"$W/link.err"; then echo "- - - - BUILD-ERR"; return; fi
         out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 "$WRAP" timeout "$T" "$W/$s.prog" 2>"$W/gc.err" </dev/null) ;;
  esac
  local it ck gc rusage_line user_us sys_us nivcsw cpu_ms
  it=$(sed -n 's/^iters: //p' <<<"$out")
  ck=$(sed -n 's/^check: //p' <<<"$out")
  gc=$(grep -c 'regeneration #' "$W/gc.err" 2>/dev/null); gc="${gc:-0}"
  [ -n "$it" ] || { echo "- - - - CRASH"; return; }
  rusage_line=$(grep '^BENCH_RUSAGE:' "$W/gc.err" | tail -1)
  user_us=$(echo "$rusage_line" | grep -oE 'user_us=[0-9]+' | cut -d= -f2)
  sys_us=$(echo "$rusage_line" | grep -oE 'sys_us=[0-9]+' | cut -d= -f2)
  nivcsw=$(echo "$rusage_line" | grep -oE 'nivcsw=[0-9]+' | cut -d= -f2); nivcsw="${nivcsw:-0}"
  cpu_ms=$(awk -v u="${user_us:-0}" -v y="${sys_us:-0}" 'BEGIN{printf "%.1f", (u+y)/1000}')
  echo "$it $cpu_ms $nivcsw $gc ${ck:-NOCHECK}"
}
best() {
  local eng="$1" run="$2" bi=0 bm=0 bn=0 bg=0 ck="" r i m n g c
  for _ in $(seq 1 "$REPS"); do
    r=$(run1 "$eng" "$run"); i=$(awk '{print $1}' <<<"$r"); m=$(awk '{print $2}' <<<"$r")
    n=$(awk '{print $3}' <<<"$r"); g=$(awk '{print $4}' <<<"$r"); c=$(cut -d' ' -f5- <<<"$r")
    [ "$i" = "-" ] && { echo "- - - - $c"; return; }
    ck="$c"
    if [ "$(awk -v a="$i" -v b="$m" -v x="$bi" -v y="$bm" 'BEGIN{print (b>0 && (y<=0 || a/b > x/y))?1:0}')" = 1 ]; then bi=$i; bm=$m; bn=$n; bg=$g; fi
  done
  echo "$bi $bm $bn $bg $ck"
}
KERNELS="arith_loop array_sum eval_fixed fibonacci func_call indirect_dispatch mixed_workload op_dispatch pattern_bt roman string_concat string_manip string_pattern table_access var_access"
echo "DIRECT RE-BASELINE (interim, s265-marker-migration bridge) -- 15 canonical corpus/benchmarks/snobol4 kernels vs CLEAN oracle"
echo "oracle: $SBL   RT_OPT(runtime .so): -O0 (Makefile default; NO -O2 per RULES.md s262)   reps: $REPS"
echo "measurement condition: SCRIP_NOHUGE=$NOHUGE SCRIP_HEAP_MB=$HEAP  instrument: tools/bench_rusage external cpu(user+sys)"
echo "load at start: $(uptime | sed 's/.*load average: //')"
echo
printf "%-18s %11s %11s %11s   %16s %16s %5s %7s  %s\n" BENCHMARK "sbl/s" "m3/s" "m4/s" "m3 vs sbl" "m4 vs sbl" "gc" "nivcsw" "check(sbl/m3/m4)"
printf "%-18s %11s %11s %11s   %16s %16s %5s %7s  %s\n" "------------------" "-----------" "-----------" "-----------" "----------------" "----------------" "-----" "-------" "----------------"
tot_ok=0; tot_bad=0
for s in $KERNELS; do
  sno="$B/$s.sno"
  [ -f "$sno" ] || { echo "  MISSING: $sno"; continue; }
  declare -A R=() ; declare -A C=(); declare -A G=(); declare -A N=()
  for eng in sbl m3 m4; do
    res=$(best "$eng" "$sno")
    i=$(awk '{print $1}' <<<"$res"); m=$(awk '{print $2}' <<<"$res")
    N[$eng]=$(awk '{print $3}' <<<"$res"); G[$eng]=$(awk '{print $4}' <<<"$res"); c=$(cut -d' ' -f5- <<<"$res")
    if [ "$i" = "-" ]; then R[$eng]="NA"; C[$eng]="$c"; else R[$eng]=$(rate "$i" "$m"); C[$eng]="$c"; fi
  done
  g3="${G[m3]:-0}"; g4="${G[m4]:-0}"
  case "$g3" in ''|*[!0-9]*) g3=0 ;; esac; case "$g4" in ''|*[!0-9]*) g4=0 ;; esac
  gcn=$((g3+g4))
  nmax=0; for eng in sbl m3 m4; do nv="${N[$eng]:-0}"; case "$nv" in ''|*[!0-9]*) nv=0 ;; esac; [ "$nv" -gt "$nmax" ] && nmax=$nv; done
  nivstr="$nmax"; [ "$nmax" -gt "$NIVCSW_FLAG" ] && nivstr="${nmax}!"
  ckstat=ok
  if [ "${C[sbl]}" = "${C[m3]}" ] && [ "${C[m3]}" = "${C[m4]}" ]; then tot_ok=$((tot_ok+1)); else ckstat=DISAGREE; tot_bad=$((tot_bad+1)); fi
  sp3=$(ratiofmt "${R[m3]:-NA}" "${R[sbl]:-NA}")
  sp4=$(ratiofmt "${R[m4]:-NA}" "${R[sbl]:-NA}")
  printf "%-18s %11s %11s %11s   %16s %16s %5s %7s  %s/%s/%s\n" "$s" \
    "$(human "${R[sbl]:-NA}")" "$(human "${R[m3]:-NA}")" "$(human "${R[m4]:-NA}")" "$sp3" "$sp4" \
    "$([ "$gcn" -gt 0 ] && echo "GC$gcn" || echo 0)" "$nivstr" "${C[sbl]:-?}" "${C[m3]:-?}" "${C[m4]:-?}"
done
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad (agreement across sbl/m3/m4 check lines)"
echo "⛔ rows marked '!' in nivcsw exceeded $NIVCSW_FLAG -- scheduler-contaminated even at best-of-$REPS; re-measure on a quieter box before quoting as a headline number."
echo "⛔ rows marked GC>0 collected inside the measurement window -- that rate is a stall figure, not throughput."
