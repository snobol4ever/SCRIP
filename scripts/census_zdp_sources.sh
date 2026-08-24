#!/usr/bin/env bash
# census_zdp_sources.sh — corpus-wide ZDP TOP-source histogram + tier totals.  Re-run at every zeta-traversal handoff; the ACTIVATION percentage must be explainable, never merely low.
# ZDP corpus sweep: TOP-source op histogram + tier totals.  Reads compile STDERR only; never touches committed .s artifacts.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
OUT=${1:-/tmp/zdp_sweep_out}
mkdir -p "$OUT"; : > "$OUT/sources.txt"; : > "$OUT/tiers.txt"
n=0
for f in $(find $S4E/corpus/tests/snobol4 $S4E/corpus/probe -name '*.sno' | sort); do
    n=$((n+1))
    SCRIP_ZDP=2 timeout 15 ./scrip --compile "$f" < /dev/null 2>"$OUT/.err" >/dev/null
    grep "^\[ZDP\]   node" "$OUT/.err" | sed 's/.*op=\([A-Za-z0-9_]*\) \([A-Z-]*\) tier=\([A-Z]*\).*/\1 \2 \3/' >> "$OUT/sources.txt"
    grep "^\[ZDP\] graph" "$OUT/.err" | sed 's/.*TIER spine=\([0-9]*\) activation=\([0-9]*\) standing=\([0-9]*\)/\1 \2 \3/' >> "$OUT/tiers.txt"
done
echo "programs swept: $n"
echo "=== TOP-SOURCE op x class histogram ==="
sort "$OUT/sources.txt" | uniq -c | sort -rn
echo "=== TIER totals (spine activation standing) ==="
awk '{s+=$1; a+=$2; t+=$3} END {printf "spine=%d activation=%d standing=%d  total=%d  activation_pct=%.2f\n", s,a,t,s+a+t,100.0*a/(s+a+t)}' "$OUT/tiers.txt"
