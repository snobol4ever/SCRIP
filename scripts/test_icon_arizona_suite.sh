#!/usr/bin/env bash
# test_icon_arizona_suite.sh — grade SCRIP m3 (--run) + m4 (--compile+link) against the vendored
# official Arizona Icon test suite (corpus/packages/icon/arizona_tests/general, upstream 9.5).
#
# GROUND TRUTH FOR THE CONTRACT: upstream's own general/Test-icon — compile NAME.icn, feed NAME.dat
# as stdin if present else /dev/null, capture stdout+stderr combined, diff against NAME.std. This
# script reproduces that exactly for SCRIP's two native execution modes instead of icont/iconx.
#
# THREE OUTCOMES, NEVER CONFLATED (SCRIP Icon requires an explicit ';' between bare statements and
# does zero newline processing — RULES.md FACT RULE, test_gate_icn_semicolon_required.sh — so these
# unmodified upstream standard-Icon-dialect programs are expected to hit this in large numbers):
#   PASS   — parses, runs, output byte-identical to .std.
#   REJECT — fails to *parse* at all ("parse error" from SCRIP, always on stderr). Named per-file,
#            counted separately, never silently folded into FAIL.
#   FAIL   — parses and runs, output does not match .std. The genuine correctness signal.
#
# Usage: bash scripts/test_icon_arizona_suite.sh [-v]   (-v prints every REJECT/FAIL name inline)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_flag_gate.sh unloadable" >&2; exit 2; }
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_SO="$HERE/../out/libscrip_rt.so"
CORPUS="$S4E/corpus"
SUITE="$CORPUS/packages/icon/arizona_tests/general"
TIMEOUT="${TIMEOUT:-8}"
VERBOSE=0
if [ $# -gt 0 ]; then
  case "$1" in
    -v) VERBOSE=1 ;;
    *) flaggate_reject "$1" "-v" ;;
  esac
fi

# ⛔ A MISSING PREREQUISITE IS A REFUSAL (rc=2), NEVER A GREEN EXIT (RULES.md FACT RULE).
if [ ! -d "$SUITE" ]; then
  echo "⛔ GATE REFUSES: corpus subtree missing: $SUITE" >&2
  echo "   vendor /home/resources/icon-master/tests/general into it first." >&2
  exit 2
fi
if [ ! -x "$SCRIP" ]; then
  echo "⛔ GATE REFUSES: scrip not built at $SCRIP" >&2
  exit 2
fi

TOTAL=0
M3_PASS=0; M3_REJECT=0; M3_FAIL=0
M4_PASS=0; M4_REJECT=0; M4_FAIL=0
M3_REJECT_NAMES=""; M3_FAIL_NAMES=""
M4_REJECT_NAMES=""; M4_FAIL_NAMES=""

for std in "$SUITE"/*.std; do
  [ -f "$std" ] || continue
  name=$(basename "$std" .std)
  icn="$SUITE/$name.icn"
  [ -f "$icn" ] || continue
  TOTAL=$((TOTAL+1))
  exp=$(cat "$std")
  dat="$SUITE/$name.dat"
  stdin_file="/dev/null"
  [ -f "$dat" ] && stdin_file="$dat"

  # ── mode 3: --run ──────────────────────────────────────────────────────────────────────────────
  m3out=$(timeout "$TIMEOUT" "$SCRIP" --run "$icn" < "$stdin_file" 2>&1)
  if printf '%s' "$m3out" | grep -q 'parse error'; then
    M3_REJECT=$((M3_REJECT+1)); M3_REJECT_NAMES="$M3_REJECT_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m3 REJECT] $name"
  elif [ "$m3out" = "$exp" ]; then
    M3_PASS=$((M3_PASS+1))
  else
    M3_FAIL=$((M3_FAIL+1)); M3_FAIL_NAMES="$M3_FAIL_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m3 FAIL] $name"
  fi

  # ── mode 4: --compile (asm to stdout) -> assemble+link libscrip_rt.so -> run ─────────────────────
  s4=$(mktemp /tmp/ariz_XXXXXX.s); bin4=$(mktemp /tmp/ariz_XXXXXX.bin); rm -f "$bin4"
  m4diag=$(timeout "$TIMEOUT" "$SCRIP" --compile "$icn" 2>&1 >"$s4" </dev/null)
  m4out=""
  if [ -s "$s4" ] && [ -f "$RT_SO" ]; then
    if gcc -no-pie "$s4" -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -o "$bin4" 2>/dev/null; then
      m4out=$(timeout "$TIMEOUT" "$bin4" < "$stdin_file" 2>&1)
    fi
  fi
  if printf '%s\n%s\n%s' "$m4diag" "$m4out" "$(cat "$s4" 2>/dev/null)" | grep -q 'parse error'; then
    M4_REJECT=$((M4_REJECT+1)); M4_REJECT_NAMES="$M4_REJECT_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m4 REJECT] $name"
  elif [ "$m4out" = "$exp" ]; then
    M4_PASS=$((M4_PASS+1))
  else
    M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES="$M4_FAIL_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m4 FAIL] $name"
  fi
  rm -f "$s4" "$bin4"
done

echo "=== Arizona Icon suite (upstream 9.5, general/, $TOTAL gradable of 99 vendored) ==="
echo "mode-3 (--run):      PASS=$M3_PASS REJECT=$M3_REJECT FAIL=$M3_FAIL  / $TOTAL"
echo "mode-4 (--compile):  PASS=$M4_PASS REJECT=$M4_REJECT FAIL=$M4_FAIL  / $TOTAL"
[ -n "$M3_FAIL_NAMES" ] && echo "m3 FAIL:$M3_FAIL_NAMES"
[ -n "$M4_FAIL_NAMES" ] && echo "m4 FAIL:$M4_FAIL_NAMES"
echo "m3 REJECT ($M3_REJECT):$M3_REJECT_NAMES"
echo "m4 REJECT ($M4_REJECT):$M4_REJECT_NAMES"
echo "ARIZONA_SUITE_BOARD total=$TOTAL m3_pass=$M3_PASS m3_reject=$M3_REJECT m3_fail=$M3_FAIL m4_pass=$M4_PASS m4_reject=$M4_REJECT m4_fail=$M4_FAIL"
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite Arizona --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "m3 $M3_PASS/$TOTAL · m4 $M4_PASS/$TOTAL (m3_fail=$M3_FAIL m4_fail=$M4_FAIL, reject $M3_REJECT/$M4_REJECT, \`test_icon_arizona_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

