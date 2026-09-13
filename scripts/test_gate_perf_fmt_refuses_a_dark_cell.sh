#!/usr/bin/env bash
# test_gate_perf_fmt_refuses_a_dark_cell.sh -- ⭐ THE DARK-CELL LAW, LOCKED (CEO-676; hq_P 2026-09-13).
#
# A benchmark arm that cannot measure its subject must REFUSE, naming the subject -- it must never
# emit a blank cell, and a grid must refuse to publish a column it could not fill.  This gate holds
# lib_perf_fmt.sh, THE ONE AUTHORITY for printing a multiple, to that law.
#
# ⛔ WHAT IT IS GUARDING AGAINST, MEASURED ON THIS TREE BEFORE THE CURE -- every one of these exited
# rc=0, so a caller, a grid and a reader all saw success:
#     perf_mult 100 0      -> "n/a"        a blank cell, read as a measurement that came back empty
#     perf_mult 0   100    -> "0.000x"     ⛔ FABRICATED: a NUMBER.  It sorts, it colours RED, it gets quoted
#     perf_mult 100 ERROR  -> "n/a"        awk's `v + 0` turned a string into 0 and then into a cell
#     perf_mult 100 -5     -> "-20.000x"   ⛔ a NEGATIVE multiple, printed and coloured like a reading
# The last two were found by this gate's own author while writing it, which is the argument for the gate.
#
# ⛔ THIS GATE REFUSES rc=2 RATHER THAN SKIPPING when it cannot measure (no lib, no bash): a test that
# cannot run must never report the same string as one that passed.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
LIB="$HERE/lib_perf_fmt.sh"
[ -r "$LIB" ] || { echo "⛔ GATE REFUSES: cannot read $LIB -- this gate proves nothing without it"; exit 2; }
# shellcheck source=/dev/null
. "$LIB" || { echo "⛔ GATE REFUSES: $LIB failed to source"; exit 2; }
export PERF_COLOR=0
PASS=0; FAIL=0
ok()  { PASS=$((PASS+1)); printf '  ok   %s\n' "$1"; }
bad() { FAIL=$((FAIL+1)); printf '  FAIL %s\n     expected: %s\n     actual:   %s\n' "$1" "$2" "$3"; }
#-----------------------------------------------------------------------------------------------------
# A. A DARK CELL REFUSES: non-empty non-numeric token on stdout, rc=2, subject named on stderr.
dark() {
    local what="$1" ref="$2" ours="$3" subj="$4" out rc err
    err="$(mktemp)"; out="$(perf_mult "$ref" "$ours" "$subj" 2>"$err")"; rc=$?
    if [ "$rc" -ne 2 ]; then bad "$what: must exit 2" "rc=2" "rc=$rc (stdout='$out')"; rm -f "$err"; return; fi
    case "$out" in *REFUSED*) : ;; *) bad "$what: stdout must say REFUSED" "REFUSED" "'$out'"; rm -f "$err"; return ;; esac
    # ⛔ the cell must not be readable as a number -- that is shape (2), the fabricated multiple
    if printf '%s' "$out" | grep -Eq '^-?[0-9.]+x?$'; then bad "$what: cell is numeric" "non-numeric" "'$out'"; rm -f "$err"; return; fi
    if ! grep -q "$subj" "$err"; then bad "$what: stderr must NAME the subject" "mentions '$subj'" "$(tr '\n' ' ' <"$err")"; rm -f "$err"; return; fi
    rm -f "$err"; ok "$what refuses, names '$subj', cell='$out'"
}
echo "A. a subject that cannot be measured is REFUSED, never printed"
dark "zero ours (was n/a)"        100 0       "nreverse m3 vs gprolog"
dark "zero ref (was 0.000x)"      0   100     "deriv vs gprolog"
dark "non-numeric ours (was n/a)" 100 ERROR   "tak m4 vs swi"
dark "literal n/a ours"           100 n/a     "queens m3 vs gnu"
dark "empty ours"                 100 ""      "log10 m4 vs gnu"
dark "negative ours (was -20x)"   100 -5      "ham m3 vs swi"
dark "zero both"                  0   0       "crypt m3 vs gnu"
#-----------------------------------------------------------------------------------------------------
# B. A REAL MEASUREMENT IS UNTOUCHED.  ⛔ This half is why the gate can be trusted: a refusal rule that
# also broke live cells would be caught here, not in a grid three sessions later.
echo "B. a measurable subject still prints its multiple, unchanged"
m() { local want="$1" got; got="$(perf_mult "$2" "$3" 2>/dev/null)"
      [ "$got" = "$want" ] && ok "perf_mult $2 $3 = $want" || bad "perf_mult $2 $3" "$want" "$got"; }
