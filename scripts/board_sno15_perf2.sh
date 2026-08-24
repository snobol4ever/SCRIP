#!/usr/bin/env bash
# board_sno15_perf2.sh — SPEED board, v2: WINDOW-ADEQUATE by input amplification.
# v1 (board_sno15_perf.sh) proved its own numbers inadmissible: 14/15 windows were <800ms, so the
# ratios were measuring PROCESS STARTUP, not compute (the exact s147 disease — sub-1s windows swung
# 1.4x-3.9x run to run). The rail's fix is loop-injection + in-program TIME(); that only works on the
# five injectable single-pattern *-match stems. This script fixes the window for ALL FIFTEEN WITHOUT
# TOUCHING ANY PROGRAM: it AUTO-RANGES THE INPUT (concatenating the corpus to itself) until sbl's
# window clears MIN_MS. Same program, same work per byte, just more bytes — a workload a user could
# actually have. Startup is then a small constant fraction of a large window instead of the whole thing.
# Honesty rules unchanged from v1: a diverging mode gets NO RATIO (X-DIV); every window is printed;
# m4 compile/link happens before the clock. RATIO = SCRIP/SPITBOL, lower better; 0.50=2x, 0.33=3x.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
SC=${SC:-$S4E/SCRIP}; D=${D:-$S4E/corpus/snobol4/demo}
. "$(dirname "${BASH_SOURCE[0]}")/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL=${SBL:-$(sbl_clean_bin)}   # BENCHMARK oracle (s255) -- x64/bin/sbl is instrumented, ~2.2-3.5x slower
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
ulimit -s unlimited
R=${R:-3}; TMO=${TMO:-600}; MIN_MS=${MIN_MS:-800}; XMAX=${XMAX:-64}
ONLY=${ONLY:-}
inp_for() { case $1 in claws5*) echo "$D/CLAWS5inTASA.dat";; treebank*) echo "$D/VBGinTASA.dat";;
  json*) echo "$D/twitter.json";; calculator*) echo "$D/calculator.input";; *) echo "";; esac; }
xf_for()  { case $1 in treebank*) echo "-s256m";; *) echo "";; esac; }
med() { python3 -c "import sys,statistics;v=[int(x) for x in sys.argv[1:] if x];print(int(statistics.median(v)) if v else 0)" "$@"; }
ms() { local t0 t1; t0=$(date +%s%N); "$@" >/dev/null 2>&1; t1=$(date +%s%N); echo $(( (t1-t0)/1000000 )); }
rat() { python3 -c "import sys;a=float(sys.argv[1]);b=float(sys.argv[2]);print('%.2f'%(a/b) if b>0 else 'n/a')" "$1" "$2"; }
printf '%-26s %4s %8s %8s %8s %8s %8s  %s\n' PROGRAM XN SBLms M3ms M4ms M3/SBL M4/SBL FLAG
printf '%.0s-' {1..90}; echo
for nm in claws5 claws5-match claws5-match-fence \
          treebank treebank-match treebank-match-fence \
          json-match json-match-fence \
          calculator-1 calculator-1-match calculator-1-match-fence \
          calculator-2 calculator-2-match calculator-2-match-fence; do
  [ -n "$ONLY" ] && [[ " $ONLY " != *" $nm "* ]] && continue
  src="$D/$nm.sno"; base=$(inp_for "$nm"); xf=$(xf_for "$nm"); flag=""
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/o.sno"; cat "$src" >> "$W/o.sno"
  # ---- auto-range the INPUT until sbl's window clears MIN_MS ----
  xn=1
  while :; do
    : > "$W/in"; for _ in $(seq 1 $xn); do cat "$base" >> "$W/in"; done
    t=$(ms timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$W/in")
    [ "$t" -ge "$MIN_MS" ] && break
    [ "$xn" -ge "$XMAX" ] && { flag="WINDOW-CAPPED"; break; }
    xn=$((xn*2))
  done
  inp="$W/in"
  # ---- correctness at the amplified input; a ratio is issued ONLY for a mode proven identical ----
  timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$inp" > "$W/ref" 2>/dev/null || { printf '%-26s %4s %s\n' "$nm" "$xn" "SBL FAILED"; continue; }
  ok3=0; timeout $TMO "$SC/scrip" --run "$src" < "$inp" > "$W/a3" 2>/dev/null && cmp -s "$W/ref" "$W/a3" && ok3=1
  ok4=0
  if timeout $TMO "$SC/scrip" --compile "$src" > "$W/a.s" 2>/dev/null \
     && gcc -no-pie "$W/a.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/a.prog" 2>/dev/null; then
    timeout $TMO "$W/a.prog" < "$inp" > "$W/a4" 2>/dev/null && cmp -s "$W/ref" "$W/a4" && ok4=1
  fi
  sb=(); t3=(); t4=()
  for i in $(seq 1 $R); do
    sb+=( "$(ms timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$inp")" )
    [ $ok3 = 1 ] && t3+=( "$(ms timeout $TMO "$SC/scrip" --run "$src" < "$inp")" )
    [ $ok4 = 1 ] && t4+=( "$(ms timeout $TMO "$W/a.prog" < "$inp")" )
  done
  msb=$(med "${sb[@]}"); m3=0; m4=0
  [ $ok3 = 1 ] && m3=$(med "${t3[@]}"); [ $ok4 = 1 ] && m4=$(med "${t4[@]}")
  r3="X-DIV"; r4="X-DIV"
  [ $ok3 = 1 ] && r3=$(rat "$m3" "$msb"); [ $ok4 = 1 ] && r4=$(rat "$m4" "$msb")
  [ "$msb" -lt "$MIN_MS" ] && flag="$flag NOISY"
  printf '%-26s %4s %8s %8s %8s %8s %8s  %s\n' "$nm" "$xn" "$msb" "$m3" "$m4" "$r3" "$r4" "$flag"
done
printf '%.0s-' {1..90}; echo
echo "XN = input concatenated XN times to clear MIN_MS=${MIN_MS}ms. RATIO = SCRIP/SPITBOL, lower better."
echo "0.50 = 2x faster, 0.33 = 3x faster. X-DIV = mode diverges, no ratio issued. R=$R medians. RT_OPT=-O0."
