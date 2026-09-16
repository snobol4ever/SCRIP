#!/usr/bin/env bash
# test_gate_suite_board_line_prints_shipped_with_outside_named.sh -- THE HARNESS BOARD LINE PRINTS PASS OVER THE SHIPPED
# POPULATION WITH OUTSIDE NAMED, AND THE OUTSIDE ENTRIES REACH THE PROGRESS DB (coo 2026-09-16; ceo CEO-772, CEO-749; Lon
# 2026-09-16 "Get those fixed" -- OUTSIDE is debt; row snobol4-master-runner-publishes-over-the-graded-population-1972-not-
# the-shipped-1980-with-outside-named).
#
# THE DEFECT: corpus_suite_harness.py dropped every --outside entry from its SUITE_BOARD total= (1972 for a 1980-entry
# master) and recorded nothing for them, so test_corpus_snobol4.sh published 1961/1972 and the SUITES.tsv row had to be
# hand-set to 1961/1980 OUTSIDE=8 (COO-82); on the plain path (no --by-modes-column) total= still carried the outside
# entries while the mode counts did not -- one word, two arithmetics. Every board write dropped OUTSIDE (hq_snobol4).
#
# THE CURE UNDER TEST: the SUITE_BOARD prints total= (graded) shipped= (graded + outside) outside= (the entries THIS run
# removed, so a sharded sum reads once), OUTSIDE_BASELINE lines name each with the oracle's class and reason, and each is
# appended to the progress DB as OUTSIDE in every requested mode; test_corpus_snobol4.sh quotes all_pass/shipped OUTSIDE=N
# and refuses when the fields are absent.
#
# ARMS (hermetic: a three-entry suite cut from the live master under mktemp in a tests/snobol4/ALL.sno layout, the progress
# DB redirected by S4E_PROGRESS_DB to a scratch table -- the live table is never touched; a fixture outside the corpus is not
# a board and the one-runner guard does not apply, CEO-547):
#   (a) with one entry declared outside: SUITE_BOARD total=2 shipped=3 outside=1, all_n=2, m3_n=2 m4_n=2
#   (b) the OUTSIDE_BASELINE line names the entry with its class and reason, and the count line says IN the shipped one
#   (c) the scratch DB carries one OUTSIDE row per requested mode for the entry, note = class: reason
#   (d) CONTROL: the same suite with no --outside prints total=3 shipped=3 outside=0
#   (e) STATIC: test_corpus_snobol4.sh writes --suite-total "$_sn4_shipped" and refuses when shipped=/outside= are absent
# FAIL_ONCE=1 rewrites the captured board line's shipped=3 to shipped=2 before arm (a) grades, to prove the arm trips.
# rc 0 = every arm holds; rc 1 = a FAIL named; rc 2 = REFUSED-TO-GRADE (no binary, no master to cut the fixture from).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
H="$HERE/corpus_suite_harness.py"; MASTER="${S4E_CORPUS_ROOT:-$HERE/../../corpus}/tests/snobol4"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -f "$H" ] || refuse "no harness at $H"
[ -x "$HERE/../scrip" ] || refuse "no ./scrip binary beside scripts/ -- make first"
[ -f "$MASTER/ALL.sno" ] && [ -f "$MASTER/ALL.ref" ] || refuse "no master pair under $MASTER to cut a fixture from"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_shipped_outside.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/tests/snobol4"; head -3 "$MASTER/ALL.sno" > "$W/tests/snobol4/ALL.sno"; head -3 "$MASTER/ALL.ref" > "$W/tests/snobol4/ALL.ref"
[ "$(wc -l < "$W/tests/snobol4/ALL.sno")" = 3 ] || refuse "the master's first three lines did not cut cleanly"
NAME="$(sed -n 2p "$W/tests/snobol4/ALL.sno" | sed 's/.*;\* //')"; [ -n "$NAME" ] || refuse "cannot read the second entry's name from its ';* name' tail"
printf '%s\tORACLE_REFUSES\tfixture: the oracle refuses this entry\n' "$NAME" > "$W/tests/snobol4/ALL.outside.tsv"
DB="$W/results.tsv"; printf 'ts_utc\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\tsecs\tnote\n' > "$DB"
echo "    fixture: 3 entries cut from the master; outside = $NAME"
fails=0; checks=0; ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: the board line prints PASS over the shipped population with OUTSIDE named (CEO-749) ==="
out="$(S4E_PROGRESS_DB="$DB" timeout 120 python3 "$H" run "$W/tests/snobol4/ALL.sno" "$W/tests/snobol4/ALL.ref" --modes m3,m4 --outside "$W/tests/snobol4/ALL.outside.tsv" 2>&1)"; rc=$?
[ "$rc" -le 1 ] || refuse "the harness could not grade the fixture (rc=$rc): $(tail -3 <<<"$out")"
board="$(grep '^SUITE_BOARD ' <<<"$out")"
[ -n "${FAIL_ONCE:-}" ] && board="${board/shipped=3/shipped=2}"
f(){ grep -oE " $1=[0-9]+" <<<"$board" | head -1 | cut -d= -f2; }
if [ "$(f total)" = 2 ] && [ "$(f shipped)" = 3 ] && [ "$(f outside)" = 1 ] && [ "$(f all_n)" = 2 ] && [ "$(f m3_n)" = 2 ] && [ "$(f m4_n)" = 2 ]; then
  ck ok "(a) SUITE_BOARD total=2 (graded) shipped=3 outside=1, all_n=2, m3_n=2 m4_n=2"
