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
  gcc -O0 -g -w -Isrc -Isrc/contracts -Isrc/runtime/rt -o "$B" "$T" \
      -Wl,-rpath,"$PWD/out" -L"$PWD/out" -lscrip_rt -lm -lpthread 2>&1 | head -5
  [ -x "$B" ] || { echo "  BUILD FAIL: $T"; RC=1; continue; }
  timeout 30s "$B" < /dev/null; [ $? -eq 0 ] || RC=1
done
[ $RC -eq 0 ] && echo "RTX UNIT: ALL PASS" || echo "RTX UNIT: FAILURES"
exit $RC
