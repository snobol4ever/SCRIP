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
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
SBL="${SBL:-$S4A/x64/bin/sbl}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/snobol4}"  # BM-ONE (s153): timed family PROMOTED -- one copy, legacy retired, harness.inc is the driver
T="${TIMEOUT:-60}"; REPS="${REPS:-1}"; NOHUGE="${NOHUGE:-1}"
# ⛔ GC-FREE WINDOW IS A MEASUREMENT PRECONDITION, NOT A TUNING KNOB (BM-3).  SCRIP's collector is
# EXHAUSTION-TRIGGERED (gc_heap.c: rt_gcheap_alloc collects only when the arena is full) and its cost
# scales with EVERY BLOCK EVER ALLOCATED, not with the live set: one measured regeneration walked
# 7,352,520 blocks to retain 1,548 and cost ~835 ms.  At the 512 MB default arena that single pause
# lands inside a 500 ms budget on the allocating rows and IS the reported number -- table_access read
# 0.23x of the oracle with the stall and 0.46x without, array_sum 0.34x -> 0.97x.  A benchmark window
# containing a collection is not a throughput reading, so the arena is sized past the window's total
# allocation and the regeneration count is COUNTED AND PRINTED per row: gc>0 marks the row untrusted
# rather than silently averaging a stall into it.  The oracle needs no equivalent (measured: sbl is
# insensitive to -d1024m on both allocating rows), so this equalises the condition, it does not favour.
HEAP="${HEAP:-1024}"
# -s16m: the json deserializer's recursive descent overflows the oracle's DEFAULT stack (ERROR 246)
# once its match runs inside ZBODY's frame.  Harmless to every other row; sizing a stack is not a
# throughput knob.  Larger values are refused by this container ("Stack memory unavailable").
SBLFLAGS="${SBLFLAGS:--s16m}"
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
  local eng="$1" sno="$2" s in; s=$(basename "${sno%.sno}")
  # DATA-DRIVEN ROWS (BM-4): a workload benchmark is fed its corpus on stdin from a sibling
  # <family>.dat, family = the program name minus any -match/-match-fence suffix, so the three
  # variants of one grammar provably read the SAME bytes.  Microbenchmarks have no .dat and keep
  # /dev/null -- a program that expects input and finds none must not silently "pass" on empty.
  in="$(dirname "$sno")/$(sed 's/-match\(-fence\)\?$//' <<<"$s").dat"; [ -f "$in" ] || in=/dev/null
  case "$eng" in
    sbl) [ -x "$SBL" ] || { echo "- - - ORACLE-MISSING"; return; }
         out=$(timeout "$T" "$SBL" -b $SBLFLAGS "$sno" 2>/dev/null <"$in"); : > "$W/gc.err" ;;
    m3)  out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 timeout "$T" "$SCRIP" --run "$sno" 2>"$W/gc.err" <"$in") ;;
    m4)  "$SCRIP" --compile "$sno" > "$W/$s.s" 2>/dev/null
         if [ ! -s "$W/$s.s" ] || ! gcc -no-pie "$W/$s.s" -L"$RT" -lscrip_rt -lm \
              -Wl,-rpath,"$RT" -o "$W/$s.prog" 2>/dev/null; then echo "- - - BUILD-ERR"; return; fi
         out=$(cd "$W" && SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" SCRIP_ZETA_TELEM=1 timeout "$T" "./$s.prog" 2>"$W/gc.err" <"$in") ;;
  esac
  local it ms ck gc
  it=$(sed -n 's/^iters: //p' <<<"$out"); ms=$(sed -n 's/^ms: //p'   <<<"$out")
  ck=$(sed -n 's/^check: //p' <<<"$out")
  gc=$(grep -c 'regeneration #' "$W/gc.err" 2>/dev/null); gc="${gc:-0}"
  [ -n "$it" ] || { echo "- - - CRASH"; return; }
  echo "$it $ms $gc ${ck:-NOCHECK}"
}
# ---- best of REPS (max throughput = least interference) --------------------
best() {
  local eng="$1" sno="$2" bi=0 bm=0 bg=0 ck="" r i m g c
  for _ in $(seq 1 "$REPS"); do
    r=$(run1 "$eng" "$sno"); i=$(awk '{print $1}' <<<"$r"); m=$(awk '{print $2}' <<<"$r")
    g=$(awk '{print $3}' <<<"$r"); c=$(cut -d' ' -f4- <<<"$r")
    [ "$i" = "-" ] && { echo "- - - $c"; return; }
    ck="$c"
    # the reported rep is the fastest one; its OWN gc count travels with it, so a printed rate and its
    # gc flag always describe the same run (a suite-wide max would mark clean rows dirty and vice versa)
    if [ "$(awk -v a="$i" -v b="$m" -v x="$bi" -v y="$bm" 'BEGIN{print (b>0 && (y<=0 || a/b > x/y))?1:0}')" = 1 ]; then bi=$i; bm=$m; bg=$g; fi
  done
  echo "$bi $bm $bg $ck"
}
echo "TIME-BASED SNOBOL4 BENCHMARKS -- fixed time budget, iterations counted"
echo "engines: $ENGINES   reps: $REPS   corpus: $B"
echo "measurement condition: SCRIP_NOHUGE=$NOHUGE  SCRIP_HEAP_MB=$HEAP (sbl unaffected -- separate binary)"
if [ -f "$FLOORTSV" ]; then echo "noise floor: $FLOORTSV (per-row; min-det = 3*cv)"
else echo "noise floor: NOT BAKED -- run scripts/bake_noise_floor_snobol4_timed.sh"; fi
echo
printf "%-20s %12s %12s %12s   %8s %8s %5s %9s  %s\n" BENCHMARK "sbl/s" "m3/s" "m4/s" "m3:sbl" "m4:m3" "gc" "min-det" "check"
printf "%-20s %12s %12s %12s   %8s %8s %5s %9s  %s\n" "--------------------" "------------" "------------" "------------" "--------" "--------" "-----" "---------" "-----"
tot_ok=0; tot_bad=0; tot_gc=0; tot_xf=0; tot_xp=0; UNGRADED=""; XPASSED=""
for sno in "$B"/*.sno; do
  [ -e "$sno" ] || continue
  # ⛔ UNGRADED IS PRINTED BY NAME, NEVER SILENTLY SKIPPED (owed since s158, landed s170).
  # The guard grades only harness-driven programs, so a skipped program is INVISIBLE in the table
  # above -- which is how 12 legacy-shaped programs sat in this directory reading as "not measured"
  # and as "nothing to see". Its NAME now goes to the census under the summary. BM-2 finished s170:
  # the list is EMPTY, the suite FAILS if it ever is not, and that is the end state made mechanical.
  if ! grep -q "INCLUDE '.*harness.inc'" "$sno"; then UNGRADED="$UNGRADED $(basename "${sno%.sno}")"; continue; fi
  s=$(basename "${sno%.sno}"); ref="${sno%.sno}.ref"
  declare -A R=(); declare -A C=(); declare -A G=()
  for eng in $ENGINES; do
    res=$(best "$eng" "$sno")
    i=$(awk '{print $1}' <<<"$res"); m=$(awk '{print $2}' <<<"$res")
    G[$eng]=$(awk '{print $3}' <<<"$res"); c=$(cut -d' ' -f4- <<<"$res")
    if [ "$i" = "-" ]; then R[$eng]="NA"; C[$eng]="$c"; else R[$eng]=$(rate "$i" "$m"); C[$eng]="$c"; fi
  done
  # a CRASHed/BUILD-ERR row reports "-" for its gc field; coerce to 0 so one dead row cannot abort the board
  g3="${G[m3]:-0}"; g4="${G[m4]:-0}"
  case "$g3" in ''|*[!0-9]*) g3=0 ;; esac
  case "$g4" in ''|*[!0-9]*) g4=0 ;; esac
  gcn=$(( g3 + g4 )); [ "$gcn" -gt 0 ] && tot_gc=$((tot_gc+1))
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
  # XFAIL LANE -- the corpus-wide convention (CORPUS-LOCATIONS.md): a sibling .xfail marks
  # known-unimplemented territory and is counted XFAIL, not FAIL. A row that PASSES with a marker
  # present is an XPASS: printed loudly below so a stale marker cannot mask a later regression,
  # but it never reddens the suite -- a defect getting fixed is not a suite failure.
  if [ -f "${sno%.sno}.xfail" ]; then
    if [ "$ckstat" = ok ]; then ckstat=XPASS; tot_xp=$((tot_xp+1)); XPASSED="$XPASSED $s"
    else ckstat="XFAIL:$ckstat"; tot_xf=$((tot_xf+1)); fi
  elif [ "$ckstat" = ok ]; then tot_ok=$((tot_ok+1))
  else tot_bad=$((tot_bad+1)); fi
  sp3=$(awk -v a="${R[m3]:-NA}" -v b="${R[sbl]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  sp4=$(awk -v a="${R[m4]:-NA}" -v b="${R[m3]:-NA}" 'BEGIN{ if(a=="NA"||b=="NA"||b+0==0){print "-"} else printf "%.2fx", a/b }')
  md="-"
  if [ -f "$FLOORTSV" ]; then
    md=$(awk -F'\t' -v b="$s" -v h="nohuge=$NOHUGE" '$1==b && $2=="m3" && $3==h {printf "%s%%", $8}' "$FLOORTSV")
    [ -n "$md" ] || md="-"
  fi
  printf "%-20s %12s %12s %12s   %8s %8s %5s %9s  %s\n" "$s" \
    "$(human "${R[sbl]:-NA}")" "$(human "${R[m3]:-NA}")" "$(human "${R[m4]:-NA}")" "$sp3" "$sp4" \
    "$([ "$gcn" -gt 0 ] && echo "GC$gcn" || echo 0)" "$md" "$ckstat"
done
echo
echo "CHECK RESULT: ok=$tot_ok bad=$tot_bad xfail=$tot_xf xpass=$tot_xp"
[ -n "$XPASSED" ] && echo "⛔ XPASS:$XPASSED -- passes WITH a sibling .xfail present. Re-measure, then RETIRE the marker."
if [ -n "$UNGRADED" ]; then
  echo "⛔ UNGRADED (in $B, not harness-driven, absent from the table above):$UNGRADED"
  echo "   Every benchmark here must include harness.inc -- convert it or delete it (BM-2). This is a FAILURE."
else
  echo "ungraded: none -- every .sno in $B is harness-driven and graded (BM-2 end state, s170)."
fi
if [ "$tot_gc" -gt 0 ]; then
  echo "⛔ $tot_gc row(s) COLLECTED inside the measurement window -- those rates are stall figures, not"
  echo "   throughput.  Raise HEAP (currently ${HEAP}MB) until the gc column reads 0 before quoting them."
else
  echo "gc: 0 rows collected inside the window at HEAP=${HEAP}MB -- every rate above is stall-free."
fi
[ "$tot_bad" -eq 0 ] && [ -z "$UNGRADED" ]
