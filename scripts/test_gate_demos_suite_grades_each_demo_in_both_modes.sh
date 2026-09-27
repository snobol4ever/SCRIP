#!/usr/bin/env bash
export S4E_ONE_RUNNER_FIXTURE="gate arm ${0##*/}: test_demos_suite.sh invoked on a scratch demo tree as an instrument fixture, not a board (CEO-1312)"
# test_gate_demos_suite_grades_each_demo_in_both_modes.sh -- THE DEMO SUITE ROW'S RUNNER AND THE WORKHORSE READER (Lon 2026-09-27: "all
# demos are also benchmarks. They are work-horse benchmarks." and "So demos are test suites in that they have a small input ... and a ref
# file."; ceo CEO-1312/1313; coo COO-206).
# ONE SCRATCH DEMO TREE (outside every corpus checkout), graded by test_demos_suite.sh snobol4:
#   a/hello.sno   PASS both modes, a .workhorse sidecar            b/echo.sno   reads its SAMPLE stdin from echo.in (a symlink to a shared
#   c/bad.sno     a wrong ref: FAIL, named                                        file, the shape shared demo inputs take), PASS both modes
#   d/noref.sno   no .ref: FAIL, counted and named, never skipped  lib/util.sno declared in CONTAINERS.tsv: out of the population
# ARMS: (1) the board line reads total=4 both_modes_pass=2 m3_pass=2 m4_pass=2 containers=1 workhorse_declared=1; (2) the reds are named,
# the no-ref one as no-ref; (3) the progress rows land in a scratch table as class benchmark, suite snobol4-demos, keyed demos/snobol4/<r>,
# one per program and mode (8 rows, the container none); (4) the fixture tree is never written as a suite row; (5)-(8) the workhorse
# reader: a valid sidecar echoes stdin/scale/argv, and an unknown key, a missing stdin file and a zero scale each REFUSE rc 2.
# FAILED ONCE: arms (1)-(4) need the runner, which did not exist before this landing; (5)-(8) the reader likewise.
# EXIT 0 every arm holds; 1 an arm is red; 2 REFUSED (the fixture could not be built).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
G=demos_suite_grades_each_demo_in_both_modes
[ -x "$ROOT/scrip" ] || { echo "⛔ REFUSED(2) [$G]: no scrip binary -- make first"; exit 2; }
SCRATCH="${S4E_SCRATCH:-$(cd "$ROOT/.." && pwd)/.scratch}"; mkdir -p "$SCRATCH" || { echo "⛔ REFUSED(2) [$G]: cannot create $SCRATCH"; exit 2; }
T="$(mktemp -d "$SCRATCH/gate_demos_XXXXXX")" || { echo "⛔ REFUSED(2) [$G]: mktemp failed"; exit 2; }; trap 'rm -rf "$T"' EXIT
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
F="$T/demos/snobol4"; mkdir -p "$F/a" "$F/b" "$F/c" "$F/d" "$F/lib"
printf "\tOUTPUT = 'hi'\nEND\n" > "$F/a/hello.sno"; printf 'hi\n' > "$F/a/hello.ref"; printf 'stdin\t-\nwhy\tgate fixture\n' > "$F/a/hello.workhorse"
printf "LOOP\tOUTPUT = INPUT\t:S(LOOP)\nEND\n" > "$F/b/echo.sno"; printf 'one\ntwo\n' > "$F/b/shared.input"; ln -s shared.input "$F/b/echo.in"; printf 'one\ntwo\n' > "$F/b/echo.ref"
printf "\tOUTPUT = 'two'\nEND\n" > "$F/c/bad.sno"; printf 'one\n' > "$F/c/bad.ref"
printf "\tOUTPUT = 'x'\nEND\n" > "$F/d/noref.sno"
printf "\tDEFINE('U()')\t:(U_END)\nU\t:(RETURN)\nU_END\n" > "$F/lib/util.sno"
# (e) a link manifest naming a module OUTSIDE the demo tree by a source-relative path, as the JCON demos do
#     (link "../../../packages/icon/jcon-compiler/dump"): the scratch copy must keep the program's depth or this reads
#     "link: cannot open" in both modes (coo COO-209)
mkdir -p "$F/e" "$T/packages/gate"; printf 'procedure greet()\n   write("linked")\nend\n' > "$T/packages/gate/mod.icn"
printf 'link "../../../packages/gate/mod"\nprocedure main()\n   greet()\nend\n' > "$F/e/linked.icn"; printf 'linked\n' > "$F/e/linked.ref"
printf '# fixture\nlib/util.sno\tLIBRARY\tincluded by nothing here (gate fixture)\n' > "$F/CONTAINERS.tsv"
out="$(DEMOS_DIR="$F" S4E_PROGRESS_DB="$T/db.tsv" timeout 600 bash "$HERE/test_demos_suite.sh" snobol4 2>&1)"; rc=$?
[ "$rc" = 2 ] && { echo "⛔ REFUSED(2) [$G]: the runner did not measure the fixture:"; printf '%s\n' "$out" | grep -m3 -E 'REFUS|UNPROVEN' | sed 's/^/    /'; exit 2; }
_b="$(printf '%s\n' "$out" | grep -m1 '^DEMOS_SUITE_BOARD ')"; _ok=1
for kv in lang=snobol4 total=5 shipped=6 all_pass=3 all_n=5 m3_pass=3 m4_pass=3 containers=1 workhorse_declared=1; do grep -qE "(^| )$kv( |\$)" <<<"$_b" || _ok=0; done
[ "$_ok" = 1 ] && ck ok "(1) the board: a DEMOS_SUITE_BOARD line, total=5 shipped=6 all_pass=3 all_n=5 m3/m4 3 containers=1 workhorse_declared=1 (the shared stdin read through echo.in; the link manifest resolved through the depth-keeping copy)" || ck no "(1) the board line: $_b"
# (1b) the writer READS the fraction off that line by name and would archive it verbatim (CEO-827): a receipt that is prose is the CEO-1325 fault
# (the write path itself gates on a clean committed tree and the lane seat, neither of which a gate may assume, so the
#  READER is called directly: the same two functions `write` runs on --text)
_w="$(python3 - "$HERE" "$_b" <<'PY'
import sys; sys.path.insert(0, sys.argv[1]); import util_score_row as u
line = sys.argv[2]; m = u.BOARD_LINE_RX.search(line)
p, t, why = u.fraction_from_text(line, None, None)
print("archivable=%s pass=%s total=%s why=%s" % ("yes" if m else "no", p, t, why))
PY
)"
grep -qx 'archivable=yes pass=3 total=5 why=None' <<<"$_w" && ck ok "(1b) util_score_row reads all_pass=3 all_n=5 off the DEMOS_SUITE_BOARD line by name and its archiver matches it (CEO-827)" || ck no "(1b) the writer's reader on the board line: $_w"
{ grep -q 'c/bad.sno(m3=FAIL,m4=FAIL)' <<<"$out" && grep -q 'd/noref.sno(no-ref)' <<<"$out"; } && ck ok "(2) the reds are named, the ref-less demo as no-ref" || ck no "(2) the named reds: $(grep 'NOT BOTH' <<<"$out")"
_rows="$(awk -F'\t' 'NR>1 && $5=="benchmark" && $6=="snobol4-demos" {print $8" "$9" "$10}' "$T/db.tsv" 2>/dev/null | sort)"
{ [ "$(printf '%s\n' "$_rows" | grep -c .)" = 10 ] && grep -qx 'demos/snobol4/e/linked.icn m4 PASS' <<<"$_rows" && grep -qx 'demos/snobol4/a/hello.sno m3 PASS' <<<"$_rows" && grep -qx 'demos/snobol4/d/noref.sno m4 FAIL' <<<"$_rows" && ! grep -q util <<<"$_rows"; } \
  && ck ok "(3) 10 progress rows (the linked Icon manifest PASS in both modes), class benchmark, suite snobol4-demos, keyed demos/snobol4/<r>, the container none" || ck no "(3) progress rows: $(tr '\n' ';' <<<"$_rows")"
