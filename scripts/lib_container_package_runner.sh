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
# util_score_row.py: THE AND PER PROGRAM over the programs the oracle grades, with a program a ruled class excludes named in EXCLUDED.tsv
# by a ruled class and shown as Excl, so shipped = denominator + Excl; every other ungraded program stays in as debt. rc: the harness's, 2 refused.
container_package_run() {
  local lang="$1" ext="$2" key="$3" suite="$4" gate="$5"
  local here sd board rc out field_ scored shipped excl denom bothp m3p m4p inv cc
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
  # ⭐ THE POPULATION IS SHIPPED TOO (CEO-749/1286, test_gate_score_row_denominator_includes_xfails.sh): every shipped program the container does
  # not grade gets one UNGRADED progress row per mode, its class from UNGRADABLE.tsv or UNGRADED.tsv, before the row is written
  . "$here/lib_progress.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_progress.sh unloadable"; return 2; }
  local graded_list ung_rows n cls
  graded_list="$(cd "$sd" && S4E_PROGRESS_OFF=1 python3 scripts/corpus_suite_harness.py list "$suite/ALL.$ext" "$suite/ALL.ref" --lang "$lang" 2>/dev/null)"
  ( ung_rows="$(mktemp)"; trap 'rm -f "$ung_rows"' EXIT   # a subshell trap: this library is sourced, so an EXIT trap here must not replace the caller's
  while IFS= read -r f; do
    n="$(basename "$f" ".$ext")"; grep -qxF "$n" <<<"$graded_list" && continue
    cls="$(awk -F'\t' -v k="$n.$ext" '$1==k {print $2; exit}' "$suite/UNGRADABLE.tsv" "$suite/UNGRADED.tsv" 2>/dev/null)"
    printf 'package\t%s\t%s\t%s\tm3\tUNGRADED\t0\t%s\npackage\t%s\t%s\t%s\tm4\tUNGRADED\t0\t%s\n' "$key" "$lang" "$n" "${cls:-unclassed}" "$key" "$lang" "$n" "${cls:-unclassed}" >> "$ung_rows"
  done < <(find "$suite" -maxdepth 1 -name "*.$ext" ! -name "ALL.$ext" | sort)
  [ -s "$ung_rows" ] && { progress_append_rows_tsv "$ung_rows" || echo "⚠ the UNGRADED progress rows were not appended (reason above) -- the row write's cross-check reads the graded population only" >&2; } )
  # the denominator is SHIPPED (CEO-1286): a program the container does not grade is named in UNGRADABLE.tsv or UNGRADED.tsv and stays
  # in as debt; only a program a ruled EXCLUDED.tsv class names leaves it, shown as Excl
  excl=0; [ -f "$suite/EXCLUDED.tsv" ] && excl=$(grep -vc -E '^#|^\s*$' "$suite/EXCLUDED.tsv")
  denom=$((shipped - excl))
  echo "$(printf '%s' "$key" | tr 'a-z-' 'A-Z_')_BOARD shipped=$shipped graded=$scored excluded=$excl denominator=$denom both_pass=$bothp m3_pass=$m3p m4_pass=$m4p"
  INV_PACKAGE="$key"; INV_DIR="$suite"; INV_EXT=".$ext"
  inv="$(inventory_line "$scored" 0)"
  if [ -n "$inv" ]; then echo "$inv"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi
  cc="${S4E_CRITERION_CHANGED:-}"
  python3 "$here/util_score_row.py" write --lang "$lang" --column vendor --suite "$key" --suite-key "$key" --modes m3,m4 \
      ${cc:+--criterion-changed "$cc"} --suite-pass "$bothp" --suite-total "$denom" --excluded "$excl" --measurer "${S4E_SEAT:-}" \
      --text "$key both-modes $bothp/$denom · m3 $m3p/$denom · m4 $m4p/$denom ($scored graded of $shipped shipped, the rest named in UNGRADABLE.tsv/UNGRADED.tsv as debt, EXCLUDED=$excl${inv:+ · $inv} (\`$gate.sh\`))" \
      || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
  return "$rc"
}
