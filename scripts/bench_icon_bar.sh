#!/usr/bin/env bash
export SCRIP_DIAG=0   # benchmarks run with every diagnostic off: the collector's self-checks and the node-id stores (Lon 2026-09-25, in-chat to the ceo: "For benchmarks turn off all diagnostic code."; ceo CEO-1262)
# bench_icon_bar.sh -- THE ICON SPEED ROW'S CRITERION (ceo CEO-1276, 2026-09-26; the Icon twin of bench_snobol4_bar.sh): ONE kernel under
# corpus/benchmarks/icon, the bare multiple Arizona iconx time / SCRIP mode-4 time (RULES.md FACT RULE, CEO-1242) against a bar, read from
# the three-angle harness bench_triangulate_icon.sh (the fixed-time twin's microseconds per iteration, BAR_BUD_MS (500) per point,
# BAR_N (5) iterations per fixed-iteration point) in ONE run with --out to a mktemp TSV -- no board, no row written.
#   kernel K BAR       K is the kernel's file stem (bench_icnagg_field_isolate, concord, geddump, ...). A kernel whose two timed angles
#                      DISAGREE, or that does not PASS every angle in every engine, REFUSES: no multiple over an uncited reading.
# EXIT 0 at or above the bar (GREEN), 1 below it (RED, the row is open), 2 REFUSED (no binary, no oracle, no citable reading).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
refuse() { echo "⛔ bench_icon_bar REFUSE(2): $*"; exit 2; }
[ $# -ge 3 ] && [ "$1" = kernel ] || { echo "usage: $0 kernel <stem> <bar>"; exit 2; }
NAME=$2; BAR=$3; [ -x "$ROOT/scrip" ] || refuse "no ./scrip (make first)"
T=$(mktemp -d) || refuse "mktemp"; trap 'rm -rf "$T"' EXIT
( cd "$ROOT" && BUDGET_MS="${BAR_BUD_MS:-500}" BENCH_N_DEFAULT="${BAR_N:-5}" bash scripts/bench_triangulate_icon.sh --out "$T/t.tsv" "$NAME" ) > "$T/log" 2>&1
[ -s "$T/t.tsv" ] || { tail -5 "$T/log"; refuse "the harness wrote no TSV for $NAME"; }
read -r ix m4 <<<"$(awk -F'\t' -v k="$NAME" 'NR>1 && $1==k && $2=="iconx" && $13=="PASS" && $12 ~ /^AGREE/ {ix=$9*1000/$8} NR>1 && $1==k && $2=="m4" && $13=="PASS" && $12 ~ /^AGREE/ {m4=$9*1000/$8} END{print ix+0, m4+0}' "$T/t.tsv")"
[ "$ix" != 0 ] && [ "$m4" != 0 ] || { grep -E "^$NAME" "$T/log" | cut -c1-160; refuse "$NAME has no citable reading on iconx and m4 (a DISAGREE or a non-PASS angle; raise BAR_N for a sub-millisecond kernel)"; }
awk -v ix="$ix" -v m4="$m4" -v bar="$BAR" -v k="$NAME" 'BEGIN{x=ix/m4; printf "kernel %s: iconx %.1f us/it, m4 %.1f us/it (fixed-time twins); reads %.2fx iconx in mode 4 (bar %.2fx): %s\n", k, ix, m4, x, bar, (x>=bar)?"GREEN":"RED"; exit (x>=bar)?0:1}'
