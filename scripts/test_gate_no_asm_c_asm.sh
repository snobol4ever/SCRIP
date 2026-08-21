#!/usr/bin/env bash
# test_gate_no_asm_c_asm.sh — ⛔ LON'S LAW (2026-08-20 s194, in-chat): "You must guarantee that no ASM -> C -> ASM."
#
# THE RULE: emitted asm may call C, and C may call asm, but emitted asm must NEVER reach asm THROUGH C.  The middle C
# frame is where the machine loses its wires: gamma/omega ride r10/r11 (Lon s55) while RTCC banks r10/r11 in rtccb[32],
# a FLAT GLOBAL with NO nesting, so an inner activation entered through C overwrites the OUTER activation's banked
# continuations.  beauty's M1 SIGSEGV is that exact shape -- at the fault r11 = rip = &rtccb (FINDING s194c).
#
# ⛔ THIS IS A RATCHET, NOT A CLEAN GATE, AND THE COUNT IS COMPUTED, NEVER TYPED (the s169 lesson: a ratchet assembled
# from the violations already seen is permanently one syntax behind the code).  The floor today is the measured census;
# it MAY NOT GROW.  Override for a deliberate, reviewed change: ASM_C_ASM_RATCHET=<n>.
#
# ⛔ WHAT A GREEN HERE DOES *NOT* PROVE.  The census is a STATIC, NAME-BASED call graph: INDIRECT calls (p->fn, dtp
# slots, jump tables) are INVISIBLE to it, and that is the direction this machine uses most.  A count of 0 would prove
# only that no NAMED road remains -- never that the property holds.  Read it as a floor that must fall, not a proof.
# The census REFUSES to report at all unless it can still reproduce beauty's MEASURED M1 path (its own selftest).
set -uo pipefail
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT="$(S4E_SCRIP="$HERE/.." python3 "$HERE/util_asm_c_asm_census.py" 2>&1)"; rc=$?
echo "$OUT"
[ "$rc" = 3 ] && { echo "⛔ GATE REFUSES: census selftest failed (see above) — fix the analysis before trusting any verdict."; exit 1; }
[ "$rc" = 0 ] || { echo "⛔ GATE ERROR: census exited $rc"; exit 1; }
N="$(printf '%s\n' "$OUT" | sed -n 's/^COUNT=//p' | tail -1)"
CEIL="${ASM_C_ASM_RATCHET:-26}"
case "$N" in ''|*[!0-9]*) echo "⛔ GATE ERROR: census printed no COUNT"; exit 1;; esac
echo "------------------------------------------------------------"
if [ "$N" -gt "$CEIL" ]; then
    echo "⛔ FAIL: ASM->C->ASM roads $N EXCEEDS the ratchet $CEIL — a new road was added."
    echo "   Lon's law forbids the shape.  Either route the call so C is a LEAF, or lower the road count elsewhere."
    exit 1
fi
[ "$N" -lt "$CEIL" ] && echo "⭐ RATCHET FALLS: $N < $CEIL — lower the ceiling in this gate IN THE SAME COMMIT."
echo "OK: ASM->C->ASM roads $N (ratchet $CEIL).  ⛔ Green here is a FLOOR, not a proof — indirect calls are invisible."
exit 0
