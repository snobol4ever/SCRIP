#!/usr/bin/env bash
# FACT RULE "bb_bin_t IS ABOLISHED": zero references to the deleted type / consumer IN CODE (comments
# stripped). The type+helper are deleted from emit_str.h/.cpp; every box emits via bb_emit_x86.
cd "$(dirname "$0")/.." || exit 2
total=0; hits=""
while IFS= read -r f; do
  # strip block comments first (whole-file), then line comments, THEN grep
  m=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f" | grep -cE '\bbb_bin_t\b|\bbb_emit_asm_result\b')
  if [ "$m" -gt 0 ]; then total=$((total+m)); hits="$hits $f($m)"; fi
done < <(find src/emitter -name '*.cpp' -o -name '*.c' -o -name '*.h')
echo "bb_bin_t / bb_emit_asm_result live (code, comments stripped): $total  (MUST be 0)"
if [ "$total" -ne 0 ]; then echo "OFFENDERS:$hits"; echo "GATE FAIL."; exit 1; fi
echo "OK: bb_bin_t abolished (zero live code references)."
