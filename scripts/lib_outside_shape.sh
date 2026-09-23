#!/usr/bin/env bash
# lib_outside_shape.sh -- THE CEO-749 SHAPE FOR A PACKAGE ROW, ONE AUTHORITY FOR EVERY RUNNER THAT PUBLISHES ONE (row
# snobol4-four-package-rows-leave-their-outside-baseline-programs-out-of-the-denominator-ceo-749, hq_snobol4 2026-09-23).
#
# CEO-749 and Lon's word of 2026-09-16 ("Get those fixed"): a program outside the SPITBOL baseline -- one the oracle itself
# cannot answer, listed in the package's OUTSIDE_SPITBOL_BASELINE.tsv -- STAYS IN ITS ROW'S DENOMINATOR, named, as debt to cure.
# The shape is PASS over SHIPPED with OUTSIDE=k named in the same row. Before this file, five package runners published
# PASS over GRADED: Gimpel 127/132 with 12 listed, Budne 71/71 with 49 listed, Flake 110/124 with 56 listed and Dotnet 5/5
# with 9 listed. TPgm's 1/8 OUTSIDE=6 had been restored BY HAND, and its own runner would have undone it on the next run.
#
# THE MECHANISM IT RIDES: util_score_row.py refuses a denominator move without a --criterion-changed stamp, and APPENDS the
# stamp to SUITES.tsv column 12; util_suite_banner.py renders the LAST OUTSIDE=N token of column 12 beside the fraction.
# So a runner owes a stamp exactly when the published total moves OR the outside count differs from the last OUTSIDE=N
# already recorded -- and owes nothing on a run that changes neither, so the criterion date is not rewritten every run.
#
# USAGE (source it, then):  stamp="$(outside_shape_stamp <suites-key> <shipped-total> <outside-count>)"
#   prints the '<YYYY-MM-DD>:<reason>' stamp when one is owed, nothing when none is. An explicit S4E_CRITERION_CHANGED
#   always wins at the call site (the runner passes ${S4E_CRITERION_CHANGED:-$stamp}).
# A missing SUITES.tsv prints a stamp (a first row always names its criterion); a malformed count REFUSES rc=2.
outside_shape_stamp() {
  local key="${1:-}" total="${2:-}" out="${3:-}" tsv prev cc last
  case "$total$out" in ''|*[!0-9]*) echo "⛔ REFUSE(2) outside_shape_stamp: total [$total] and outside [$out] must be counts" >&2; return 2;; esac
  [ -n "$key" ] || { echo "⛔ REFUSE(2) outside_shape_stamp: no suites key" >&2; return 2; }
  tsv="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}/.github/SUITES.tsv"
  if [ -f "$tsv" ]; then
    prev="$(awk -F'\t' -v k="$key" '$1==k {print $10; exit}' "$tsv")"
    cc="$(awk -F'\t' -v k="$key" '$1==k {print $12; exit}' "$tsv")"
    last="$(printf '%s\n' "$cc" | grep -oE 'OUTSIDE=[0-9]+' | tail -1 | cut -d= -f2)"
    [ "$prev" = "$total" ] && [ "$last" = "$out" ] && return 0
  fi
  printf '%s:CEO-749-shape-pass-over-shipped-%s-with-OUTSIDE=%s-named-in-OUTSIDE_SPITBOL_BASELINE.tsv-they-stay-in-the-denominator-as-debt-to-cure' "$(date +%F)" "$total" "$out"
}
