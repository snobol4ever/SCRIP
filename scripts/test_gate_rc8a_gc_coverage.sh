#!/bin/bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_rc8a_gc_coverage.sh — RC-8a / GOAL-SN4-HOME-RBX X-1.
# ASSERTS, at a real collection: the CAS capture-pending island is walked when it is occupied. SELF-ARMING per the RBX
# GATES line: the arm PASSES only on a collection that found the island occupied and walked it.
#
# ⛔ THE WITNESS IS INLINE, AND WHY (the coo 2026-10-01, row instruments-test-gate-rc8a-gc-coverage-refuses-rc-2-...): corpus
# c06960a12 (2026-08-29) deleted corpus/probe/, so probe/mv_arbno_callcap.sno was gone and this gate refused rc 2 for a month, in
# no blocking arm, so nobody read it. That program survives as SNOBOL4 master entry 1517 (origin probe_probe_top__mv_arbno_callcap)
# and MEASURED, extracted through corpus_suite_harness.py and run at its declared --stlimit -d131072k -s4096k: 4 collections at
# SCRIP_GC_STRESS 1 and 2, max cas_scanned_bytes 0 at stress 1 2 3 8 64 -- its *p() allocates nothing, so no collection fires
# while the capture is pending and it can never arm the island again. The witness below is its shape with a deferred function
# that ALLOCATES (64 bytes a step, 200 steps) inside ARBNO(LEN(1) . *f()): at stress 1 it reads 802 collections and max
# cas_scanned_bytes=48, and prints S, as sbl -bf does. It is a gate fixture, never a suite program, so it is written here and
# graded by its output as well as by the collector line; it runs at the shipped arena and declares nothing.
#
# ⛔ TWO ASSERTIONS RETIRED, EACH BY THE COLLECTOR LANDING THAT REMOVED WHAT IT TESTED (the coo 2026-10-01, measured):
#   (1) "the RTCC block is RANGE-registered at every collection" -- be6b03b5e (2026-09-22) removed the registration on
#       purpose ("the caller-saved spill block is no longer registered as a GC root range -- the registration had no
#       consumer and told every reader a lie"); the block's invariant is now that it never holds a heap reference, graded
#       by test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh. [GC-COV] reads ranges=0 by design.
#   (3) "pz=0 whenever a root region is registered" -- c6bf8e789 (2026-09-19, THE E SWITCH) deleted punt-zero with the
#       word sweep and the descriptor sniff, and [GC-COV] has printed no pz field since; the old arm read the absent field
#       as pz=1 and would have called it a FAIL.
# ⛔ AN UNARMED ARM MEASURED NOTHING, SO IT REFUSES (rc 2), NEVER GREEN: with (1) and (3) retired the CAS arm is the gate.
#
# The positive control is a SOURCE CENSUS of SCRIP's own src/ for the un-rooting escape SCRIP_GC_UNROOT. It used to grep
# "$ROOT/src" with ROOT the CORPUS, which has no src/, so the census read "unreachable" whatever the runtime held.
# Usage: bash scripts/test_gate_rc8a_gc_coverage.sh
# Exit: 0 the armed arm walked the island · 1 a FAIL (wrong output, or the escape is back) · 2 could not measure (no binary,
# no collection, unarmed).
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
SCRIP="$HERE/../scrip"
SRC="$HERE/../src"
[ -x "$SCRIP" ] || { echo "GATE RC-8a: REFUSE(2) — no scrip binary at $SCRIP (cannot measure)"; exit 2; }
[ -d "$SRC" ] || { echo "GATE RC-8a: REFUSE(2) — no source tree at $SRC for the control census (cannot measure)"; exit 2; }
T=$(mktemp -d) || { echo "GATE RC-8a: REFUSE(2) — mktemp"; exit 2; }
trap 'rm -rf "$T"' EXIT TERM INT
WIT="$T/rc8a_cas_witness.sno"
cat > "$WIT" <<'SNO'
        DEFINE("f()")                       :(f_end)
f       i = 0
floop   i = i + 1
        s = s DUPL('y', 64)
        LT(i, 200)                          :S(floop)
        f = .dummy                          :(NRETURN)
f_end
        pat = ARBNO(LEN(1) . *f()) 'z'
        'xxz' ? pat                         :F(FF)
        OUTPUT = 'S'                        :(END)
FF      OUTPUT = 'F'
END
SNO
WANT=S   # sbl -bf prints S (the coo 2026-10-01)
# every [GC-COV] line of the run, one per collection
cov() { ( cd "$T" && SCRIP_GC_COVERAGE=1 SCRIP_GC_STRESS="${1:-1}" timeout 60s "$SCRIP" --run "$WIT" < /dev/null 2> "$T/err" > "$T/out"; echo "rc=$?" > "$T/rc" ); grep "GC-COV" "$T/err"; }
L=$(cov 1)
GOT=$(cat "$T/out" 2>/dev/null)
[ "$(cat "$T/rc")" = rc=124 ] && { echo "GATE RC-8a: REFUSE(2) — the witness ran past 60 s (a timeout firing is not a verdict), so nothing was measured"; exit 2; }
[ "$(cat "$T/rc")" = rc=0 ] && [ "$GOT" = "$WANT" ] || { echo "GATE RC-8a: FAIL — the witness printed [$GOT] with $(cat "$T/rc"); want [$WANT] rc=0 (sbl -bf): the collection mid-match broke the program"; echo "GATE RC-8a: RED"; exit 1; }
if [ -z "$L" ]; then echo "GATE RC-8a: REFUSE(2) — instrument DARK (no [GC-COV] line; no collection fired), so nothing was measured"; exit 2; fi
N=$(grep -c . <<<"$L")
echo "  witness: inline (the gate's own fixture), printed [$GOT] as sbl -bf does; $N collection(s) read at SCRIP_GC_STRESS=1"
echo "  [RETIRED] (1) RTCC range registration -- removed by be6b03b5e; the spill block's invariant is test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh"
echo "  [RETIRED] (3) pz=0 -- punt-zero deleted by c6bf8e789; [GC-COV] prints no pz field"
# ---- ASSERTION 2: the CAS island is walked when occupied (SELF-ARMING) ---------------------------------
C=$(grep -o 'cas_scanned_bytes=[0-9]*' <<<"$L" | cut -d= -f2 | sort -n | tail -1)
if [ "${C:-0}" -gt 0 ]; then
    echo "  [PASS] CAS island walked at collection (max cas_scanned_bytes=$C over $N collection(s))"
    if ! grep -rq 'SCRIP_GC_UNROOT' "$SRC" 2>/dev/null; then echo "  [PASS] control (SOURCE CENSUS of $SRC): the cas un-rooting escape is UNREACHABLE — zero SCRIP_GC_UNROOT"; else echo "  [FAIL] control: SCRIP_GC_UNROOT is back in $SRC — the cas arm needs its runtime sabotage control restored"; echo "GATE RC-8a: RED"; exit 1; fi
    echo "GATE RC-8a: GREEN"; exit 0
fi
echo "GATE RC-8a: REFUSE(2) — CAS arm UNARMED: the witness occupied 0 island bytes at all $N collection(s), so nothing was measured."
echo "         The island is reachable only through the *-target capture form (c_rt_cap_open's varname[0]=='*' arm) with a"
echo "         collection firing while the capture is pending; the witness above stopped reaching it and the arm needs one that does."
exit 2
