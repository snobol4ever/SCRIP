#!/usr/bin/env bash
# board_passthru_combo.sh — THE PASS-THRU COMBINATORIAL BOARD (ARCH-PASSTHRU witness plan, s177 PT-COMBO + s179 class 6-9 fills).
# Runs EVERY corpus/tests/snobol4/probe/passthru.sno suite witness (pt*/ptc*/ptx*/ptw*/cset32_all5) in BOTH modes
# (m3 --run; m4 --compile -> gcc -no-pie -> run), diffs vs the oracle-baked .ref beside each, prints per-row
# verdicts + a per-class rollup. A red row is never denied (law 0d): minimize to a reproducer, stop the world,
# show Lon. Usage: bash scripts/board_passthru_combo.sh [m3|m4|both] [name-filter]
# ⭐ RE-POINTED 2026-08-28 (seat08, probe-consolidate-passthru / corpus-crosscheck-probe-total-conversion, Lon's
# total-conversion ruling): the 183 witnesses that used to live as loose corpus/probe/passthru/*.sno files now
# live in corpus/tests/snobol4/probe/passthru.{sno,ref} (the corpus-suites-consolidation format) -- this script
# is JUST ANOTHER CONSUMER of that shared file now, the same relationship test_gate_udc.sh already has with
# tests/snobol4/tests/snobol4/probe_loose/cn.{sno,ref}, not a second copy of the witness text. one_shot() and the classifier/rollup
# below are UNCHANGED from before the conversion; only the SOURCE of (name, .sno, .ref) triples changed, from a
# directory glob to entries extracted from the suite via corpus_suite_harness.py's `list`+`extract` (the ONE
# parsing authority -- reused, never re-implemented in bash, per this file's own established precedent).
# ⭐ 5 witnesses (ptw_min_arbno_alt_fence_L1, ptw_min_defer2_hang, ptw_min_rseal_arbno, ptw_min_rseal_commands,
# ptw_min_rseal_unsealed_ctl) are pre-existing, documented reds (XFAIL in the suite -- see corpus_suite_harness.py's
# module docstring): extraction strips the XFAIL marker back to a plain standalone .sno/.ref, so they print here
# exactly as red as they always did -- this board's whole point is surfacing them, never suppressing them, unlike
# test_corpus_snobol4.sh's aggregate gate which buckets them separately so they don't register as a regression.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root; S4E_HOME overrides
set -u
SCRIP_DIR="${SCRIP_DIR:-$S4E/SCRIP}"
HARNESS="${HARNESS:-$SCRIP_DIR/scripts/corpus_suite_harness.py}"
SUITE_SNO="${SUITE_SNO:-$S4E/corpus/tests/snobol4/probe/passthru.sno}"
SUITE_REF="${SUITE_REF:-$S4E/corpus/tests/snobol4/probe/passthru.ref}"
MODE="${1:-both}"
FILT="${2:-}"
TMO="${TMO:-8}"
if [ ! -f "$SUITE_SNO" ] || [ ! -f "$SUITE_REF" ]; then echo "⛔ GATE REFUSES: suite file missing ($SUITE_SNO)"; exit 2; fi
if ! command -v python3 >/dev/null 2>&1; then echo "⛔ GATE REFUSES: python3 not found"; exit 2; fi
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
mapfile -t NAMES < <(python3 "$HARNESS" list "$SUITE_SNO" "$SUITE_REF") || { echo "⛔ GATE REFUSES: could not list suite entries ($SUITE_SNO)"; exit 2; }
[ "${#NAMES[@]}" -eq 0 ] && { echo "⛔ GATE REFUSES: suite file has zero entries ($SUITE_SNO)"; exit 2; }
EX="$W/extracted"; mkdir -p "$EX"
extract_fail=0
for base in "${NAMES[@]}"; do
  [ -n "$FILT" ] && case "$base" in *"$FILT"*) ;; *) continue;; esac
  python3 "$HARNESS" extract "$SUITE_SNO" "$SUITE_REF" "$base" "$EX/$base.sno" --out-ref "$EX/$base.ref" >/dev/null 2>&1 \
    || { echo "⚠️  extract FAILED for $base" >&2; extract_fail=$((extract_fail+1)); }
done
[ "$extract_fail" -gt 0 ] && echo "⚠️  $extract_fail/${#NAMES[@]} entries failed to extract -- board below is incomplete" >&2
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
  echo "=== passthru combo board · mode $m · SCRIP $(cd "$SCRIP_DIR" && git rev-parse --short HEAD) · corpus $(cd "$S4E/corpus" && git rev-parse --short HEAD) ==="
  declare -A CP=() CT=()
  pass=0; tot=0; reds=""
  for f in "$EX"/*.sno; do
    base="$(basename "$f" .sno)"
    cls="$(echo "$base" | sed -E 's/^(ptc[0-9]+[fb]|pt[0-9]+|ptx|ptw).*/\1/')"
    v="$(one_shot "$f" "$m" "$EX/$base.ref")"
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
