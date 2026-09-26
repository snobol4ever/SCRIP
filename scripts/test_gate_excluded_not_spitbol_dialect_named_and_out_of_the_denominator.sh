#!/usr/bin/env bash
# test_gate_excluded_not_spitbol_dialect_named_and_out_of_the_denominator.sh -- EVERY SNOBOL4 PACKAGE'S EXCLUDED.tsv NAMES ONLY
# SHIPPED PROGRAMS THAT SPITBOL REJECTS, WITH THE CSNOBOL4 FEATURE NAMED, AND THE SUITE ROW CARRIES THE COUNT (ceo CEO-1286).
#
# LON 2026-09-26, in-chat to the ceo, verbatim: "The task is to identify which of the test programs found in the original package
# are in the SPITBOL dialect. We want to exclude from the denominator all that are not SPITBOL dialect. We want to show this number
# of EXCLUDED tests in the test-suite grid." -- and the test: "So the way to tell if a program is SPITBOL dialect is two-fold, 1st
# is rejected by SPITBOL, and 2nd why it is rejected is a CSNOBOL4 feature not supported. Then if these two are true it is NOT
# SPITBOL dialect."
#
# ARMS, per package under corpus/packages/snobol4 (aisnobol csnobol4_suite dotnet gimpel snoflake_suite spitbol_testpgms
# spitbol_x64_tests -- a package with no EXCLUDED.tsv is RED: EXCLUDED=0 is a ruling written down, never an absence):
#   (a) every row is name<TAB>NOT_SPITBOL_DIALECT<TAB>evidence, the evidence 60+ chars naming CSNOBOL4 and sbl -bf
#   (b) every name ships (the file exists) and is not a CONTAINERS.tsv row (a container is already out of shipped)
#   (c) HALF ONE OF THE TEST, MEASURED: every name (or its <name>_driver, gimpel) is in OUTSIDE_SPITBOL_BASELINE.tsv -- the record of
#       what sbl -bf refuses; a program the oracle runs cannot be excluded whatever feature it uses
#   (d) no name twice
#   (e) SPITBOL's own test deck is never excluded (spitbol_testpgms/*.spt, csnobol4_suite/diag1.sno, diag2.sno -- Lon 2026-09-26:
#       "Get ALL 8 working in SCRIP since they are SPITBOL programs.")
# and, over .github/SUITES.tsv:
#   (f) each package's row carries today_excluded, an integer no larger than the sidecar's row count
# FAIL_ONCE=1 plants a dotnet row naming a shipped program the oracle RUNS (chap8_funcs.sno), so arm (c) trips.
# rc 0 = every arm holds; rc 1 = a FAIL named; rc 2 = REFUSED-TO-GRADE (no packages tree, no SUITES.tsv).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
PK="${S4E_CORPUS_ROOT:-$ROOT/corpus}/packages/snobol4"; TSV="${S4E_SUITES_TSV:-$ROOT/.github/SUITES.tsv}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -d "$PK" ] || refuse "no packages tree at $PK"
[ -f "$TSV" ] || refuse "no SUITES.tsv at $TSV"
fails=0; checks=0; ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
rows_of(){ grep -v '^#' "$1/EXCLUDED.tsv" | grep .; [ -n "${FAIL_ONCE:-}" ] && [ "$(basename "$1")" = dotnet ] && printf 'chap8_funcs.sno\tNOT_SPITBOL_DIALECT\tPLANTED by FAIL_ONCE: a program sbl -bf RUNS, so half one of the test fails; csnobol4 runs it too (CSNOBOL4)\n'; return 0; }
echo "=== gate: EXCLUDED.tsv names only shipped programs SPITBOL rejects for a CSNOBOL4 feature, and the row carries the count (CEO-1286) ==="
for spec in aisnobol:aisnobol csnobol4_suite:csnobol4 dotnet:dotnet gimpel:gimpel snoflake_suite:snoflake spitbol_testpgms:testpgms spitbol_x64_tests:x64tests; do
  p="${spec%%:*}"; key="${spec#*:}"; d="$PK/$p"
  if [ ! -f "$d/EXCLUDED.tsv" ]; then ck FAIL "$p: no EXCLUDED.tsv -- EXCLUDED=0 is written down, never absent"; continue; fi
  bad_shape=""; missing=""; container=""; not_outside=""; dup=""; deck=""; n=0
  while IFS=$'\t' read -r name cls ev; do
    [ -n "$name" ] || continue; n=$((n+1))
    { [ "$cls" = NOT_SPITBOL_DIALECT ] && [ "${#ev}" -ge 60 ] && printf '%s' "$ev" | grep -q 'CSNOBOL4' && printf '%s' "$ev" | grep -q 'sbl -bf'; } || bad_shape="$bad_shape $name"
    [ -f "$d/$name" ] || missing="$missing $name"
    [ -f "$d/CONTAINERS.tsv" ] && awk -F'\t' -v n="$name" '$1==n{f=1} END{exit f?0:1}' "$d/CONTAINERS.tsv" && container="$container $name"
    stem="${name%.*}"
    if [ -f "$d/OUTSIDE_SPITBOL_BASELINE.tsv" ]; then
      awk -F'\t' -v a="$name" -v b="${stem}_driver.${name##*.}" '$0 !~ /^#/ && ($1==a || $1==b){f=1} END{exit f?0:1}' "$d/OUTSIDE_SPITBOL_BASELINE.tsv" || not_outside="$not_outside $name"
    else not_outside="$not_outside $name(no OUTSIDE_SPITBOL_BASELINE.tsv)"; fi
    case "$p/$name" in spitbol_testpgms/*|csnobol4_suite/diag1.sno|csnobol4_suite/diag2.sno) deck="$deck $name" ;; esac
  done < <(rows_of "$d")
  dup="$(rows_of "$d" | cut -f1 | sort | uniq -d | tr '\n' ' ')"
  [ -z "$bad_shape" ] && ck ok "(a) $p: $n row(s), each NOT_SPITBOL_DIALECT with evidence naming CSNOBOL4 and sbl -bf" || ck FAIL "(a) $p: malformed row(s):$bad_shape"
  [ -z "$missing$container" ] && ck ok "(b) $p: every name ships and none is a container" || ck FAIL "(b) $p: not shipped:$missing container:$container"
  [ -z "$not_outside" ] && ck ok "(c) $p: every excluded program is in OUTSIDE_SPITBOL_BASELINE.tsv (rejected by SPITBOL, half one)" || ck FAIL "(c) $p: the oracle does not refuse these, so they cannot be excluded:$not_outside"
  [ -z "$dup" ] && ck ok "(d) $p: no name twice" || ck FAIL "(d) $p: named twice: $dup"
  [ -z "$deck" ] && ck ok "(e) $p: SPITBOL's own test deck is not excluded" || ck FAIL "(e) $p: SPITBOL's own programs excluded:$deck"
  row="$(awk -F'\t' -v k="$key" '$1==k' "$TSV")"; hdr="$(grep -v '^#' "$TSV" | head -1)"
  col="$(printf '%s\n' "$hdr" | tr '\t' '\n' | grep -n '^today_excluded$' | cut -d: -f1)"
  if [ -z "$col" ]; then ck FAIL "(f) $key: SUITES.tsv has no today_excluded column"
  else
    te="$(printf '%s\n' "$row" | cut -f"$col")"
    case "$te" in ''|*[!0-9]*) ck FAIL "(f) $key: today_excluded reads [$te], not an integer" ;;
      *) [ "$te" -le "$n" ] && ck ok "(f) $key: today_excluded=$te <= $n named in the sidecar" || ck FAIL "(f) $key: today_excluded=$te exceeds the $n named in the sidecar" ;; esac
  fi
done
echo "checks=$checks fails=$fails"
[ "$fails" -eq 0 ]
