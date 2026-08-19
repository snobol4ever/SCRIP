#!/usr/bin/env bash
# test_gate_s130_behav.sh -- s130 BEHAVIOURAL A/B in mode 3 against the checked-in .ref oracles.
# Records PASS/FAIL per program for the current arm; compare two tags to expose row-level movers.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
ROOT=${ROOT:-$S4E/corpus}
SETS=${SETS:-"crosscheck"}
case "$1" in
record)
  tag=$2; out=/tmp/s130b_$tag.txt; : > "$out"
  for s in $SETS; do
    for f in $(find "$ROOT/$s" -name '*.sno' | sort); do
      r="${f%.sno}.ref"; [ -f "$r" ] || continue
      got=$(timeout 15 $S4E/SCRIP/scrip --run "$f" < /dev/null 2>/dev/null)
      if [ "$got" = "$(cat "$r")" ]; then echo "PASS $f" >> "$out"; else echo "FAIL $f" >> "$out"; fi
    done
  done
  p=$(grep -c '^PASS' "$out"); n=$(grep -c '^FAIL' "$out")
  echo "$tag: PASS=$p FAIL=$n TOTAL=$((p+n))" ;;
compare)
  a=/tmp/s130b_$2.txt; b=/tmp/s130b_$3.txt
  echo "== FIXED by $3 (FAIL->PASS) =="; join -j2 <(sort -k2 "$a") <(sort -k2 "$b") -o 1.1,2.1,0 | awk '$1=="FAIL"&&$2=="PASS"{print "  +",$3}'
  echo "== BROKEN by $3 (PASS->FAIL) =="; join -j2 <(sort -k2 "$a") <(sort -k2 "$b") -o 1.1,2.1,0 | awk '$1=="PASS"&&$2=="FAIL"{print "  -",$3}'
  echo "== tallies =="; grep -c '^PASS' "$a" | sed "s/^/  $2 PASS=/"; grep -c '^PASS' "$b" | sed "s/^/  $3 PASS=/" ;;
esac
