#!/usr/bin/env bash
# test_conformance_spitbol_manual.sh -- row `conformance-sweep-spitbol-manual`.
#
# Re-runs every witness under corpus/probe/conformance/ (minted by the SPITBOL-manual conformance sweep --
# each witness is sourced from the manual's OWN worked example, v3.7) through test_one_witness.sh (m3 + m4
# vs the live x64 sbl oracle, -bf) and reports a compact PASS/DIFF table plus a summary line. A DIFF here
# reproduces an already-filed conformance-sweep queue row (see corpus/probe/conformance/*.sno headers /
# the sweep's FINDING for which row owns which witness) -- this script is a fast re-check, not a triage
# tool: it does not classify NEW divergences, it only tells you whether a KNOWN one is still present.
#
# usage: bash scripts/test_conformance_spitbol_manual.sh [--quiet]
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
D="$ROOT/../corpus/probe/conformance"
QUIET=0; [ "${1:-}" = "--quiet" ] && QUIET=1
[ -d "$D" ] || { echo "UNPROVEN: no $D -- has the conformance sweep ever minted a witness?"; exit 2; }
total=0; pass=0; diverge=0
for f in "$D"/*.sno; do
  [ -f "$f" ] || continue
  total=$((total+1))
  b="$(basename "$f" .sno)"
  line="$(bash "$ROOT/scripts/test_one_witness.sh" "$f" "conf_$b" 2>&1)"
  if printf '%s' "$line" | grep -q 'm3=PASS *m4=PASS'; then
    pass=$((pass+1))
    [ "$QUIET" = 1 ] || printf 'AGREE    %s\n' "$b"
  else
    diverge=$((diverge+1))
    printf 'DIVERGE  %s :: %s\n' "$b" "$line"
  fi
done
printf 'conformance-sweep-spitbol-manual: %d witnesses, %d agree, %d diverge\n' "$total" "$pass" "$diverge"
[ "$diverge" -eq 0 ]
