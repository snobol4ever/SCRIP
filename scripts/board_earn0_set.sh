#!/usr/bin/env bash
# board_earn0_set.sh — LOWER seat (GOAL-SN4-HOME-LOWER L-0) acceptance board.
# Runs every corpus/probe/earn0 witness in m3 (and optionally m4), diffs vs the
# oracle-baked .ref, and prints a BY-SET table. Judge BY SET, never by count.
# Usage: bash scripts/board_earn0_set.sh [m3|m4|both]
set -u
SCRIP_DIR="${SCRIP_DIR:-/home/claude/SCRIP}"
EARN0="${EARN0:-/home/claude/corpus/probe/earn0}"
MODE="${1:-m3}"
TMO="${TMO:-8}"
one_shot() {
  local f="$1" mode="$2" ref="$3" out rc
  if [ "$mode" = m3 ]; then
    out="$(cd /tmp && timeout "${TMO}s" "$SCRIP_DIR/scrip" --run "$f" 2>/dev/null < /dev/null)"; rc=$?
  else
    out="$(cd /tmp && timeout "${TMO}s" "$SCRIP_DIR/scrip" --compile "$f" 2>/dev/null < /dev/null)"; rc=$?
  fi
  if   [ ! -f "$ref" ];   then echo "NO-REF"
  elif [ "$rc" -eq 124 ]; then echo "FAIL-hang"
  elif [ "$rc" -eq 139 ]; then echo "FAIL-segv"
  elif [ "$rc" -eq 134 ]; then echo "FAIL-abort"
  elif [ "$out" = "$(cat "$ref")" ]; then echo "PASS"
  else echo "FAIL-silent"; fi
}
# REPEAT>1: a row whose verdict is not constant across runs is FLAKY, and FLAKY is
# reported as such — never as whichever arm came up first. earn0_stored_capture is
# the known member (measured 3xPASS-shape/5xABORT at 52545cbf); a single-shot board
# reports it as a coin flip and silently misleads the next seat.
run_one() {
  local f="$1" mode="$2" base ref v first flaky=0 i
  base="$(basename "$f" .sno)"; ref="$EARN0/$base.ref"
  first="$(one_shot "$f" "$mode" "$ref")"
  for i in $(seq 2 "${REPEAT:-1}"); do
    v="$(one_shot "$f" "$mode" "$ref")"; [ "$v" = "$first" ] || flaky=1
  done
  [ "$flaky" -eq 1 ] && first="FLAKY(${first}/…)"
  printf '%-38s %-4s %s\n' "$base" "$mode" "$first"
}
for m in $( [ "$MODE" = both ] && echo "m3 m4" || echo "$MODE" ); do
  echo "=== earn0 board · mode $m · SCRIP $(cd "$SCRIP_DIR" && git rev-parse --short HEAD) · corpus $(cd "$EARN0" && git rev-parse --short HEAD) ==="
  for f in "$EARN0"/*.sno; do run_one "$f" "$m"; done
done
