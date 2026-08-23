#!/bin/bash
# TE gate (TMP-ERADICATE): the emit-time slot allocators are DELETED — zero live references allowed.
# bb_slot_alloc16 / bb_slot_alloc16_or_get / bb_slot_claim / bb_flat_cursor_reserve exist nowhere as code
# (the compiler enforces it: symbols undefined). This gate guards resurrection. Comments stripped via gcc.
# KEEPERS, deliberately not matched: bb_slot_get / bb_slot_register (the per-glob node->offset memo table).
cd "$(dirname "$0")/.."
n=0
# ⭐ V2-5 GATE HONESTY: examining nothing must exit UNPROVEN(2), never read as a pass.
. "$(dirname "$0")/lib_gate.sh"
gate_floor "$(ls "$(dirname "$0")"/../src/emitter/*.cpp "$(dirname "$0")"/../src/templates/*.cpp 2>/dev/null | wc -l)" 100 "emitter/template source files"
for f in $(grep -rlE 'bb_slot_alloc16|bb_slot_claim|bb_flat_cursor_reserve' src --include=*.c --include=*.cpp --include=*.h 2>/dev/null); do
  c=$(gcc -fpreprocessed -dD -E -P "$f" 2>/dev/null | grep -cE '\bbb_slot_alloc16(_or_get)?\b|\bbb_slot_claim\b|\bbb_flat_cursor_reserve\b')
  n=$((n+c))
done
echo "live emit-time allocator references (comments stripped): $n (must be 0)"
if [ "$n" -eq 0 ]; then echo "PASS: emit-time temp allocation is eradicated (LOWER ir_drive_slot_assign is the ONLY slot source)."; exit 0; else echo "FAIL: allocator resurrected"; exit 1; fi
