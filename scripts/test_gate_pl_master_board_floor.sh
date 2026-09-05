#!/usr/bin/env bash
# test_gate_pl_master_board_floor.sh -- FLOOR gate for the Prolog master board (ceo CEO-152: "the master
# board >= its pre-cure count in both modes"). Runs the master in 16 shards and sums them.
#
# ⛔ A FLOOR, NOT A TARGET: it fails when the board drops BELOW the pin, never for being above it. Raise the
# pin in the landing commit that earns it, and say what moved it (never a PINNED POPULATION -- see A CRITERION NEVER PINS
# A POPULATION COUNT in .github/GOAL-TEST-SUITE-CONSISTENCY.md, the anti-pattern that retired the optbypass watermark).
# ⛔ PER MODE, NEVER SUMMED (the ceo's standing rule for this board): m3 and m4 are graded independently and
# a rise in one may not pay for a fall in the other.
# ⭐ SHARDED BECAUSE THE MASTER EXCEEDS THE SINGLE-CALL CAP (row corpus-runner-master-suite-exceeds-single-call-cap,
# hq_B 2026-09-02); --shard k/N partitions the suite exactly once so the shard boards SUM to the monolithic board.
# ⛔ It REFUSES rc=2 rather than reporting a floor it could not measure: a missing binary, a missing suite, or any
# shard that fails to print its SUITE_BOARD line means no verdict was possible -- never a green from a short count.
#
# PIN 198/198 (m3/m4), set 2026-09-02 by hq_B at the pre-cure count measured on SCRIP 5d12c898 + corpus 3196897d,
# BEFORE the ninit==0 fallback deletion and the seven XFAIL promotions. Both landed at 209/209; the pin is left at
# the pre-cure 198 deliberately, because this row's claim is "the cure did not cost the board", and a floor raised
# to the post-cure number in the same commit can no longer make that statement about the next change.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP:-$ROOT/scrip}"; P="${PL_SUITE_DIR:-$S4E/corpus/tests/prolog}"
PIN_M3="${PIN_M3:-230}"; PIN_M4="${PIN_M4:-230}"; SHARDS="${SHARDS:-16}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip not built at $SCRIP"
[ -f "$P/ALL.pl" ] && [ -f "$P/ALL.ref" ] || refuse "master suite not found at $P"
W="$(mktemp -d)" || refuse "mktemp failed"; trap 'rm -rf "$W"' EXIT
for k in $(seq 1 "$SHARDS"); do
  timeout 900 python3 "$HERE/corpus_suite_harness.py" run "$P/ALL.pl" "$P/ALL.ref" --lang prolog --modes m3,m4 --shard "$k/$SHARDS" 2>&1 | grep '^SUITE_BOARD' >> "$W/b.txt"
done
got="$(grep -c . "$W/b.txt" 2>/dev/null || echo 0)"
[ "$got" -eq "$SHARDS" ] || refuse "only $got of $SHARDS shards printed a SUITE_BOARD line -- the board was not fully measured"
read -r tot m3 m4 <<<"$(awk '{for(i=1;i<=NF;i++){split($i,a,"=");if(a[1]=="total")t+=a[2];if(a[1]=="m3_pass")p3+=a[2];if(a[1]=="m4_pass")p4+=a[2]}} END{print t, p3, p4}' "$W/b.txt")"
echo "=== prolog master board floor: total=$tot  m3_pass=$m3 (pin $PIN_M3)  m4_pass=$m4 (pin $PIN_M4) ==="
fail=0
[ "$m3" -ge "$PIN_M3" ] && echo "  ok    m3 $m3 >= $PIN_M3" || { echo "  FAIL  m3 $m3 BELOW pin $PIN_M3"; fail=1; }
[ "$m4" -ge "$PIN_M4" ] && echo "  ok    m4 $m4 >= $PIN_M4" || { echo "  FAIL  m4 $m4 BELOW pin $PIN_M4"; fail=1; }
[ "$fail" -eq 0 ] || { echo "⛔ GATE FAIL: the master board fell below its floor"; exit 1; }
echo "✅ GATE PASS: board at or above the floor in both modes"; exit 0
