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
#   37872 (s206, tree dda156eb, 20 Icon benchmarks, RT_OPT=-O0, SCRIP_NOFC default=1).
#         ⚠ THE INHERITED s204 FIGURE OF 39,193 DOES NOT REPRODUCE AT THIS HEAD -- re-measured
#         37,872, a gap of 1,321 (3.4%).  s204's tree predates NOFC-DEFAULT-ON (259b9cdb) and
#         the s22r/s22t SNOBOL4 spine work, all of which move shared emitter code.  The number
#         used by this gate is the MEASURED one; the inherited one is recorded only so the gap
#         is not rediscovered as a regression.  (Ceremony re-measured 873 vs s204's ~830 --
#         same reproduction caveat, same conclusion.)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-/home/claude/corpus}"
BENCH="${ICON_BENCH:-$CORPUS/benchmarks/icon}"
ICN_C_BASELINE="${ICN_C_BASELINE:-37872}"
PY="$HERE/util_icn_rbp_census.py"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$BENCH" ] || { echo "SKIP no Icon benchmark corpus at $BENCH"; exit 0; }
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
