#!/usr/bin/env bash
export S4E_ONE_RUNNER_FIXTURE="gate arm ${0##*/}: three package runners invoked on scratch packages as instrument fixtures, not boards (CEO-1269, CEO-523)"
# test_gate_package_runners_grade_a_library_through_its_driver.sh -- THE LIBRARY RULE, HELD BY THE RUNNERS (ceo CEO-1269, CEO-700
# applied; coo 2026-09-25, row instruments-the-arizona-jcon-and-gimpel-runners-grade-a-library-through-its-driver-...). A library is
# the program and NAME_driver is its grading vehicle, never a program in the shipped population. So a package runner (a) excludes
# every *_driver file from the shipped count by name, and (b) grades the library by its driver -- the library's verdict in each mode
# is the driver's, recorded under the LIBRARY's name in the progress database. A library with no driver stays in the denominator as
# owed, a non-pass, named.
#
# ONE SCRATCH PACKAGE PER RUNNER, three libraries each: one with a passing driver, one with a failing driver and one with none. Each
# must read shipped 3, graded 2, pass 1, with the failing library and the driverless one NAMED. Its inventory must sum (the
# driverless library declared UNGRADED NEEDS_DRIVER), and its progress rows must be keyed by the libraries and never by a driver.
#   arizona  test_icon_arizona_suite.sh  S4E_HOME=<scratch>, package at corpus/packages/icon/arizona_tests/general
#   jcon     test_icon_jcon_suite.sh     --corpus <scratch>
#   gimpel   test_snobol4_gimpel_suite.sh  CORPUS=<scratch>, package at packages/snobol4/gimpel
# THE REFS ARE CUT LIVE FROM THE ORACLE (icont/iconx for Icon, sbl -bf for SNOBOL4) for the passing drivers. The failing driver's ref is
# deliberately wrong: 5 where the oracle prints 4. The Icon runners grade against the ref alone, so that driver is a FAIL. gimpel's
# scorecard also compares against the LIVE oracle, and SCRIP agrees with it, so its failing driver also reads &VERSION. SPITBOL refuses
# that (ERROR 251), the scorecard grades the driver against its pin alone, and the wrong pin reads FAIL whatever SCRIP answers.
# NOTHING LIVE IS TOUCHED: S4E_PROGRESS_DB is a scratch table per runner, S4E_SCORE_NO_WRITE shuts the leaderboard write, every
# package lives under mktemp.
# ⛔ FAILED ONCE ON THE PARENT (measured at the landing, recorded in its commit): the runners before the rule count the drivers as
# shipped programs and record their names.
# EXIT 0 every arm holds; 1 an arm is red; 2 REFUSED (an oracle or the fixture could not be built -- nothing was measured).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSED(2): lib_oracle_flags.sh unloadable"; exit 2; }
ICONT="$(icont_bin 2>/dev/null)"; ICONX="$(iconx_bin 2>/dev/null)"; SBL="$(sbl_correctness_bin 2>/dev/null)"
for b in "$ICONT" "$ICONX" "$SBL"; do [ -n "$b" ] && [ -x "$b" ] || { echo "⛔ REFUSED(2): an oracle is missing (icont='$ICONT' iconx='$ICONX' sbl='$SBL') -- a ref cannot be cut, so nothing can be graded"; exit 2; }; done
SCRATCH="${S4E_SCRATCH:-$(cd "$ROOT/.." && pwd)/.scratch}"; mkdir -p "$SCRATCH" || { echo "⛔ REFUSED(2): cannot create $SCRATCH"; exit 2; }
T="$(mktemp -d "$SCRATCH/gate_libdrv_XXXXXX")" || { echo "⛔ REFUSED(2): mktemp failed"; exit 2; }; trap 'rm -rf "$T"' EXIT
export S4E_SCORE_NO_WRITE="gate fixture ${0##*/}: a scratch package, never a board"
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
has() { grep -qE -- "$2" "$1"; }

