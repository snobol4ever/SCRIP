#!/usr/bin/env bash
# board_passthru_combo.sh — THE PASS-THRU COMBINATORIAL BOARD (ARCH-PASSTHRU witness plan, s177 PT-COMBO + s179 class 6-9 fills).
# Runs EVERY corpus/probe/passthru witness (pt*/ptc*/ptx*/ptw*) in BOTH modes (m3 --run; m4 --compile -> gcc -no-pie -> run),
# diffs vs the oracle-baked .ref beside each, prints per-row verdicts + a per-class rollup. A red row is never denied (law 0d):
# minimize to a reproducer, stop the world, show Lon. Usage: bash scripts/board_passthru_combo.sh [m3|m4|both] [name-filter]
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root; S4E_HOME overrides
set -u
SCRIP_DIR="${SCRIP_DIR:-$S4E/SCRIP}"
PT="${PT:-$S4E/corpus/probe/passthru}"
MODE="${1:-both}"
FILT="${2:-}"
TMO="${TMO:-8}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
one_shot() {
  local f="$1" mode="$2" ref="$3" out rc
  if [ "$mode" = m3 ]; then
    out="$(cd "$W" && timeout "${TMO}s" "$SCRIP_DIR/scrip" --run "$f" 2>/dev/null < /dev/null)"; rc=$?
  else
    if ! (cd "$W" && timeout 60 "$SCRIP_DIR/scrip" --compile "$f" </dev/null > "$W/p.s" 2>/dev/null) || [ ! -s "$W/p.s" ]; then echo "FAIL-compile"; return; fi
    if ! gcc -no-pie "$W/p.s" -L"$SCRIP_DIR/out" -lscrip_rt -lm -Wl,-rpath,"$SCRIP_DIR/out" -o "$W/p.bin" 2>/dev/null; then echo "FAIL-asm"; return; fi
    out="$(cd "$W" && timeout "${TMO}s" "$W/p.bin" 2>/dev/null < /dev/null)"; rc=$?
  fi
  if   [ ! -f "$ref" ];   then echo "NO-REF"
  elif [ "$rc" -eq 124 ]; then echo "FAIL-hang"
  elif [ "$rc" -eq 139 ]; then echo "FAIL-segv"
  elif [ "$rc" -eq 134 ]; then echo "FAIL-abort"
  elif [ "$out" = "$(cat "$ref")" ]; then echo "PASS"
  else echo "FAIL-diff(rc=$rc)"; fi
}
for m in $( [ "$MODE" = both ] && echo "m3 m4" || echo "$MODE" ); do
  echo "=== passthru combo board · mode $m · SCRIP $(cd "$SCRIP_DIR" && git rev-parse --short HEAD) · corpus $(cd "$PT" && git rev-parse --short HEAD) ==="
  declare -A CP=() CT=()
  pass=0; tot=0; reds=""
  for f in "$PT"/pt*.sno; do
    base="$(basename "$f" .sno)"
    [ -n "$FILT" ] && case "$base" in *"$FILT"*) ;; *) continue;; esac
    cls="$(echo "$base" | sed -E 's/^(ptc[0-9]+[fb]|pt[0-9]+|ptx|ptw).*/\1/')"
    v="$(one_shot "$f" "$m" "$PT/$base.ref")"
    tot=$((tot+1)); CT[$cls]=$(( ${CT[$cls]:-0} + 1 ))
    if [ "$v" = PASS ]; then pass=$((pass+1)); CP[$cls]=$(( ${CP[$cls]:-0} + 1 )); else reds="$reds $base:$v"; fi
    printf '%-32s %-4s %s\n' "$base" "$m" "$v"
  done
  echo "--- $m rollup by class:"
  for c in $(echo "${!CT[@]}" | tr ' ' '\n' | sort); do printf '  %-8s %d/%d\n' "$c" "${CP[$c]:-0}" "${CT[$c]}"; done
  echo "--- $m TOTAL $pass/$tot"
  [ -n "$reds" ] && echo "--- $m REDS:$reds"
  unset CP CT
done
