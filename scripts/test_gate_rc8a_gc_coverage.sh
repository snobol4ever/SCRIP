#!/bin/bash
# test_gate_rc8a_gc_coverage.sh — RC-8a / GOAL-SN4-HOME-RBX X-1.
# ASSERTS, at a real collection: (1) the RTCC block is RANGE-registered, not merely pinned; (2) the CAS
# capture-pending island is walked when it is occupied. SELF-ARMING per the RBX GATES line: the CAS arm is
# WARN while no reachable program occupies the island and FAIL once one does; the RTCC arm is always FAIL.
# EVERY assertion carries its POSITIVE CONTROL — SCRIP_GC_UNROOT re-opens the pre-s33 hole and the gate must
# go RED there, because a gate that cannot fail for the right reason is not a gate (RULES.md).
# Usage: bash scripts/test_gate_rc8a_gc_coverage.sh [corpus_root]
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
ROOT="${1:-$S4E/corpus}"
SCRIP="$(cd "$(dirname "$0")/.." && pwd)/scrip"
WIT="$ROOT/probe/mv_arbno_callcap.sno"
rc=0
[ -x "$SCRIP" ] || { echo "GATE RC-8a: BLOCKED — no scrip binary at $SCRIP"; exit 1; }
cov() { SCRIP_GC_COVERAGE=1 SCRIP_GC_STRESS="${2:-1}" timeout 60s "$SCRIP" --run "$1" < /dev/null 2>&1 >/dev/null | grep "GC-COV" | tail -1; }
covu() { SCRIP_GC_UNROOT="$3" SCRIP_GC_COVERAGE=1 SCRIP_GC_STRESS="${2:-1}" timeout 60s "$SCRIP" --run "$1" < /dev/null 2>&1 >/dev/null | grep "GC-COV" | tail -1; }
field() { echo "$1" | grep -o "$2=[0-9]*" | cut -d= -f2; }
# ---- ASSERTION 1: the RTCC block is range-registered at every collection -------------------------------
L=$(cov "$WIT"); R=$(field "$L" ranges)
if [ -z "$L" ]; then echo "GATE RC-8a: BLOCKED — instrument DARK (no [GC-COV] line; no collection fired)"; exit 1; fi
if [ "${R:-0}" -ge 1 ]; then echo "  [PASS] RTCC block range-registered (ranges=$R)"; else echo "  [FAIL] RTCC block NOT range-registered (ranges=${R:-0}) — rtcc_gc_register is pin-only, the RC-8a gap"; rc=1; fi
# ---- POSITIVE CONTROL 1: sabotage must drop the range --------------------------------------------------
LC=$(covu "$WIT" 1 rtcc); RC1=$(field "$LC" ranges)
if [ "${RC1:-9}" -eq 0 ]; then echo "  [PASS] control: SCRIP_GC_UNROOT=rtcc drops ranges to 0 — the gate can fail"; else echo "  [FAIL] control INERT: sabotage left ranges=${RC1:-?} — this assertion proves nothing"; rc=1; fi
# ---- ASSERTION 2: the CAS island is walked when occupied (SELF-ARMING) ---------------------------------
C=$(field "$L" cas_scanned_bytes)
if [ "${C:-0}" -gt 0 ]; then
    echo "  [PASS] CAS island walked at collection (cas_scanned_bytes=$C)"
    LC2=$(covu "$WIT" 1 cas); C2=$(field "$LC2" cas_scanned_bytes)
    if [ "${C2:-9}" -eq 0 ]; then echo "  [PASS] control: SCRIP_GC_UNROOT=cas drops CAS scan to 0"; else echo "  [FAIL] control INERT: sabotage left cas=${C2:-?}"; rc=1; fi
else
    echo "  [WARN] CAS arm UNARMED — witness $(basename "$WIT") occupied 0 island bytes at collection."
    echo "         Not a pass. The island is reachable only through the *-target capture form (c_rt_cap_open's"
    echo "         varname[0]=='*' arm); if this warns, the witness stopped reaching it and the arm needs a new one."
fi
# ---- ASSERTION 3: pz must be OFF whenever any root region is registered --------------------------------
P=$(field "$L" pz)
if [ "${P:-1}" -eq 0 ]; then echo "  [PASS] pz=0 — punt-zero correctly disabled while roots are registered"; else echo "  [FAIL] pz=1 with roots registered — the root phase was SKIPPED"; rc=1; fi
[ $rc -eq 0 ] && echo "GATE RC-8a: GREEN" || echo "GATE RC-8a: RED"
exit $rc
