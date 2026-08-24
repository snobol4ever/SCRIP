#!/usr/bin/env bash
# scripts/test_gate_icn_rbp_census_ratchet.sh -- ICN-FB-0 (GOAL-ICON-BB.md).
#
# THE ICON rbp INSTRUMENT.  Gates the ICN-FB ladder: Lon's s204 directive is that every Icon
# operand resolves through RSP and rbp survives ONLY for housekeeping.  Three numbers, three
# DIFFERENT contracts -- conflating them is what retired the SNOBOL4 ratchet (see that gate's
# FLATDISP-9 header):
#
#   DRIFT      ZERO-ASSERT.  A class-C ref in a region whose prologue never seeded rbp reads
#              the CALLER's frame.  Inherited from FLATDISP-9; 0 is genuinely reachable and
#              anything else is the s188/s189 defect class.  FAILS THE GATE.
#   C_data     RATCHET.  The conversion target.  Unlike SNOBOL4 -- where a seeded ref is the
#              pinned frame model working as designed and can never read zero -- an Icon
#              seeded data ref is removable by moving its GRAPH off the pin, so it ratchets.
#              FAILS THE GATE if it rises above ICN_C_BASELINE.
#   A_ceremony PIN.  ICN-FB-3's gate condition: ceremony EXACTLY UNCHANGED across a conversion
#              step (the s22c proof shape -- if ceremony moves by even one instruction the
#              step touched the class the directive protects).  Advisory here, asserted when
#              ICN_A_EXPECT is set.
#
# ⛔ RATCHET LOWERING IS MANUAL, NEVER AUTO.  3-4 sessions run in parallel (RULES.md
# CONCURRENCY); the landing session lowers ICN_C_BASELINE in the same commit as its conversion.
#
# BASELINE HISTORY:
#   ⭐ 0 (s247, seat1, SCRIP 0b1b7d4f pristine, 23 Icon benchmarks, RT_OPT=-O0) — THE DIRECTIVE IS MET.
#      C_data 37872 -> 0, A_ceremony -> 0, D_scratch -> 0, DRIFT -> 0: EVERY program in the benchmark set
#      now emits ZERO rbp references of any kind.  Verified by hand against the raw asm rather than trusting
#      an all-zero board (the "non-empty is not alive" reflex, inverted): queens.icn emits 6,707 lines with
#      0 `rbp` mentions and 2,700 `[rsp` references.  Lon's s204 directive — "ALL operands in EVERY BB
#      accessed via RSP, NOT RBP" — is achieved on the benchmark surface, and the 37,872 ceiling had become
#      37,872 units of silent headroom (the same defect this session found in icn_no_stack, ceiling 127 vs
#      measured 0).  Lowered to 0 per this gate's own TIGHTEN instruction.
#   ⛔⛔ THIS CEILING NOW COLLIDES WITH RUNG N-2 BY DESIGN, AND THAT IS DELIBERATE.  GOAL-ICON-100's N-2
#      gives Icon GENERATORS an RBP ζ-ACTIVATION frame (the SN4 R-4(b) blob shape: `push rbp; mov rbp,rsp;
#      carve`, pair at [rbp+8]/[rbp+16], registry slots at [rbp-N]) because Lon's LATER s242 ruling says a BB
#      owns an RBP frame exactly where compile-time depth is unknowable — "Icon's only dynamic-growth sites
#      are generator procedures and co-expressions".  The moment N-2 lands, C_data goes above 0 and THIS GATE
#      FAILS.  That is the correct behaviour: the two rulings (s204 "rbp only for housekeeping", s242 "frames
#      for genuinely dynamic depth") are not in conflict — s242 names WHERE the exception lives — but
#      encoding that exception here is a GATE RELAXATION of a zero-assert Lon personally directed, so it is
#      HIS CALL, not a side effect of the rung that needs it (precedent: the s230 gate-relaxation flag).
#      ⛔ N-2 MUST NOT LAND WITHOUT THAT RULING.  The proposed shape, for when it is asked: a FIFTH class
#      beside A/C/D/DRIFT — ζ-ACTIVATION refs inside graphs whose depth is genuinely dynamic (generator /
#      co-expression), counted and reported SEPARATELY, with C_data staying a zero-assert everywhere else.
#      Leaving the ceiling at 37,872 instead would let N-2 add frames with the instrument saying nothing at
#      all, which is the failure this whole rung exists to end.
#   37872 (s206, tree dda156eb, 20 Icon benchmarks, RT_OPT=-O0, SCRIP_NOFC default=1).
#         ⚠ THE INHERITED s204 FIGURE OF 39,193 DOES NOT REPRODUCE AT THIS HEAD -- re-measured
#         37,872, a gap of 1,321 (3.4%).  s204's tree predates NOFC-DEFAULT-ON (259b9cdb) and
#         the s22r/s22t SNOBOL4 spine work, all of which move shared emitter code.  The number
#         used by this gate is the MEASURED one; the inherited one is recorded only so the gap
#         is not rediscovered as a regression.  (Ceremony re-measured 873 vs s204's ~830 --
#         same reproduction caveat, same conclusion.)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"
BENCH="${ICON_BENCH:-$CORPUS/benchmarks/icon}"
ICN_C_BASELINE="${ICN_C_BASELINE:-0}"
PY="$HERE/util_icn_rbp_census.py"
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
[ -d "$BENCH" ] || { echo "⛔ REFUSED-TO-GRADE no Icon benchmark corpus at $BENCH"; exit 2; }
[ -f "$PY" ]    || { echo "GATE FAIL: missing $PY"; exit 2; }
# ⛔ EXIT STATUS READ FROM python DIRECTLY, NEVER THROUGH A PIPE -- a `| tail` reports TAIL's
# status and silently discards the verdict (the defect test_gate_fc_no_residual_rbp.sh records
# as its own root cause).  Capture, then print, then test.
OUT=$(SCRIP="$SCRIP" python3 "$PY" "$BENCH"/*.icn 2>&1); RC=$?
printf '%s\n' "$OUT"
C=$(printf '%s\n' "$OUT" | sed -n 's/^RATCHET_C=\([0-9]*\)$/\1/p' | tail -1)
A=$(printf '%s\n' "$OUT" | sed -n 's/^CEREMONY_A=\([0-9]*\)$/\1/p' | tail -1)
[ -n "$C" ] || { echo "GATE FAIL: census produced no RATCHET_C line"; exit 2; }
echo
if [ "$RC" -ne 0 ]; then
    echo "GATE FAIL: DRIFT -- class-C reference(s) naming a base the prologue never established."
    exit 1
fi
echo "ZERO-ASSERT OK: drift=0."
if [ "$C" -gt "$ICN_C_BASELINE" ]; then
    echo "GATE FAIL: RATCHET C_data=$C > baseline=$ICN_C_BASELINE (+$((C-ICN_C_BASELINE)))."
    exit 1
fi
if [ "$C" -lt "$ICN_C_BASELINE" ]; then
    echo "RATCHET OK, TIGHTEN: C_data=$C < baseline=$ICN_C_BASELINE (-$((ICN_C_BASELINE-C)))."
    echo "  -> lower ICN_C_BASELINE to $C in THIS commit (manual; parallel sessions share this file)."
else
    echo "RATCHET OK: C_data=$C == baseline."
fi
if [ -n "$ICN_A_EXPECT" ]; then
    if [ "$A" -ne "$ICN_A_EXPECT" ]; then
        echo "GATE FAIL: CEREMONY A=$A != expected $ICN_A_EXPECT -- a conversion step moved a class the directive protects (ICN-FB-3)."
        exit 1
    fi
    echo "CEREMONY PIN OK: A=$A == $ICN_A_EXPECT."
else
    echo "CEREMONY (advisory, set ICN_A_EXPECT to pin): A=$A."
fi
exit 0
