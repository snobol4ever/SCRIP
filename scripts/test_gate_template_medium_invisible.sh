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
# ⛔ BLIND SPOT CLOSED s170 (seat1, row `medium-retire`). The s169 ratchet counted `if (MEDIUM_` + `IF(MEDIUM_`
# because those were the shapes the census had seen. They are NOT the only way to gate on the medium: two live
# sites in bb_call_proc_staged spelled it as a CONDITIONAL EXPRESSION -- `(MEDIUM_BINARY && cell ? A : B)` --
# and the guard regex walked straight past both, so the true count was 31, not 29. A ratchet that enumerates
# SPELLINGS will always be one syntax behind the code. This one now counts every MEDIUM_* token that survives
# comment-stripping, which is spelling-independent and is what the rule actually says ("Zero MEDIUM_* in any
# bb_*.cpp"); the guard-site number is kept beside it as a breakdown, never as the gate.
# ⛔ RATCHET CLOSED s172 (seat6, row `ab-cell-hoist`): 3 -> 0.  The last three were all in bb_define.cpp and all one class -- the AB fn-cell
# store (g_ab_fn_cells + bb_ab_cell_addr) is C-side LIVE-IMAGE state that only a BINARY image has, so the template had to ask the
# medium whether a cell existed.  The store was hoisted to src/emitter/emit.cpp (the file that owns g_medium) and bb_ab_cell_addr now
# answers NULL for TEXT; all three sites became tests of the returned POINTER.  Per-medium byte-identity measured: 0 movers / 527
# comparable .s and 0 movers / 575 mode-3 runs.  THE CEILING IS NOW ZERO AND MUST STAY ZERO -- this is no longer a known-red ratchet
# but a live invariant: a single MEDIUM_* token in any bb_*.cpp fails this gate.
MEDIUM_RATCHET="${MEDIUM_RATCHET:-0}"
sites=0
for f in src/templates/bb_*.cpp; do
  code=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f")
  n=$(printf '%s' "$code" | grep -o 'MEDIUM_[A-Z_]*' | wc -l)
  [ "$n" -gt 0 ] && { sites=$((sites+n)); echo "  MEDIUM_ code sites: $(basename $f) ($n)"; }
done
guards=$(grep -hoE 'if \(MEDIUM_|IF\(MEDIUM_' src/templates/bb_*.cpp 2>/dev/null | wc -l)
echo "BOTH-MEDIUM code sites in src/templates/bb_*.cpp: $sites  (ratchet ceiling $MEDIUM_RATCHET, target 0; of these, $guards are if/IF guard sites)"
if [ "$sites" -gt "$MEDIUM_RATCHET" ]; then
  echo "RATCHET FAIL: $sites MEDIUM_* code site(s) > ceiling $MEDIUM_RATCHET — the known-red count MAY NOT GROW."
  rc=1
fi
[ "$sites" -lt "$MEDIUM_RATCHET" ] && echo "RATCHET IMPROVED: $sites < $MEDIUM_RATCHET — lower MEDIUM_RATCHET in this script to lock the gain in."
exit "$rc"
