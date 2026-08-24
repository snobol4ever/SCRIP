#!/bin/bash
# test_demo_full_3way.sh — claws5 + treebank FULL-corpus tri-identity gate (sbl == m3 == m4).
# Encodes the s107 recipes: oracle temp-prepend (-CASE 0 control card + tab &TRIM = 0 — never patch corpus),
# sbl heap -d512m -i64m (this build REJECTS -P), treebank sbl stack -s256m (ERROR 246 at the whole-corpus
# match otherwise), ulimit -s unlimited for SCRIP both modes (raw SIGSEGV otherwise — see GOAL-SNOBOL4-BB s107).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
SC=${SC:-$S4E/SCRIP}; D=${D:-$S4E/corpus/demo}
. "$(dirname "${BASH_SOURCE[0]}")/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL=${SBL:-$S4A/x64/bin/sbl}   # CORRECTNESS oracle -- tri-identity gate, not timing
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
ulimit -s unlimited
fail=0
one() {
  local nm=$1 src=$2 inp=$3 xf=$4
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/$nm.sbl.sno"; cat "$src" >> "$W/$nm.sbl.sno"
  "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/$nm.sbl.sno" < "$inp" > "$W/$nm.sbl" 2>/dev/null || { echo "$nm: sbl FAILED"; fail=1; return; }
  "$SC/scrip" --run "$src" < "$inp" > "$W/$nm.m3" 2>/dev/null || { echo "$nm: m3 FAILED rc=$?"; fail=1; return; }
  "$SC/scrip" --compile "$src" > "$W/$nm.s" 2>/dev/null && gcc -no-pie "$W/$nm.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/$nm.prog" 2>/dev/null || { echo "$nm: m4 build FAILED"; fail=1; return; }
  "$W/$nm.prog" < "$inp" > "$W/$nm.m4" 2>/dev/null || { echo "$nm: m4 run FAILED rc=$?"; fail=1; return; }
  if cmp -s "$W/$nm.sbl" "$W/$nm.m3" && cmp -s "$W/$nm.sbl" "$W/$nm.m4"; then echo "$nm: TRI-IDENTICAL ($(wc -l < "$W/$nm.sbl") lines)"; else echo "$nm: DIVERGES"; fail=1; fi
}
one claws5   "$D/claws5.sno"        "$D/CLAWS5inTASA.dat" ""
one treebank "$D/treebank.sno" "$D/VBGinTASA.dat"    "-s256m"
[ "$fail" -eq 0 ] && echo "DEMO-FULL 3WAY: PASS" || echo "DEMO-FULL 3WAY: FAIL"
exit "$fail"
