#!/usr/bin/env bash
# lib_perf_fmt.sh -- ⭐ THE ONE AUTHORITY for PRINTING a benchmark multiple.
#
# THE RULE IT IMPLEMENTS (Lon, 2026-08-23 s266; authority .github/RULES.md § FACT RULES):
#   THE UNIT IS `x`, A MULTIPLE, ON THE FASTER AXIS.  multiple = reference / ours.
#   2.00x is twice the reference's speed, 0.50x is half.  THE NUMBER IS THE DIRECTION.
#   ⛔ FASTER and SLOWER are NOT UNITS and never attach to a multiple: "0.666 slower" is
#      self-contradictory, because *slower* has its own multiple (1.5x as slow), so the
#      phrase names two answers at once.  "2x slower" is the same disease from the other
#      side -- it means 0.50x and reads as the bigger number.
#   ✅ PERCENTAGES may take the English word, and only percentages: 10% faster = 1.10x,
#      10% slower = 0.90x.  perf_pct below is the one that is allowed to say it.
#   ⭐ COLOUR WHERE THE MEDIUM SUPPORTS IT (Lon, same session): below 1.00x RED, at or
#      above 1.00x GREEN.  Colour is a SECOND channel, never the only one -- every
#      function here prints a number that is still correct with the colour stripped,
#      because it will be: pipes, logs, CI capture and `less` all lose it.
#
# ⛔ WHY THIS IS A FILE AND NOT A CONVENTION.  Six harnesses printed their own ratios and
# three of them printed them in opposite directions -- that is how GOAL-HQ-PERFORM's RUNG
# P-0 table came to run two divisors down one column.  A rule that lives only in markdown
# is re-derived by every script author.  Callers that cannot load this file must REFUSE,
# not fall back to a private format -- same law as lib_oracle_flags.sh.
#
#=====================================================================================================
# ⛔⭐ THE DARK-CELL LAW (CEO-676, dark-is-worse-than-red CEO-582 arriving in the benchmark harness;
# hq_P 2026-09-13).  A BENCHMARK ARM THAT CANNOT MEASURE ITS SUBJECT REFUSES, NAMING THE SUBJECT.
# It never emits a blank cell, and a grid refuses to publish a column it could not fill.
#
# THE THREE SHAPES THIS FILE USED TO PRODUCE, ALL MEASURED IN THIS TREE, ALL NOW REFUSED:
#   (1) BLANK -- `if (o + 0 == 0) { print "n/a" }`.  A tidy two-letter cell that reads as a
#       measurement which came back empty, when the truth is that no measurement happened.
#   (2) FABRICATED -- a sub-floor reading printed as 0 became "0.000x" for deriv-vs-gprolog
#       (bench_prolog_vanroy.sh:78 records it).  WORSE than blank: it is a NUMBER, it sorts,
#       it colours RED, and it will be quoted.  awk's `o + 0` also silently turns any
#       non-numeric string into 0, so a cell holding "ERROR" rendered as a multiple.
#   (3) INVISIBLE -- callers guard with `[ -n "$rf" ] && [ -n "$r3" ] && perf_row ...`, so a
#       missing operand DELETES THE ROW.  A grid with a row missing looks complete; nobody
#       greps for a row that was never printed.  perf_row_or_refuse is the cure for that one.
# ⭐ A caller that already knows its subject is below the instrument should say so in its own
# words (bench_icon_kernels.sh prints "<1 tick") -- that is a MEASUREMENT, not a dark cell, and
# it is exactly why the refusal names the subject rather than guessing on the caller's behalf.
#
# ⛔⭐ AND A VERDICT WITHOUT ITS LOAD IS NOT A VERDICT (CEO-697 one layer deeper, hq_P 2026-09-13).
# The ruling says a COST without the load it ran under is not a cost.  Measured on this box: two
# runs of IDENTICAL angle-2 code differ by up to 2.15x, and gnu's `cal` column moved 1,352,177 ->
# 742,684 iterations/s (1.82x) between two runs of unchanged code.  Two van Roy gates PASS on a
# busy box and FAIL on an idle one, because a budget-driven harness picks its iteration count from
# how fast the box is.  A pass/fail that silently tracks machine load is the one disguise nobody
# audits, because a green gate is never investigated.  So the load stamp lives HERE, in the
# printer, and not in a caller's good intentions: perf_grid_begin welds it to the shared-axes line
# every grid must already print, so a grid cannot open without stating the load it ran under.
#=====================================================================================================
#
# USAGE
#   . "$(dirname "$0")/lib_perf_fmt.sh"
#   perf_mult <reference_cost> <our_cost> [subject]  -> "1.628x" (green) / "0.734x" (red)
#                                                       or REFUSED (rc=2, subject named on stderr)
#   perf_pct  <before_cost>    <after_cost> [subject] -> "24.3% faster" / "8.0% slower"
#   perf_row  <label> <ref> <ours>                   -> one aligned, coloured table row
#   perf_row_or_refuse <label> <ref> <ours>          -> same, but a missing operand PRINTS a
#                                                       refusing row instead of vanishing
#   perf_load_stamp                                  -> "load 1.71 on 16 cores (0.11/core)"
#   perf_grid_begin <axis-line>                      -> the shared-axes line WITH the load stamp
#   perf_grid_end                                    -> rc=2 if any cell in this grid refused
#   PERF_COLOR=0 forces plain; PERF_COLOR=1 forces colour even when not a tty.
#   PERF_DARK_CELLS counts refusals since the last perf_grid_begin.  Read it, do not set it.
perf_color_on() {
    case "${PERF_COLOR:-auto}" in 0|no|off) return 1 ;; 1|yes|on) return 0 ;; esac
    [ -t 1 ] || return 1                       # not a terminal: a pipe or a log, so plain
    [ "${TERM:-dumb}" != dumb ] || return 1
    return 0
}
#-----------------------------------------------------------------------------------------------------
# perf_is_cost VALUE -- is this a usable COST?  A cost is a positive real.  ⛔ This is the guard that
# stops awk's `v + 0` from silently turning "ERROR", "" or "n/a" into 0 and then into a multiple.
# Zero is NOT a usable cost: a zero denominator is not an infinitely fast program, it is a program
# the instrument could not see, and the caller must say which of those it means.
perf_is_cost() {
    case "$1" in ''|*[!0-9.eE+-]*) return 1 ;; esac
    awk -v v="$1" 'BEGIN{ exit !(v + 0 > 0 && v ~ /^[+]?[0-9]*[.]?[0-9]+([eE][+-]?[0-9]+)?$/) }'
}
#-----------------------------------------------------------------------------------------------------
# perf_refuse SUBJECT WHY -- the ONE way this file reports an unmeasurable subject.  Prints a loud,
# non-blank, non-numeric token on stdout so the cell cannot be read as a measurement, names the
# subject and the reason on stderr, counts itself, and returns 2.
perf_refuse() {
    PERF_DARK_CELLS=$(( ${PERF_DARK_CELLS:-0} + 1 ))
    printf 'REFUSED'
    printf '⛔ perf: REFUSED to print a multiple for %s -- %s (a dark cell is worse than a red one: CEO-676)\n' \
           "${1:-<unnamed subject>}" "${2:-unmeasurable operand}" >&2
    return 2
}
#-----------------------------------------------------------------------------------------------------
# perf_mult REF OURS [SUBJECT] -- the multiple, coloured, with its unit.  Nothing else.
# ⛔ REFUSES rather than printing "n/a" or a fabricated "0.000x"; see THE DARK-CELL LAW above.
perf_mult() {
    [ $# -ge 2 ] || { echo "perf_mult: need <reference_cost> <our_cost> [subject]" >&2; return 2; }
    local subj="${3:-reference=$1 ours=$2}"
    perf_is_cost "$1" || { perf_refuse "$subj" "reference cost is not a positive number: '$1'"; return 2; }
    perf_is_cost "$2" || { perf_refuse "$subj" "our cost is not a positive number: '$2'"; return 2; }
    awk -v r="$1" -v o="$2" -v c="$(perf_color_on && echo 1 || echo 0)" 'BEGIN{
        m = r / o
        s = sprintf("%.3fx", m)
        if (c) { print (m >= 1.0 ? "\033[92m" : "\033[31m") s "\033[0m" } else { print s }
    }'
}
#-----------------------------------------------------------------------------------------------------
# perf_pct BEFORE AFTER [SUBJECT] -- a DELTA, and a delta is the one form allowed to say the word.
# BEFORE/AFTER are COSTS (lower is better), so after < before is faster.
perf_pct() {
    [ $# -ge 2 ] || { echo "perf_pct: need <before_cost> <after_cost> [subject]" >&2; return 2; }
    local subj="${3:-before=$1 after=$2}"
    perf_is_cost "$1" || { perf_refuse "$subj" "before cost is not a positive number: '$1'"; return 2; }
    perf_is_cost "$2" || { perf_refuse "$subj" "after cost is not a positive number: '$2'"; return 2; }
    awk -v b="$1" -v a="$2" -v c="$(perf_color_on && echo 1 || echo 0)" 'BEGIN{
        m = b / a; p = (m - 1) * 100
        s = (p >= 0) ? sprintf("%.1f%% faster", p) : sprintf("%.1f%% slower", -p)
        if (c) { print (p >= 0 ? "\033[92m" : "\033[31m") s "\033[0m" } else { print s }
    }'
}
#-----------------------------------------------------------------------------------------------------
# perf_row LABEL REF OURS -- one row of a grid.  ⛔ The grid's own header still has to name
# the axis ONCE ("x vs <reference>") and carry its SHARED-AXES line; this only formats.
# ⭐ The label is TRUNCATED to its field, never wrapped: a row that wraps turns a grid into prose,
# and Lon's ruling on the suite banner was that an unreadable grid is not a grid (2026-09-13).
# ⛔ THE SUBSHELL SEAM, AND IT MADE perf_grid_end INERT ON THE ONLY PATH THAT MATTERS (hq_P 2026-09-13).
# This function used to call perf_mult inside the printf's own $( ), so perf_refuse's
# PERF_DARK_CELLS bump happened in a SUBSHELL and never reached the parent: every refusal arriving
# through perf_row was invisible to perf_grid_end, which then closed rc=0 on a grid with a hole in it.
# ⭐ The two gate arms that should have caught it BOTH PASSED -- arm C refused a grid whose dark cell
# came from a DIRECT perf_mult call (parent shell, counter survives) and arm D proved the row prints
# REFUSED -- and nothing composed them. A seam between two passing guards is where a mechanism leaks.
# So the multiple is captured FIRST, its rc read in THIS shell, and the count bumped here.
perf_row() {
    local lbl="$1" mult rc
    [ "${#lbl}" -le 34 ] || lbl="${lbl:0:31}..."
    mult="$(perf_mult "$2" "$3" "$1")"; rc=$?
    [ "$rc" -eq 0 ] || PERF_DARK_CELLS=$(( ${PERF_DARK_CELLS:-0} + 1 ))
    printf '  %-34s %14s %14s   %s\n' "$lbl" "$2" "$3" "$mult"
}
#-----------------------------------------------------------------------------------------------------
# perf_row_or_refuse LABEL REF OURS -- perf_row for callers that today write
#     [ -n "$ref" ] && [ -n "$ours" ] && perf_row "$k" "$ref" "$ours"
# and so DELETE the row when an operand is missing.  A deleted row is shape (3) of the dark cell: the
# grid looks complete because nobody greps for a row that was never printed.  This prints the row.
perf_row_or_refuse() { perf_row "$1" "${2:-<none>}" "${3:-<none>}"; }
#-----------------------------------------------------------------------------------------------------
# perf_load_stamp -- the load this measurement ran under.  ⛔ Not decoration: on this 16-core box two
# runs of identical code differ by up to 2.15x, so a rate without this line is not a reading.
perf_load_stamp() {
    local la cores
    la="$(awk '{print $1}' /proc/loadavg 2>/dev/null)" || la=""
    cores="$(getconf _NPROCESSORS_ONLN 2>/dev/null)" || cores=""
    [ -n "$la" ] && [ -n "$cores" ] || { printf 'load UNKNOWN (⛔ no /proc/loadavg -- treat every rate below as ungraded)'; return 0; }
    awk -v l="$la" -v c="$cores" 'BEGIN{ printf "load %s on %s cores (%.2f/core)", l, c, l/c }'
}
#-----------------------------------------------------------------------------------------------------
# perf_grid_begin AXIS-LINE -- open a grid.  Emits the shared-axes line the FACT RULE requires ONCE,
# with the load stamp welded to it, and resets the dark-cell counter.
perf_grid_begin() {
    PERF_DARK_CELLS=0
    printf '  %s\n  measured at %s · %s\n' "${1:-x vs reference (basis and RT_OPT unstated -- ⛔ name them)}" \
           "$(date -u '+%Y-%m-%dT%H:%M:%SZ')" "$(perf_load_stamp)"
}
#-----------------------------------------------------------------------------------------------------
# perf_grid_end -- close a grid.  ⛔ A GRID REFUSES TO PUBLISH A COLUMN IT COULD NOT FILL: if any cell
# refused, say how many and return 2 so the caller's `set -e` or its own verdict line carries it out.
perf_grid_end() {
    [ "${PERF_DARK_CELLS:-0}" -eq 0 ] && return 0
    printf '  ⛔ GRID REFUSES: %s cell(s) could not measure their subject (named on stderr above); this grid is INCOMPLETE and must not be quoted as a reading\n' \
           "$PERF_DARK_CELLS"
    return 2
}
