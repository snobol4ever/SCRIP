#!/usr/bin/env bash
# FACT RULE "ONE MEDIUM, INVISIBLE": a BB template never hand-encodes an instruction as raw bytes nor
# branches on the medium for an instruction. Forbidden in BB_templates/*.cpp (comments stripped):
#   raw-byte producers: x86_Lrec, x86_Jrec, x86_Drec, x86_b1(, x86_b2(, x86_b3(, bytes(, u8(, u32le, u64le
#   instruction medium-branch: IF(MEDIUM_BINARY , IF(MEDIUM_MACRO_DEF
# ALLOWED: IF(MEDIUM_TEXT, ...) for a comment/label only. x86_asm.h is the encoder impl (exempt).
# Informational WIP baseline; --strict enforces zero.
cd "$(dirname "$0")/.." || exit 2
total=0; hits=""
for f in src/templates/*.cpp; do
  code=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f")
  m=$(printf '%s' "$code" | grep -cE 'x86_Lrec|x86_Jrec|x86_Drec|x86_b1\(|x86_b2\(|x86_b3\(|\bbytes\(|\bu8\(|\bu32le|\bu64le|IF\(MEDIUM_BINARY|IF\(MEDIUM_MACRO_DEF')
  if [ "$m" -gt 0 ]; then total=$((total+m)); hits="$hits $(basename $f)($m)"; fi
done
echo "medium-branch / raw-byte producers in BB templates (code): $total  (target 0)"
rc=0
if [ "$total" -ne 0 ]; then
  echo "REMAINING:$hits"
  echo "(informational WIP baseline — each owning GOAL-*-BB session drives its boxes to 0 as it converts.)"
  [ "${1:-}" = "--strict" ] && { echo "GATE FAIL (--strict)."; rc=1; }
else
  echo "OK: every BB template is medium-invisible (pure x86() concat)."
fi

# =====================================================================================================
# BOTH-MEDIUM RATCHET (RULES.md "NO MEDIUM_* IN TEMPLATES") — added s169 (seat1, queue row gates-dead-paths).
# RULES.md carried the ratchet as a TYPED NUMBER (29) next to a DIFFERENT command (`grep -rn 'MEDIUM_'
# src/templates/bb_*.cpp`, which yields 38). A ratchet whose number and whose command disagree cannot be
# enforced: the next seat runs the documented command, sees 38, and reads a 9-site regression that never
# happened. The 29 is the GUARD-SITE count -- `if (MEDIUM_` plus `IF(MEDIUM_` -- and guard sites are what
# the rule actually forbids ("any function gating output on MEDIUM_TEXT/MEDIUM_BINARY is a violation").
# That is the number pinned here, computed, never typed. Retire the sites via queue row `medium-retire`.
MEDIUM_RATCHET="${MEDIUM_RATCHET:-3}"
guards=$(grep -hoE 'if \(MEDIUM_|IF\(MEDIUM_' src/templates/bb_*.cpp 2>/dev/null | wc -l)
echo "BOTH-MEDIUM guard sites in src/templates/bb_*.cpp: $guards  (ratchet ceiling $MEDIUM_RATCHET, target 0)"
if [ "$guards" -gt "$MEDIUM_RATCHET" ]; then
  echo "RATCHET FAIL: $guards MEDIUM_* guard site(s) > ceiling $MEDIUM_RATCHET — the known-red count MAY NOT GROW."
  rc=1
fi
[ "$guards" -lt "$MEDIUM_RATCHET" ] && echo "RATCHET IMPROVED: $guards < $MEDIUM_RATCHET — lower MEDIUM_RATCHET in this script to lock the gain in."
exit "$rc"
