#!/usr/bin/env bash
# lib_container_package_runner.sh -- THE BODY OF A PACKAGE RUNNER WHOSE PACKAGE IS ONE CONTAINER (ALL.<ext> / ALL.ref / ALL.csv, built by
# util_build_package_suite.py with every ref cut from the oracle). Sourced by the Rosetta runners (Lon 2026-10-09, in-chat to the coo,
# verbatim: "Let's get rosseta for both Pascal and Prolog on the official test suite banner. Graded against the oracle."), each of which
# keeps its own one-runner guard on line 2 and calls:
#
#   container_package_run <lang> <ext> <suite-key> <suite-dir> <gate-name>
#
# It grades the container in both modes through corpus_suite_harness.py run (the one grader: declared heap and stack from ALL.csv, the
# timeout retry, the per-program progress rows), prints the board line and the inventory, and writes the suite row through
# util_score_row.py: THE AND PER PROGRAM over the programs the oracle grades, with the programs it cannot grade named in EXCLUDED.tsv
# (each also in UNGRADABLE.tsv, the oracle's reason) and shown as Excl, so shipped = denominator + Excl. rc: the harness's, 2 refused.
container_package_run() {
  local lang="$1" ext="$2" key="$3" suite="$4" gate="$5"
  local here sd board rc out field_ scored shipped excl bothp m3p m4p inv cc
  here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; sd="$here/.."
  export TIMEOUT_RETRY="${TIMEOUT_RETRY:-$here/util_timeout_retry.sh}"
  . "$here/lib_inventory.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_inventory.sh unloadable"; return 2; }
  [ -x "$sd/scrip" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $sd/scrip -- build first (make)"; return 2; }
  "$here/util_require_fresh.sh" --gate "$gate" "$sd/scrip" "${RT_DIR:-$sd/out}/libscrip_rt.so" || return 2
  [ -f "$suite/ALL.$ext" ] && [ -f "$suite/ALL.ref" ] || { echo "⛔ REFUSE(rc=2): no container at $suite/ALL.{$ext,ref} -- run: python3 scripts/util_build_package_suite.py $suite --lang $lang --twice"; return 2; }
  shipped=$(find "$suite" -maxdepth 1 -name "*.$ext" ! -name "ALL.$ext" | wc -l)
  out="$(cd "$sd" && python3 scripts/corpus_suite_harness.py run "$suite/ALL.$ext" "$suite/ALL.ref" --lang "$lang" --modes m3,m4 2>&1)"; rc=$?
  printf '%s\n' "$out"
  board="$(printf '%s\n' "$out" | grep -m1 '^SUITE_BOARD ')"
  [ -n "$board" ] || { echo "⛔ REFUSE(rc=2): corpus_suite_harness.py printed no SUITE_BOARD line (above) -- nothing measured, no row written"; return 2; }
  field_() { printf '%s\n' "$board" | grep -oE " $1=[0-9]+" | head -1 | cut -d= -f2; }
  scored="$(field_ total)"; bothp="$(field_ all_pass)"; m3p="$(field_ m3_pass)"; m4p="$(field_ m4_pass)"
  excl=$((shipped - scored))
  [ "$excl" -ge 0 ] || { echo "⛔ REFUSE(rc=2): the container grades $scored programs but the package ships $shipped -- rebuild the container"; return 2; }
  if [ -f "$suite/EXCLUDED.tsv" ]; then
    local named; named=$(grep -vc -E '^#|^\s*$' "$suite/EXCLUDED.tsv")
    [ "$named" = "$excl" ] || { echo "⛔ REFUSE(rc=2): EXCLUDED.tsv names $named programs but shipped $shipped - graded $scored = $excl -- the container and the record drifted; rebuild both"; return 2; }
  elif [ "$excl" -gt 0 ]; then echo "⛔ REFUSE(rc=2): $excl shipped programs are not in the container and no EXCLUDED.tsv names them"; return 2; fi
  echo "$(printf '%s' "$key" | tr 'a-z-' 'A-Z_')_BOARD shipped=$shipped graded=$scored excluded=$excl both_pass=$bothp m3_pass=$m3p m4_pass=$m4p"
  INV_PACKAGE="$key"; INV_DIR="$suite"; INV_EXT=".$ext"
  inv="$(inventory_line "$scored" 0)"
  if [ -n "$inv" ]; then echo "$inv"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi
  cc="${S4E_CRITERION_CHANGED:-}"
  python3 "$here/util_score_row.py" write --lang "$lang" --column vendor --suite "$key" --suite-key "$key" --modes m3,m4 \
      ${cc:+--criterion-changed "$cc"} --suite-pass "$bothp" --suite-total "$scored" --excluded "$excl" --measurer "${S4E_SEAT:-}" \
      --text "$key both-modes $bothp/$scored · m3 $m3p/$scored · m4 $m4p/$scored ($shipped shipped, EXCLUDED=$excl the oracle cannot grade, named in EXCLUDED.tsv${inv:+ · $inv} (\`$gate.sh\`))" \
      || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
  return "$rc"
}