else ck no "(a) board fields -- total=$(f total) shipped=$(f shipped) outside=$(f outside) all_n=$(f all_n) m3_n=$(f m3_n) m4_n=$(f m4_n): $board"; fi
grep -q "^OUTSIDE_BASELINE $NAME ORACLE_REFUSES: fixture: the oracle refuses this entry" <<<"$out" && grep -q 'OUTSIDE_BASELINE_COUNT 1 .*IN the shipped one' <<<"$out" \
  && ck ok "(b) OUTSIDE_BASELINE names $NAME with its class and reason; the count line says IN the shipped population" \
  || ck no "(b) OUTSIDE lines -- got: $(grep OUTSIDE <<<"$out" | head -2)"
n_out="$(awk -F'\t' -v p="$NAME" '$8==p && $10=="OUTSIDE"{print $9":"$12}' "$DB" | sort | tr '\n' ' ')"
[ "$n_out" = "m3:ORACLE_REFUSES: fixture: the oracle refuses this entry m4:ORACLE_REFUSES: fixture: the oracle refuses this entry " ] \
  && ck ok "(c) the progress DB carries $NAME as OUTSIDE in m3 and m4 with note = class: reason" \
  || ck no "(c) DB OUTSIDE rows for $NAME: '$n_out'"
out2="$(S4E_PROGRESS_DB="$DB" timeout 120 python3 "$H" run "$W/tests/snobol4/ALL.sno" "$W/tests/snobol4/ALL.ref" --modes m3,m4 2>&1)"; board2="$(grep '^SUITE_BOARD ' <<<"$out2")"
g(){ grep -oE " $1=[0-9]+" <<<"$board2" | head -1 | cut -d= -f2; }
[ "$(g total)" = 3 ] && [ "$(g shipped)" = 3 ] && [ "$(g outside)" = 0 ] && ck ok "(d) CONTROL: without --outside the board prints total=3 shipped=3 outside=0" || ck no "(d) control board: total=$(g total) shipped=$(g shipped) outside=$(g outside)"
R="$HERE/test_corpus_snobol4.sh"
grep -q -- '--suite-total "\$_sn4_shipped"' "$R" && grep -q 'shipped=/outside= fields' "$R" && ck ok "(e) STATIC: test_corpus_snobol4.sh publishes --suite-total \$_sn4_shipped and refuses when shipped=/outside= are absent" || ck no "(e) test_corpus_snobol4.sh does not publish the shipped population or does not refuse on absent fields"
echo "population: $checks arm(s) graded, $fails FAIL"
[ "$fails" = 0 ] && { echo "GATE PASS [suite_board_line_prints_shipped_with_outside_named]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [suite_board_line_prints_shipped_with_outside_named]: $fails of $checks arms FAIL"; exit 1
