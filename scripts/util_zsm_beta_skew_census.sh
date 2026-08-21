#!/usr/bin/env bash
# util_zsm_beta_skew_census.sh — THE β-SKEW EXCEPTIONS LIST (s196, Lon P2 directive).
#
# Lon (in-chat s196): "maybe a report at BETA that RSP is different than at ALPHA since most
# boxes require that.  ARBNO is an exception, and I'm curious to have a list of all the
# exceptions."  This census MEASURES that list instead of asserting it: it runs a witness set
# under the full ZSM (SCRIP_ZSM=1 SCRIP_ZSM_ALL=1 SCRIP_ZSM_BSKEW=1), collects every
# [ZSM-BSKEW] line (a β arrival whose rsp differs from the same node's α rsp), and aggregates
# per IR op: which box kinds legitimately resume at a different depth than they were entered.
#
#   bash scripts/util_zsm_beta_skew_census.sh [file.sno ...]     # default: ptc grid + retry family
#
# Read the table as CANDIDATES: a kind appearing here is either a documented exception
# (ARBNO-family per-iteration frames, DEFER blob records) or a defect — the per-op rollup is
# what makes the difference visible.  A program whose VERDICT changes under the instrument
# must not be quoted (s184 proved ZSM-ALL transparent; re-prove after instrument edits).
set -uo pipefail
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
SCRIP="$S4E/SCRIP/scrip"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
FILES=("$@")
if [ ${#FILES[@]} -eq 0 ]; then
    FILES=( "$S4E"/corpus/probe/passthru/ptc*.sno "$S4E"/corpus/probe/retry/*.sno )
fi
n=0
for f in "${FILES[@]}"; do
    [ -f "$f" ] || continue
    n=$((n+1))
    SCRIP_ZSM=1 SCRIP_ZSM_ALL=1 SCRIP_ZSM_BSKEW=1 timeout 8 "$SCRIP" "$f" </dev/null 2>>"$TMP/ev" >/dev/null || true
done
echo "== β-SKEW EXCEPTIONS CENSUS over $n programs =="
echo "   (β arrivals whose rsp differs from their own α rsp; most boxes require equality — Lon s196)"
grep -h 'ZSM-BSKEW' "$TMP/ev" 2>/dev/null \
  | sed -E 's/.*op=([0-9]+)\(([A-Za-z_0-9]+)\).*differs from α by (-?[0-9]+) bytes.*/\2 \3/' \
  | awk '{ n[$1]++; if (!( $1 in mn) || $2 < mn[$1]) mn[$1] = $2; if (!($1 in mx) || $2 > mx[$1]) mx[$1] = $2; if ($2 != last[$1]) { d[$1]++; last[$1] = $2 } }
         END { printf "%-28s %8s %10s %10s\n", "op", "events", "min-skew", "max-skew";
               for (k in n) printf "%-28s %8d %10d %10d\n", k, n[k], mn[k], mx[k] }' \
  | sort -k2 -rn
echo "== programs contributing: =="
grep -l 'ZSM-BSKEW' "$TMP/ev" >/dev/null 2>&1 || true
grep -c 'ZSM-BSKEW' "$TMP/ev" 2>/dev/null | sed 's/^/total β-skew events: /'
