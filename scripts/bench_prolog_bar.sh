#!/usr/bin/env bash
export SCRIP_DIAG=0   # benchmarks run with every diagnostic off: the collector's self-checks and the node-id stores (Lon 2026-09-25, in-chat to the ceo: "For benchmarks turn off all diagnostic code."; ceo CEO-1262)
# bench_prolog_bar.sh -- THE PROLOG SPEED ROW'S CRITERION (ceo CEO-1278, 2026-09-26; the Prolog twin of bench_snobol4_bar.sh and
# bench_icon_bar.sh): ONE kernel under corpus/benchmarks/prolog/bench, the bare multiple rival time / SCRIP mode-4 time (RULES.md
# FACT RULE, CEO-1242) against a bar, read from angle 1 of the Prolog three-angle harness (test_bench_prolog_timed.sh: a live doubling
# search for the largest N whose between+fail loop completes within BAR_BUD_MS (500), rate = iterations / CPU; the kernel's single-shot
# output is held to its .ref on every engine before it is timed) for the one kernel, in ONE run -- no board, no row written.
#   kernel K BAR [swi|gnu]   K is the kernel's file stem (tak, deriv, nreverse, ...); the rival is swipl (default) or gprolog
#                            (gprolog --consult-file). A kernel whose arm reads SKIP, CRASH or no rate REFUSES: no multiple over an
#                            uncited reading. BAR_MODE=m3 grades mode 3 instead of mode 4.
# EXIT 0 at or above the bar (GREEN), 1 below it (RED, the row is open), 2 REFUSED (no binary, no rival, no citable reading).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
refuse() { echo "⛔ bench_prolog_bar REFUSE(2): $*"; exit 2; }
[ $# -ge 3 ] && [ "$1" = kernel ] || { echo "usage: $0 kernel <stem> <bar> [swi|gnu]"; exit 2; }
NAME=$2; BAR=$3; RIVAL="${4:-swi}"; MODE="${BAR_MODE:-m4}"
case "$RIVAL" in swi|gnu) ;; *) refuse "rival must be swi or gnu";; esac
[ -x "$ROOT/scrip" ] || refuse "no ./scrip (make first)"; command -v swipl > /dev/null || refuse "swipl absent"; command -v gprolog > /dev/null || refuse "gprolog absent"
T=$(mktemp -d) || refuse "mktemp"; trap 'rm -rf "$T"' EXIT
( cd "$ROOT" && KERNELS="$NAME" TIME_BUDGET_MS="${BAR_BUD_MS:-500}" bash scripts/test_bench_prolog_timed.sh ) > "$T/log" 2>&1
row=$(grep -E "^$NAME[[:space:]]" "$T/log" | head -1); [ -n "$row" ] || { tail -4 "$T/log" | cut -c1-160; refuse "angle 1 printed no row for $NAME"; }   # the FIRST row is the rates table; the N and WORK tables that follow share its row shape
read -r _k gnu swi m3 m4 check <<<"$row"
case "$RIVAL" in swi) rv=$swi;; gnu) rv=$gnu;; esac; case "$MODE" in m3) mine=$m3;; *) mine=$m4;; esac
for v in "$rv" "$mine"; do case "$v" in ''|SKIP|CRASH*|DNF*|-|NA|*[!0-9.]*) echo "$row" | cut -c1-160; refuse "$NAME has no citable rate on $RIVAL and $MODE (a SKIP, a CRASH or no rate: $rv / $mine)";; esac; done
awk -v r="$rv" -v m="$mine" -v bar="$BAR" -v k="$NAME" -v rival="$RIVAL" -v mode="$MODE" -v chk="$check" 'BEGIN{x=m/r; printf "kernel %s: %s %.1f iterations/s (%.1f us/it), %s %.1f iterations/s (%.1f us/it), angle 1 %s; reads %.2fx %s in mode %s (bar %.2fx): %s\n", k, (rival=="swi")?"swipl":"gprolog", r, 1e6/r, mode, m, 1e6/m, chk, x, (rival=="swi")?"swipl":"gprolog", substr(mode,2), bar, (x>=bar)?"GREEN":"RED"; exit (x>=bar)?0:1}'
