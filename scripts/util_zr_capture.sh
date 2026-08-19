#!/usr/bin/env bash
# util_zr_capture.sh <outdir> -- capture --compile .s for the ZR-RSPRBP byte-identity witness set.
# Witness set spans 3 languages x {pinned, unpinned} and includes the ARBNO programs that are zv()'s
# only consumers, so a zv() change that is NOT inert cannot hide from it.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
OUT="$1"; mkdir -p "$OUT"
S=$S4E/SCRIP; C=$S4E/corpus
PROGS="
$C/programs/snobol4/parser/pat_arbno.sno
$C/programs/snobol4/feat/f04_pattern_primitives.sno
$C/benchmarks/snobol4/pattern_bt.sno
$C/benchmarks/snobol4/string_pattern.sno
$C/benchmarks/snobol4/arith_loop.sno
$C/benchmarks/icon/concord.icn
$C/benchmarks/icon/deal.icn
$C/programs/prolog/rung11_findall_findall_basic.pl
"
n=0
for p in $PROGS; do
  [ -f "$p" ] || { echo "MISS  $p"; continue; }
  b=$(basename "$p"); b=${b%.*}
  timeout 90 "$S/scrip" --compile "$p" > "$OUT/$b.s" 2>"$OUT/$b.err"
  sz=$(wc -c < "$OUT/$b.s")
  [ "$sz" -gt 0 ] && { echo "OK    $b ($sz bytes)"; n=$((n+1)); } || echo "EMPTY $b"
done
echo "captured=$n"
