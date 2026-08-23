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
# USAGE
#   . "$(dirname "$0")/lib_perf_fmt.sh"
#   perf_mult <reference_cost> <our_cost>        -> "1.628x" (green) / "0.734x" (red)
#   perf_pct  <before_cost>    <after_cost>      -> "24.3% faster" / "8.0% slower"
#   perf_row  <label> <ref> <ours>               -> one aligned, coloured table row
#   PERF_COLOR=0 forces plain; PERF_COLOR=1 forces colour even when not a tty.
perf_color_on() {
    case "${PERF_COLOR:-auto}" in 0|no|off) return 1 ;; 1|yes|on) return 0 ;; esac
    [ -t 1 ] || return 1                       # not a terminal: a pipe or a log, so plain
    [ "${TERM:-dumb}" != dumb ] || return 1
    return 0
}
# perf_mult REF OURS -- the multiple, coloured, with its unit.  Nothing else.
perf_mult() {
    [ $# -eq 2 ] || { echo "perf_mult: need <reference_cost> <our_cost>" >&2; return 2; }
    awk -v r="$1" -v o="$2" -v c="$(perf_color_on && echo 1 || echo 0)" 'BEGIN{
        if (o + 0 == 0) { print "n/a"; exit }
        m = r / o
        s = sprintf("%.3fx", m)
        if (c) { print (m >= 1.0 ? "\033[92m" : "\033[31m") s "\033[0m" } else { print s }
    }'
}
# perf_pct BEFORE AFTER -- a DELTA, and a delta is the one form allowed to say the word.
# BEFORE/AFTER are COSTS (lower is better), so after < before is faster.
perf_pct() {
    [ $# -eq 2 ] || { echo "perf_pct: need <before_cost> <after_cost>" >&2; return 2; }
    awk -v b="$1" -v a="$2" -v c="$(perf_color_on && echo 1 || echo 0)" 'BEGIN{
        if (a + 0 == 0 || b + 0 == 0) { print "n/a"; exit }
        m = b / a; p = (m - 1) * 100
        s = (p >= 0) ? sprintf("%.1f%% faster", p) : sprintf("%.1f%% slower", -p)
        if (c) { print (p >= 0 ? "\033[92m" : "\033[31m") s "\033[0m" } else { print s }
    }'
}
# perf_row LABEL REF OURS -- one row of a grid.  ⛔ The grid's own header still has to name
# the axis ONCE ("x vs <reference>") and carry its SHARED-AXES line; this only formats.
perf_row() { printf '  %-34s %14s %14s   %s\n' "$1" "$2" "$3" "$(perf_mult "$2" "$3")"; }
