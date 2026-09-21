#!/usr/bin/env bash
# test_gate_the_tiny_open_protocol_and_its_glue_record_agree.sh -- TWO FILES HOLD ONE CONTRACT AND NOTHING BOUND THEM.
#
# ⛔⭐ THE DEFECT, MEASURED BY THE ceo AT THE COST OF TWO FAILED CONVERSION ATTEMPTS (CEO-1089, cured by the cto
# 2026-09-21 as spine).  rt_call_open_by_name returns a two-word {entry, protocol}; the protocol's low byte is `how`.
# bb_glue_enter_c2bb performs the jump, and its how==2 arm (the TINY/alpha record) builds a FIXED 48-byte record whose
# ARGUMENT COUNT IS THE LITERAL ZERO and which never copies g_call_args.  rt_tiny_record_enter's own asm shim -- the
# other way into the same target -- sizes its record FROM nargs, copies every argument out of g_call_args, and writes
# the real count.  So the two roads into one callee disagreed about the record's shape.
# ⛔ IT PRESENTS AS A WRONG ANSWER, NOT A CRASH, WHICH IS THE WORST SHAPE A DEFECT CAN WEAR: a target opened onto
# how=2 with arguments enters WITH NO ARGUMENTS and reads unbound parameters.  The ceo measured APPLY(ZFN,41)
# returning 1 instead of 42.  Nothing faulted; a number was simply wrong.
# ⭐ WHY THE RAKU METHOD ROAD NEVER EXPOSED IT, and why that is luck rather than safety: rt_define_returns_by_frame
# forces jmp_entry and !dyn_scope for those targets, which is how=0.  rk_method_open at by_name_dispatch.c neverthless
# opens with `total` arguments, so the road was one lowering decision away from firing.
#
# ⭐ THE CURE IS TO MAKE THE TWO SITES AGREE RATHER THAN TO REMEMBER THAT THEY DO NOT: the open now takes the tiny
# road ONLY at nargs<=0, which is exactly the record the glue can carry.  A target with arguments opens onto the
# NAMED road instead, whose prologue stages them properly.  Nothing correct could have depended on the old behaviour,
# because the old behaviour was a wrong answer.
# ⛔ AND THIS GATE IS THE POINT: the contract spans src/runtime/rt/rt.c and src/templates/bb/bb_glue_flat.cpp, two
# files no reader opens together, which is why it survived.  If a later seat TEACHES THE GLUE TO CARRY ARGUMENTS --
# the better cure, and the one this gate is written to make safe -- arm 2 goes green on its own and arm 3 then
# REQUIRES the nargs guard to be lifted, so the gate does not freeze the workaround into law.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
RT="src/runtime/rt/rt.c"; GLUE="src/templates/bb/bb_glue_flat.cpp"
[ -f "$RT" ] && [ -f "$GLUE" ] || { echo "REFUSE(2): a source file this gate grades is missing"; exit 2; }

glue_body="$(sed -n '/^std::string bb_glue_enter_c2bb/,/^}/p' "$GLUE")"
[ -n "$glue_body" ] || { echo "REFUSE(2): bb_glue_enter_c2bb not found in $GLUE -- the gate cannot grade what it cannot read"; exit 2; }

if [ "${FAIL_ONCE:-0}" = "1" ]; then
  glue_body="$(printf '%s\n' "$glue_body" | sed 's/x86("mov",  RDQ("rsp", 0), 0L)/x86("mov",  RDQ("rsp", 0), "rsi")/')"
  echo "FAIL_ONCE: pretending the glue stores a REAL count; arm 3 must then demand the nargs guard be lifted."
fi

rc=0
carries_args=0
printf '%s\n' "$glue_body" | grep -q 'g_call_args' && carries_args=1
hardcodes_zero=0
printf '%s\n' "$glue_body" | grep -q 'RDQ("rsp", 0), 0L' && hardcodes_zero=1

echo "arm 1: the glue's how==2 record copies g_call_args? $( [ $carries_args = 1 ] && echo YES || echo NO )"
echo "arm 2: the glue's how==2 record hardcodes the count to zero? $( [ $hardcodes_zero = 1 ] && echo YES || echo NO )"

guarded=0
grep -q 'if (p->dyn_scope && nargs <= 0) {.*rt_c2bb_word(p, (long)(uintptr_t)afn, 2, 0)' "$RT" && guarded=1
echo "arm 3: rt_call_open_by_name guards its how=2 return on nargs? $( [ $guarded = 1 ] && echo YES || echo NO )"

if [ $carries_args = 0 ] && [ $hardcodes_zero = 1 ]; then
  if [ $guarded = 0 ]; then
    echo "⛔ GATE FAILED: the glue's how==2 record carries NO arguments and a hardcoded zero count, but rt_call_open_by_name can still return how=2 for a call WITH arguments. That target enters with no arguments and returns a WRONG ANSWER rather than crashing (CEO-1089)."
    rc=1
  else
    echo "✅ the two sites agree: the glue carries only the zero-argument record, and the open takes that road only at nargs<=0"
  fi
else
  if [ $guarded = 1 ]; then
    echo "⛔ GATE FAILED: the glue's how==2 record now carries arguments, so the nargs<=0 guard in rt_call_open_by_name is STALE and is needlessly forcing argument-bearing calls onto the named road. Lift the guard -- this gate exists to make the better cure safe, not to freeze the workaround into law."
    rc=1
  else
    echo "✅ the two sites agree: the glue carries a real argument count and the open is free to use the tiny road"
  fi
fi
exit $rc
