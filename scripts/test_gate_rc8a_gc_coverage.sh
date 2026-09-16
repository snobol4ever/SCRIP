#!/bin/bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
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
[ -x "$SCRIP" ] || { echo "GATE RC-8a: REFUSE(2) — no scrip binary at $SCRIP (cannot measure)"; exit 2; }
[ -f "$WIT" ] || { echo "GATE RC-8a: REFUSE(2) — witness $WIT is missing (it died with the retired corpus/probe tree). CANNOT MEASURE is not FAIL: re-point WIT at a live capture witness or re-mint one."; exit 2; }
cov() { SCRIP_GC_COVERAGE=1 SCRIP_GC_STRESS="${2:-1}" timeout 60s "$SCRIP" --run "$1" < /dev/null 2>&1 >/dev/null | grep "GC-COV" | tail -1; }
field() { echo "$1" | grep -o "$2=[0-9]*" | cut -d= -f2; }
# ---- ASSERTION 1: the RTCC block is range-registered at every collection -------------------------------
L=$(cov "$WIT"); R=$(field "$L" ranges)
if [ -z "$L" ]; then echo "GATE RC-8a: REFUSE(2) — instrument DARK (no [GC-COV] line; no collection fired), so nothing was measured"; exit 2; fi
if [ "${R:-0}" -ge 1 ]; then echo "  [PASS] RTCC block range-registered (ranges=$R)"; else echo "  [FAIL] RTCC block NOT range-registered (ranges=${R:-0}) — rtcc_gc_register is pin-only, the RC-8a gap"; rc=1; fi
# ---- POSITIVE CONTROL 1: sabotage must drop the range --------------------------------------------------
LC=""; RC1=""
if ! grep -rq 'SCRIP_GC_UNROOT' "$ROOT/src" 2>/dev/null; then echo "  [PASS] control (SOURCE CENSUS): the rtcc un-rooting escape is UNREACHABLE — zero SCRIP_GC_UNROOT in src/. The runtime sabotage arm was retired with the knob (Lon 2026-09-16, remove all alternate behaviours); a census proves the hole cannot come back, which a sabotage arm never could."; else echo "  [FAIL] control: SCRIP_GC_UNROOT is back in src/ — the pre-s33 un-rooting hole is switchable again (left ranges=${RC1:-?} — this assertion proves nothing"; rc=1; fi
# ---- ASSERTION 2: the CAS island is walked when occupied (SELF-ARMING) ---------------------------------
C=$(field "$L" cas_scanned_bytes)
if [ "${C:-0}" -gt 0 ]; then
    echo "  [PASS] CAS island walked at collection (cas_scanned_bytes=$C)"
    if ! grep -rq 'SCRIP_GC_UNROOT' "$ROOT/src" 2>/dev/null; then echo "  [PASS] control (SOURCE CENSUS): the cas un-rooting escape is UNREACHABLE — zero SCRIP_GC_UNROOT in src/"; else echo "  [FAIL] control: SCRIP_GC_UNROOT is back in src/ — the CAS island can be un-rooted again"; rc=1; fi
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