m "2.000x"  100 50
m "0.500x"  50  100
m "1.000x"  7   7
m "1.628x"  1628 1000
p() { local want="$1" got; got="$(perf_pct "$2" "$3" 2>/dev/null)"
      [ "$got" = "$want" ] && ok "perf_pct $2 $3 = $want" || bad "perf_pct $2 $3" "$want" "$got"; }
p "100.0% faster" 200 100
p "50.0% slower"  100 200
#-----------------------------------------------------------------------------------------------------
# C. A GRID REFUSES TO PUBLISH A COLUMN IT COULD NOT FILL, and cannot open without its load stamp.
echo "C. the grid refuses as a whole, and states the load it ran under"
hdr="$(perf_grid_begin "x vs gprolog · WORK basis · RT_OPT=-O0 · m4" 2>/dev/null)"
case "$hdr" in *load*core*) ok "perf_grid_begin states the load and the core count" ;;
               *) bad "perf_grid_begin must carry a load stamp" "contains 'load ... cores'" "$hdr" ;; esac
perf_grid_begin "x vs gprolog" >/dev/null 2>&1
perf_mult 100 50 "clean cell" >/dev/null 2>&1
out="$(perf_grid_end 2>/dev/null)"; rc=$?
[ "$rc" -eq 0 ] && ok "a grid with no dark cell closes rc=0" || bad "clean grid must close rc=0" "rc=0" "rc=$rc ($out)"
perf_grid_begin "x vs gprolog" >/dev/null 2>&1
perf_mult 100 50 "clean cell"  >/dev/null 2>&1
perf_mult 100 0  "dark cell"   >/dev/null 2>&1
out="$(perf_grid_end 2>/dev/null)"; rc=$?
if [ "$rc" -eq 2 ] && printf '%s' "$out" | grep -q 'GRID REFUSES'; then ok "a grid with one dark cell REFUSES rc=2 and says so"
else bad "grid with a dark cell must refuse" "rc=2 + 'GRID REFUSES'" "rc=$rc ($out)"; fi
#-----------------------------------------------------------------------------------------------------
# D. A MISSING OPERAND PRINTS A ROW RATHER THAN DELETING ONE -- shape (3), the invisible cell.
echo "D. a missing operand prints a refusing row instead of vanishing"
row="$(perf_row_or_refuse "nreverse m3 vs gnu" "" "" 2>/dev/null)"
if [ -n "$row" ] && printf '%s' "$row" | grep -q 'nreverse m3 vs gnu' && printf '%s' "$row" | grep -q 'REFUSED'
then ok "perf_row_or_refuse still prints the labelled row"; else bad "row must survive a missing operand" "a row naming the kernel + REFUSED" "'$row'"; fi
#-----------------------------------------------------------------------------------------------------
# E. ⛔ THE COMPOSITION -- AND IT IS THE ARM WHOSE ABSENCE LET A DEFECT SURVIVE INSIDE ITS OWN GATE.
# Arm C refuses a grid whose dark cell came from a DIRECT perf_mult call (parent shell, so the
# PERF_DARK_CELLS bump survives).  Arm D proves the row still prints.  NEITHER ARM COMPOSED THEM, and
# perf_row used to call perf_mult inside the printf's own $( ) -- a SUBSHELL, where the bump died.  So
# every refusal arriving through perf_row, WHICH IS THE PATH EVERY HARNESS ACTUALLY USES, was invisible
# to perf_grid_end and the grid closed rc=0 with a hole in it.  Both arms passed the whole time.
# ⭐ A SEAM BETWEEN TWO PASSING GUARDS IS WHERE A MECHANISM LEAKS -- test the composition, not the parts.
echo "E. a dark cell arriving through perf_row reaches the grid verdict (the subshell seam)"
perf_grid_begin "x vs rival" >/dev/null 2>&1
perf_row "kernel a m3 vs rival" 200 100 >/dev/null 2>&1
perf_row "kernel b m3 vs rival" 100 ""  >/dev/null 2>&1
out="$(perf_grid_end 2>/dev/null)"; rc=$?
if [ "$rc" -eq 2 ] && printf '%s' "$out" | grep -q 'GRID REFUSES'; then ok "a refusal through perf_row reaches perf_grid_end (rc=2)"
else bad "a refusal through perf_row must reach the grid verdict" "rc=2 + 'GRID REFUSES'" "rc=$rc ($out)"; fi
# ⭐ CONTROL ARM, and it is not optional: the cure must not make EVERY grid refuse.  Valid cells -> rc=0.
perf_grid_begin "x vs rival" >/dev/null 2>&1
perf_row "kernel a m3 vs rival" 200 100 >/dev/null 2>&1
perf_row "kernel b m4 vs rival" 100 200 >/dev/null 2>&1
out="$(perf_grid_end 2>/dev/null)"; rc=$?
[ "$rc" -eq 0 ] && ok "a grid of valid cells through perf_row still closes rc=0" || bad "a clean perf_row grid must close rc=0" "rc=0" "rc=$rc ($out)"
#-----------------------------------------------------------------------------------------------------
# F. THE HARNESSES THEMSELVES -- the law is worth nothing while no harness is wired to it.  Measured
# 2026-09-13: NOT ONE bench script called perf_grid_begin/perf_grid_end, so the whole-grid refusal
# guarded nothing in production, and four harnesses carried 10 sites of the row-deleting guard
#     [ -n "$ref" ] && [ -n "$ours" ] && perf_row ...
# which DELETES the row when an operand is missing -- shape (3), the invisible cell: the grid looks
# complete because nobody greps for a row that was never printed.
echo "F. the bench harnesses cannot delete a row, and they close the grid they open"
for h in bench_triangulate_pascal.sh bench_triangulate_prolog.sh bench_triangulate_raku.sh bench_triangulate_snobol4.sh bench_triangulate_demos_icon.sh bench_triangulate_demos_snobol4.sh; do
    f="$HERE/$h"
    [ -r "$f" ] || { bad "$h: unreadable" "a readable harness" "missing"; continue; }
    if grep -q '\[ -n "\$[a-zA-Z0-9_]*" \].*perf_row' "$f"; then
        bad "$h: row-deleting guard present" "no '[ -n ] && perf_row' idiom" "$(grep -c '\[ -n "\$[a-zA-Z0-9_]*" \].*perf_row' "$f") site(s)"
    elif ! grep -q 'perf_grid_begin' "$f"; then bad "$h: grid never opened" "perf_grid_begin (it carries the load stamp)" "no call"
    elif ! grep -q 'perf_grid_end'   "$f"; then bad "$h: grid never closed" "perf_grid_end (the whole-grid refusal)" "no call"
    else ok "$h: no row-deleting guard, grid opened and closed"; fi
