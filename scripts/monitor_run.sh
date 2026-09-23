#!/usr/bin/env bash
# monitor_run.sh -- THE ONE COMMAND AN HQ USES THE IPC SYNC-STEP MONITOR THROUGH (ceo 2026-09-23, CEO-1186; Lon: "finish up IPC
# sync-step monitor and let me know when the HQ's can begin to use"). Design: MONITOR-BINARY-DESIGN.md § THE PLUG INTERFACE.
#
#   bash scripts/monitor_run.sh <source>            # --modes: SCRIP mode 3 against SCRIP mode 4 in lock-step (every language)
#   bash scripts/monitor_run.sh <source> --trace    # print the trace (statement / assignment / call / return) of a mode-3 run
#   bash scripts/monitor_run.sh <source> --oracle   # SCRIP against the language's oracle in lock-step (SNOBOL4 only today: SPITBOL)
#   [--input FILE] feeds stdin (default: <base>.input beside the source if present, else /dev/null)
#
# Every mode-3/mode-4 run is preceded by the MONITOR-SAFE CHECK (RULES.md: a monitor verdict is a verdict on a different program):
# the untraced run's stdout must equal the traced run's stdout with the trace lines (****...) removed, else this REFUSES rc=2 --
# the trace changed the program and no verdict from it is about the program you meant. Exit: 0 modes agree / trace printed,
# 1 the modes DIVERGE (the controller's grid names the step), 2 could not measure (a participant never started, the source
# does not compile, the witness is not monitor-safe, or no oracle bridge exists for the language).
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"; SD="$S4E/SCRIP"; SCRIP="$SD/scrip"
src="${1:-}"; [ -n "$src" ] && [ -f "$src" ] || { echo "REFUSE(2): usage: monitor_run.sh <source> [--modes|--trace|--oracle] [--input FILE]"; exit 2; }
shift; mode=modes; input=""
while [ $# -gt 0 ]; do case "$1" in --modes) mode=modes;; --trace) mode=trace;; --oracle) mode=oracle;; --input) input="$2"; shift;; *) echo "REFUSE(2): unknown option $1"; exit 2;; esac; shift; done
[ -x "$SCRIP" ] || { echo "REFUSE(2): no $SCRIP -- run make"; exit 2; }
src="$(realpath "$src")"; base="${src%.*}"; ext="${src##*.}"
[ -z "$input" ] && [ -f "$base.input" ] && input="$base.input"; [ -z "$input" ] && input=/dev/null
case "$ext" in sno|icn|pl|pas|raku|sc|reb) ;; *) echo "REFUSE(2): $ext is not a SCRIP source extension"; exit 2;; esac
if [ "$mode" = trace ]; then
    timeout 60 "$SCRIP" --trace --run "$src" < "$input"; rc=$?
    [ "$rc" = 124 ] && { echo "REFUSE(2): the traced run did not finish in 60 s -- this is not a verdict"; exit 2; }
    exit 0
fi
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
timeout 60 "$SCRIP" --run "$src" < "$input" > "$W/plain.out" 2> "$W/plain.err"; prc=$?
timeout 60 "$SCRIP" --trace --run "$src" < "$input" > "$W/traced.out" 2> "$W/traced.err"; trc=$?
if [ "$prc" = 124 ] || [ "$trc" = 124 ]; then echo "REFUSE(2): the witness did not finish in 60 s (plain rc=$prc, traced rc=$trc) -- pick a shorter witness"; exit 2; fi
perl -0pe 's/\*\*\*\*[0-9]+[^\n]*\n//g' "$W/traced.out" > "$W/traced.stripped"
if ! cmp -s "$W/plain.out" "$W/traced.stripped" || [ "$prc" != "$trc" ]; then
    echo "REFUSE(2): NOT MONITOR-SAFE -- the traced run's program output differs from the untraced run's (plain rc=$prc, traced rc=$trc); a verdict under trace would be about a different program. First difference:"
    diff "$W/plain.out" "$W/traced.stripped" | head -6; exit 2
fi
nev=$(grep -oE '\*\*\*\*[0-9]+' "$W/traced.out" | wc -l); echo "[monitor_run] monitor-safe: untraced output identical under trace; $nev trace event(s) in mode 3"
if [ "$mode" = oracle ]; then
    case "$ext" in
        sno) parts="spl scr" ;;
        raku) parts="rko scr" ;;
        *) echo "REFUSE(2): no oracle bridge for .$ext yet -- the design (MONITOR-BINARY-DESIGN.md § THE PLUG INTERFACE, layer 6) adds one only where it earns its cost; use --modes (mode 3 against mode 4) or --trace against the oracle's own output by hand"; exit 2 ;;
    esac
else
    parts="scr3 scr4"
fi
[ "$input" = /dev/null ] || export MONITOR_STDIN="$input"
PARTICIPANTS="$parts" timeout 300 bash "$SD/scripts/test_monitor_3way_sync_step_auto.sh" "$src" > "$W/harness.out" 2>&1; hrc=$?
steps=$(grep -oE 'all reached END after [0-9]+ steps' "$W/harness.out" | grep -oE '[0-9]+' | head -1)
if [ "$hrc" = 2 ] || [ "$hrc" = 124 ]; then echo "REFUSE(2): the harness could not measure (rc=$hrc): $(grep -E 'REFUS|FAIL' "$W/harness.out" | head -1 | cut -c1-140)"; exit 2; fi
if [ "$hrc" = 0 ] && [ -n "$steps" ] && [ "$steps" -gt 0 ]; then echo "[monitor_run] AGREE: participants $parts agree event-for-event, clean termination at step $steps"; exit 0; fi
if [ "$hrc" = 0 ]; then echo "REFUSE(2): the controller printed no lock-step termination line -- an empty agreement is not a pass"; exit 2; fi
echo "[monitor_run] DIVERGE (rc=$hrc): participants $parts -- the controller's grid, last-agree trail then the first divergence:"; sed -n '/controller output/,/stdout (head)/p' "$W/harness.out" | grep -vE 'controller output|stdout \(head\)|^\s*$' | head -40
exit 1
