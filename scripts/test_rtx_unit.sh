#!/usr/bin/env bash
# test_rtx_unit.sh — RTX differential batteries (asm family vs its c_* body).
# Every RTX family owes one of these: same inputs, both implementations, bit-identical results.
# Usage: bash scripts/test_rtx_unit.sh
set -uo pipefail
cd "$(dirname "$0")/.."
RC=0
for T in src/runtime/rtx/rtx_unit_test.c src/runtime/rtx/rtx_alloc_test.c src/runtime/rtx/rtx_str_test.c src/runtime/rtx/rtx_varval_test.c; do
  [ -f "$T" ] || continue
  B="/tmp/$(basename "$T" .c)"
  # ⛔ -Isrc/ir IS WHERE descr.h LIVES. This line said -Isrc/contracts, a directory DELETED in the 2026-08-24
  # srcreorg, so THREE of the four batteries had not compiled since -- rtx_unit_test, rtx_str_test and
  # rtx_varval_test each printed a BUILD FAIL that this runner reported as RTX UNIT: FAILURES, and nothing wires
  # this runner into make test, so nobody saw it. ⭐ The alloc battery kept passing because it includes no header
  # from the moved directory, which is why the runner still printed one PASS line and looked like it was working.
  # Fixed 2026-09-13 by hq_R (not this runner's owner) while making rtx_unit_test the behavioural arm of
  # test_gate_mint_op_is_one_numbering.sh -- a stale -I is the same decay class as the stale literal that row cures.
  gcc -O0 -g -w -Isrc -Isrc/ir -Isrc/runtime/rt -o "$B" "$T" \
      -Wl,-rpath,"$PWD/out" -L"$PWD/out" -lscrip_rt -lm -lpthread 2>&1 | head -5
  [ -x "$B" ] || { echo "  BUILD FAIL: $T"; RC=1; continue; }
  timeout 30s "$B" < /dev/null; [ $? -eq 0 ] || RC=1
done
[ $RC -eq 0 ] && echo "RTX UNIT: ALL PASS" || echo "RTX UNIT: FAILURES"
exit $RC