# ---- the Icon fixture (the semicolon dialect SCRIP reads, CEO-495), refs cut from iconx
I="$T/icn"; C="$T/cut"; mkdir -p "$I" "$C" || exit 2
printf 'procedure dbl(x);\n   return 2 * x;\nend\n' > "$I/alib.icn"
printf 'link alib\nprocedure main();\n   write("dbl(21) = ", dbl(21));\n   write("dbl(-4) = ", dbl(-4));\nend\n' > "$I/alib_driver.icn"
printf 'procedure half(x);\n   return x / 2;\nend\n' > "$I/blib.icn"
printf 'link blib\nprocedure main();\n   write("half(9) = ", half(9));\nend\n' > "$I/blib_driver.icn"
printf 'procedure triple(x);\n   return 3 * x;\nend\n' > "$I/clib.icn"
printf '# fixture\n# name<TAB>CLASS<TAB>reason\nclib.icn\tNEEDS_DRIVER\ta library with no clib_driver.icn (gate fixture)\n' > "$I/UNGRADED.tsv"
printf '# fixture\n# name<TAB>CLASS<TAB>reason\n' > "$I/UNGRADABLE.tsv"
cp "$I"/*.icn "$C/" && ( cd "$C" && "$ICONT" -s -c alib.icn blib.icn && "$ICONT" -s -o ad alib_driver.icn && "$ICONT" -s -o bd blib_driver.icn ) </dev/null >/dev/null 2>&1 \
  || { echo "⛔ REFUSED(2): icont could not build the fixture's drivers"; exit 2; }
( cd "$C" && "$ICONX" ./ad ) > "$I/alib_driver.ref" 2>&1 </dev/null
( cd "$C" && "$ICONX" ./bd ) > "$C/blib_oracle.txt" 2>&1 </dev/null
grep -qx 'dbl(21) = 42' "$I/alib_driver.ref" && grep -qx 'half(9) = 4' "$C/blib_oracle.txt" || { echo "⛔ REFUSED(2): iconx did not answer the fixture as expected -- $(tr '\n' '|' < "$I/alib_driver.ref")"; exit 2; }
echo 'half(9) = 5' > "$I/blib_driver.ref"

# ---- the SNOBOL4 fixture in gimpel's shape (-INCLUDE the library), ALIB's ref cut from sbl -bf
G="$T/gp/corpus/packages/snobol4/gimpel"; mkdir -p "$G" || exit 2
printf "* ALIB - double a number\n\tDEFINE('DBL(X)')\t\t\t:(DBL_END)\nDBL\tDBL = 2 * X\t\t\t:(RETURN)\nDBL_END\n" > "$G/ALIB.sno"
printf -- "-INCLUDE \"ALIB.sno\"\n\tOUTPUT = 'DBL(21) = ' DBL(21)\n\tOUTPUT = 'DBL(-4) = ' DBL(-4)\nEND\n" > "$G/ALIB_driver.sno"
printf "* BLIB - halve a number\n\tDEFINE('HALF(X)')\t\t\t:(HALF_END)\nHALF\tHALF = X / 2\t\t\t:(RETURN)\nHALF_END\n" > "$G/BLIB.sno"
printf -- "-INCLUDE \"BLIB.sno\"\n\tOUTPUT = 'HALF(9) = ' HALF(9)\n\tOUTPUT = 'V=' &VERSION\nEND\n" > "$G/BLIB_driver.sno"
printf "* CLIB - triple a number\n\tDEFINE('TRIPLE(X)')\t\t\t:(TRIPLE_END)\nTRIPLE\tTRIPLE = 3 * X\t\t\t:(RETURN)\nTRIPLE_END\n" > "$G/CLIB.sno"
( cd "$G" && "$SBL" -bf ALIB_driver.sno ) > "$G/ALIB_driver.ref" 2>&1 </dev/null
grep -qx 'DBL(21) = 42' "$G/ALIB_driver.ref" || { echo "⛔ REFUSED(2): sbl -bf did not answer ALIB_driver as expected -- $(tr '\n' '|' < "$G/ALIB_driver.ref")"; exit 2; }
echo 'HALF(9) = 5' > "$G/BLIB_driver.ref"
printf '# fixture\n# name<TAB>CLASS<TAB>reason\nCLIB.sno\tNEEDS_DRIVER\ta library with no CLIB_driver.sno (gate fixture)\n' > "$G/UNGRADED.tsv"
printf '# fixture\n# name<TAB>CLASS<TAB>reason\n' > "$G/UNGRADABLE.tsv"

# rows <db> -- "program mode outcome" per recorded row, the program's basename without its extension
rows() { [ -f "$1" ] && awk -F'\t' 'NR>1 {p=$8; sub(/.*\//, "", p); sub(/\.[A-Za-z0-9]+$/, "", p); print p, $9, $10}' "$1"; }
progress_arms() {   # progress_arms <label> <db> <pass-lib> <fail-lib>
  local r; r="$(rows "$2")"
  { grep -qx "$3 m3 PASS" <<<"$r" && grep -qx "$3 m4 PASS" <<<"$r" && grep -qx "$4 m3 FAIL" <<<"$r" && grep -qx "$4 m4 FAIL" <<<"$r"; } \
    && ck ok "$1: progress rows are keyed by the libraries ($3 PASS, $4 FAIL, both modes)" \
    || ck no "$1: progress rows want $3 m3/m4 PASS and $4 m3/m4 FAIL, got: $(tr '\n' ';' <<<"$r")"
  grep -q '_driver ' <<<"$r" && ck no "$1: a progress row names a DRIVER: $(grep '_driver ' <<<"$r" | tr '\n' ';')" || ck ok "$1: no progress row names a driver"
}

echo "=== gate: a package runner grades a library through its driver, never the driver as a program (CEO-1269) ==="
# ---- arizona
AZ="$T/az/corpus/packages/icon/arizona_tests"; mkdir -p "$AZ/general" "$AZ/special" && cp "$I"/*.icn "$I"/*.ref "$AZ/general/" && cp "$I"/*.tsv "$AZ/"
S4E_HOME="$T/az" S4E_PROGRESS_DB="$T/az.tsv" timeout 600 bash "$HERE/test_icon_arizona_suite.sh" > "$T/az.out" 2>&1; rc=$?
if [ "$rc" = 2 ] || ! has "$T/az.out" '^ARIZONA_SUITE_BOARD '; then echo "⛔ REFUSED(2): the arizona runner did not measure the fixture (rc=$rc):"; tail -8 "$T/az.out" | sed 's/^/    /'; exit 2; fi
has "$T/az.out" '^ARIZONA_SUITE_BOARD shipped=3 graded=2 gap=1 m3_pass=1 ' && ck ok "arizona: shipped 3, graded 2 through their drivers, m3 pass 1" || ck no "arizona: $(grep -m1 '^ARIZONA_SUITE_BOARD' "$T/az.out" | cut -c1-120)"
has "$T/az.out" '^ARIZONA_AND_PER_PROGRAM and_pass=1 of 2 ' && ck ok "arizona: the AND per program is 1" || ck no "arizona: $(grep -m1 'AND_PER_PROGRAM' "$T/az.out" | cut -c1-120)"
has "$T/az.out" '^m3 FAIL: blib$' && has "$T/az.out" 'NOT GRADED .*general/clib' && ck ok "arizona: the failing library blib and the driverless clib are named" || ck no "arizona: blib/clib not named as non-passes"
has "$T/az.out" '^PACKAGE_INVENTORY package=arizona shipped=3 graded=2 ungraded=1 ' && ck ok "arizona: the inventory sums (clib owed, NEEDS_DRIVER)" || ck no "arizona: $(grep -m1 'PACKAGE_INVENTORY\|INVENTORY REFUSES' "$T/az.out" | cut -c1-140)"
progress_arms arizona "$T/az.tsv" alib blib
# ---- jcon
JC="$T/jc"; mkdir -p "$JC" && cp "$I"/*.icn "$I"/*.ref "$I"/*.tsv "$JC/"
S4E_PROGRESS_DB="$T/jc.tsv" timeout 600 bash "$HERE/test_icon_jcon_suite.sh" --corpus "$JC" > "$T/jc.out" 2>&1; rc=$?
if [ "$rc" = 2 ] || ! has "$T/jc.out" '^JCON_SUITE_BOARD '; then echo "⛔ REFUSED(2): the jcon runner did not measure the fixture (rc=$rc):"; tail -8 "$T/jc.out" | sed 's/^/    /'; exit 2; fi
has "$T/jc.out" '^JCON_SUITE_BOARD shipped=3 graded=2 gap=1 total=2 m3_pass=1 m4_pass=1' && ck ok "jcon: shipped 3, graded 2 through their drivers, pass 1 in each mode" || ck no "jcon: $(grep -m1 '^JCON_SUITE_BOARD' "$T/jc.out" | cut -c1-120)"
has "$T/jc.out" 'FAIL \(wrong output\): blib$' && has "$T/jc.out" '^UNGRADED .*clib\(' && ck ok "jcon: the failing library blib and the driverless clib are named" || ck no "jcon: blib/clib not named as non-passes"
has "$T/jc.out" '^PACKAGE_INVENTORY package=jcon shipped=3 graded=2 ungraded=1 ' && ck ok "jcon: the inventory sums (clib owed, NEEDS_DRIVER)" || ck no "jcon: $(grep -m1 'PACKAGE_INVENTORY\|INVENTORY REFUSES' "$T/jc.out" | cut -c1-140)"
progress_arms jcon "$T/jc.tsv" alib blib
# ---- gimpel
CORPUS="$T/gp/corpus" S4E_PROGRESS_DB="$T/gp.tsv" timeout 900 bash "$HERE/test_snobol4_gimpel_suite.sh" > "$T/gp.out" 2>&1; rc=$?
if [ "$rc" = 2 ] || ! has "$T/gp.out" '^GIMPEL_BOARD '; then echo "⛔ REFUSED(2): the gimpel runner did not measure the fixture (rc=$rc -- another SNOBOL4 board on this box is a refusal, not a red):"; tail -8 "$T/gp.out" | sed 's/^/    /'; exit 2; fi
has "$T/gp.out" '^GIMPEL_LIBRARIES shipped=3 graded_through_a_driver=2 no_driver=1 ' && ck ok "gimpel: shipped 3 libraries, 2 graded through their drivers, 1 with none" || ck no "gimpel: $(grep -m1 'GIMPEL_LIBRARIES' "$T/gp.out" | cut -c1-120)"
has "$T/gp.out" '^GIMPEL_BOARD total=2 .* m3_pass=1 m3_fail=1 m4_pass=1 m4_fail=1 ' && ck ok "gimpel: pass 1 in each mode" || ck no "gimpel: $(grep -m1 '^GIMPEL_BOARD' "$T/gp.out" | cut -c1-120)"
has "$T/gp.out" '^  RED    BLIB\.sno \(driver ' && has "$T/gp.out" '^NO DRIVER .*: CLIB$' && ck ok "gimpel: the failing library BLIB and the driverless CLIB are named" || ck no "gimpel: BLIB/CLIB not named as non-passes"
has "$T/gp.out" '^PACKAGE_INVENTORY package=gimpel shipped=3 graded=2 ungraded=1 ' && ck ok "gimpel: the inventory sums (CLIB owed, NEEDS_DRIVER)" || ck no "gimpel: $(grep -m1 'PACKAGE_INVENTORY\|INVENTORY REFUSES' "$T/gp.out" | cut -c1-140)"
has "$T/gp.out" 'both_modes_pass=1/3 shipped libraries' && ck ok "gimpel: the row it would publish is 1/3 over the shipped libraries" || ck no "gimpel: the row text is not 1/3 over the libraries: $(grep -m1 -o 'both_modes_pass=[^ ]*' "$T/gp.out")"
progress_arms gimpel "$T/gp.tsv" ALIB BLIB

echo "population: $checks arm(s) over 3 runners, 3 scratch packages of 3 libraries each; refs cut live from iconx and sbl -bf"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [package_runners_grade_a_library_through_its_driver]: $checks of $checks arms hold"; exit 0; fi
echo "⛔ GATE FAIL(1) [package_runners_grade_a_library_through_its_driver]: $fails of $checks arms red"; exit 1
