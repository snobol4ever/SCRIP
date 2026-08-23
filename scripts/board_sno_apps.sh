#!/usr/bin/env bash
# board_sno_apps.sh — SPEED board for the FIVE WHOLE-APPLICATION demos in the s158 working set.
# The rail (bench_sno_rail.sh) cannot measure these: it works by injecting a repeat loop around a
# single `src PATTERN :F(label)` statement, and these five are whole applications, not injectable
# single-pattern loops. This closes that half of the s158 gap: "10 of 15 have NO measurement path".
# METHOD: amplify the INPUT (concatenate the corpus to itself XN times) so the window clears ~1s and
# process startup stops dominating. The program is NEVER modified. Fixed XN per program (auto-ranging
# by doubling re-ran sbl too many times to be practical). Identity is re-checked AT THE AMPLIFIED
# INPUT, so a program that misbehaves on the larger corpus cannot silently contribute a ratio.
# A diverging mode gets NO RATIO (X-DIV) — s158's treebank-array ~8x was measured in its broken mode.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
SC=${SC:-$S4E/SCRIP}; D=${D:-$S4E/corpus/programs/snobol4/demo}
. "$(dirname "${BASH_SOURCE[0]}")/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL=${SBL:-$(sbl_clean_bin)}   # BENCHMARK oracle (s255) -- x64/bin/sbl is instrumented, ~2.2-3.5x slower
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
ulimit -s unlimited
R=${R:-3}; TMO=${TMO:-600}; ONLY=${ONLY:-}
med() { python3 -c "import sys,statistics;v=[int(x) for x in sys.argv[1:] if x];print(int(statistics.median(v)) if v else 0)" "$@"; }
ms() { local t0 t1; t0=$(date +%s%N); "$@" >/dev/null 2>&1; t1=$(date +%s%N); echo $(( (t1-t0)/1000000 )); }
rat() { python3 -c "import sys;a=float(sys.argv[1]);b=float(sys.argv[2]);print('%.2f'%(a/b) if b>0 else 'n/a')" "$1" "$2"; }
printf '%-18s %4s %8s %8s %8s %8s %8s\n' PROGRAM XN SBLms M3ms M4ms M3/SBL M4/SBL
printf '%.0s-' {1..66}; echo
run() {
  local nm=$1 xn=$2 base=$3 xf=$4
  [ -n "$ONLY" ] && [[ " $ONLY " != *" $nm "* ]] && return
  local src="$D/$nm.sno"
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/o.sno"; cat "$src" >> "$W/o.sno"
  : > "$W/in"; for _ in $(seq 1 "$xn"); do cat "$base" >> "$W/in"; done
  timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$W/in" > "$W/ref" 2>/dev/null || { printf '%-18s %4s SBL-FAIL\n' "$nm" "$xn"; return; }
  local ok3=0 ok4=0
  timeout $TMO "$SC/scrip" --run "$src" < "$W/in" > "$W/a3" 2>/dev/null && cmp -s "$W/ref" "$W/a3" && ok3=1
  if timeout $TMO "$SC/scrip" --compile "$src" > "$W/a.s" 2>/dev/null \
     && gcc -no-pie "$W/a.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/a.prog" 2>/dev/null; then
    timeout $TMO "$W/a.prog" < "$W/in" > "$W/a4" 2>/dev/null && cmp -s "$W/ref" "$W/a4" && ok4=1; fi
  local sb=() t3=() t4=()
  for i in $(seq 1 $R); do
    sb+=( "$(ms timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/o.sno" < "$W/in")" )
    [ $ok3 = 1 ] && t3+=( "$(ms timeout $TMO "$SC/scrip" --run "$src" < "$W/in")" )
    [ $ok4 = 1 ] && t4+=( "$(ms timeout $TMO "$W/a.prog" < "$W/in")" )
  done
  local msb m3=0 m4=0 r3=X-DIV r4=X-DIV
  msb=$(med "${sb[@]}")
  [ $ok3 = 1 ] && { m3=$(med "${t3[@]}"); r3=$(rat "$m3" "$msb"); }
  [ $ok4 = 1 ] && { m4=$(med "${t4[@]}"); r4=$(rat "$m4" "$msb"); }
  printf '%-18s %4s %8s %8s %8s %8s %8s\n' "$nm" "$xn" "$msb" "$m3" "$m4" "$r3" "$r4"
}
run claws5         "${XN_CLAWS5:-20}"   "$D/CLAWS5inTASA.dat" ""
run treebank       "${XN_TBL:-6}"       "$D/VBGinTASA.dat"    "-s256m"
run calculator-1   "${XN_C1:-10}"       "$D/calculator.input" ""
run calculator-2   "${XN_C2:-48}"       "$D/calculator.input" ""
printf '%.0s-' {1..66}; echo
echo "RATIO = SCRIP/SPITBOL, lower better. 0.50 = 2x, 0.33 = 3x. X-DIV = mode diverges. R=$R. RT_OPT=-O0."