done
#-----------------------------------------------------------------------------------------------------
# G. NO HARNESS MAY CALL perf_row INSIDE $( ).  ⛔ THIS IS ARM E's SEAM ONE LEVEL OUT, AND IT SURVIVED
# ARM E's CURE.  perf_row was fixed to bump PERF_DARK_CELLS in its OWN shell, but a CALLER writing
#     echo "    $(perf_row ...)"
# puts perf_row in a subshell of its own making, so the bump dies there exactly as it used to die
# inside the printer -- and the grid closes rc=0 with a hole in it.  Measured 2026-09-13 on the two
# demos harnesses, the last three such sites in the tree: wrapped read PERF_DARK_CELLS=0 / grid rc=0,
# unwrapped read 1 / rc=2 on the identical refusing cell.
# ⭐ THE REASON THIS IS A GATE AND NOT A NOTE: arm F checks that a harness OPENS and CLOSES a grid,
# and a $( )-wrapped caller passes arm F while its verdict cannot fail.  A guard that is satisfied by
# the broken shape is the shape this lane keeps paying for -- so the census that found it lives here now.
echo "G. no bench harness calls perf_row inside a subshell"
gsites=0
for f in "$HERE"/bench_*.sh; do
    [ -r "$f" ] || continue
    n="$(grep -c '\$(perf_row' "$f" 2>/dev/null)" || n=0
    [ "$n" -eq 0 ] || { bad "$(basename "$f"): perf_row wrapped in \$( )" "a direct call (the dark-cell bump must land in the harness's own shell)" "$n site(s)"; gsites=$((gsites+n)); }
done
[ "$gsites" -eq 0 ] && ok "no bench harness wraps perf_row in \$( ) (census over $HERE/bench_*.sh)"
#-----------------------------------------------------------------------------------------------------
printf '\nGATE %s -- pass=%s fail=%s\n' "$([ "$FAIL" -eq 0 ] && echo PASS || echo FAIL)" "$PASS" "$FAIL"
[ "$FAIL" -eq 0 ] || exit 1
exit 0
