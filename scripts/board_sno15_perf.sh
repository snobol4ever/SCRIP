#!/usr/bin/env bash
# board_sno15_perf.sh — SPEED board for the FIFTEEN working-set demos, honest by construction.
# Closes the s158 gap: "the RAIL ONLY KNOWS THE FIVE *-match STEMS ... 10 of 15 have NO measurement
# path yet and no ratio for them is honest until it is built."
#
# THREE HONESTY RULES, enforced mechanically, not by discipline:
#  (1) A mode that does not reproduce sbl's output byte-for-byte gets NO RATIO — printed as "X-DIV".
#      (s158:'s ~8x was measured in its DIVERGING mode and meant nothing.)
#  (2) Every window is printed. Windows under MIN_MS (default 800) are flagged NOISY: s147 measured
#      1.4x-3.9x run-to-run swings below ~1s, so a ratio from a short window is not evidence.
#  (3) Timing excludes compile/link for m4 (binary built once, before the clock) and excludes nothing
#      else — sbl and m3 both pay their own startup, as a user would.
# RATIO = SCRIP / SPITBOL. Lower is better. 0.50 = 2x faster. 0.33 = 3x faster.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
SC=${SC:-$S4E/SCRIP}; D=${D:-$S4E/corpus/programs/snobol4/demo}
. "$(dirname "${BASH_SOURCE[0]}")/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL=${SBL:-$(sbl_clean_bin)}   # BENCHMARK oracle (s255) -- x64/bin/sbl is instrumented, ~2.2-3.5x slower
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
ulimit -s unlimited
R=${R:-3}; TMO=${TMO:-300}; MIN_MS=${MIN_MS:-800}
inp_for() { case $1 in claws5*) echo "$D/CLAWS5inTASA.dat";; treebank*) echo "$D/VBGinTASA.dat";;
  json*) echo "$D/twitter.json";; calculator*) echo "$D/calculator.input";; *) echo "";; esac; }
xf_for()  { case $1 in treebank*) echo "-s256m";; *) echo "";; esac; }
med() { python3 -c "import sys,statistics;v=[int(x) for x in sys.argv[1:] if x];print(int(statistics.median(v)) if v else 0)" "$@"; }
ms() { local t0 t1; t0=$(date +%s%N); "$@" >/dev/null 2>&1; t1=$(date +%s%N); echo $(( (t1-t0)/1000000 )); }
rat() { python3 -c "import sys;a=float(sys.argv[1]);b=float(sys.argv[2]);print('%.2f'%(a/b) if b>0 else 'n/a')" "$1" "$2"; }
printf '%-26s %8s %8s %8s %8s %8s  %s\n' PROGRAM SBLms M3ms M4ms M3/SBL M4/SBL FLAG
printf '%.0s-' {1..82}; echo
for nm in claws5 claws5-match claws5-match-fence \
          treebank treebank-match treebank-match-fence \
          json-match json-match-fence \
          calculator-1 calculator-1-match calculator-1-match-fence \
          calculator-2 calculator-2-match calculator-2-match-fence; do
  src="$D/$nm.sno"; inp=$(inp_for "$nm"); xf=$(xf_for "$nm"); flag=""
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/o.sno"; cat "$src" >> "$W/o.sno"
  # correctness first — a ratio is only issued for a mode proven identical
  timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$inp" > "$W/ref" 2>/dev/null || { printf '%-26s %s\n' "$nm" "SBL FAILED"; continue; }
  ok3=0; timeout $TMO "$SC/scrip" --run "$src" < "$inp" > "$W/a3" 2>/dev/null && cmp -s "$W/ref" "$W/a3" && ok3=1
  ok4=0
  if timeout $TMO "$SC/scrip" --compile "$src" > "$W/a.s" 2>/dev/null \
     && gcc -no-pie "$W/a.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/a.prog" 2>/dev/null; then
    timeout $TMO "$W/a.prog" < "$inp" > "$W/a4" 2>/dev/null && cmp -s "$W/ref" "$W/a4" && ok4=1
  fi
  # interleaved rounds — sbl/m3/m4 adjacent in time so drift hits all three equally
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
  [ "$msb" -lt "$MIN_MS" ] && flag="NOISY(<${MIN_MS}ms)"
  printf '%-26s %8s %8s %8s %8s %8s  %s\n' "$nm" "$msb" "${m3:-–}" "${m4:-–}" "$r3" "$r4" "$flag"
done
printf '%.0s-' {1..82}; echo
echo "RATIO = SCRIP/SPITBOL, lower better. 0.50 = 2x faster, 0.33 = 3x. X-DIV = mode diverges, no ratio issued."
echo "R=$R rounds, medians. RT_OPT=-O0."