grep -q 'not the canonical demo tree -- a fixture, never written' <<<"$out" && ck ok "(4) the scratch tree is never written as a suite row" || ck no "(4) the fixture was not declared unwritten"
. "$HERE/lib_declared_arena.sh" || { echo "⛔ REFUSED(2) [$G]: lib_declared_arena.sh unloadable"; exit 2; }
W="$T/wh"; mkdir -p "$W"; printf "\tOUTPUT = 1\nEND\n" > "$W/p.sno"; printf 'full\n' > "$W/full.dat"
printf 'stdin\tfull.dat\nscale\t16\nargv\t-x 1\n' > "$W/p.workhorse"; v="$(declared_workhorse_beside "$W/p.sno" 2>/dev/null)"; r=$?
[ "$r" = 0 ] && [ "$v" = "stdin=full.dat scale=16 argv=-x 1" ] && ck ok "(5) a valid workhorse sidecar echoes stdin=full.dat scale=16 argv=-x 1" || ck no "(5) the valid sidecar: rc=$r [$v]"
printf 'stdin\tfull.dat\nloops\t3\n' > "$W/p.workhorse"; declared_workhorse_beside "$W/p.sno" >/dev/null 2>&1; r=$?
[ "$r" = 2 ] && ck ok "(6) an unknown key (loops) REFUSES rc 2 -- a demo is never run in a loop" || ck no "(6) unknown key rc=$r"
printf 'stdin\tnot_there.dat\n' > "$W/p.workhorse"; declared_workhorse_beside "$W/p.sno" >/dev/null 2>&1; r=$?
[ "$r" = 2 ] && ck ok "(7) a stdin file that is not beside the program REFUSES rc 2" || ck no "(7) missing stdin file rc=$r"
printf 'stdin\tfull.dat\nscale\t0\n' > "$W/p.workhorse"; declared_workhorse_beside "$W/p.sno" >/dev/null 2>&1; r=$?
[ "$r" = 2 ] && ck ok "(8) a zero scale REFUSES rc 2" || ck no "(8) zero scale rc=$r"
echo "population: $checks arm(s) over one scratch demo tree of 6 files (5 programs, 1 container, one an Icon link manifest into a mirrored packages root) and 4 workhorse sidecars"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$G]: $checks of $checks arms hold"; exit 0; fi
echo "⛔ GATE FAIL(1) [$G]: $fails of $checks arms red"; exit 1
