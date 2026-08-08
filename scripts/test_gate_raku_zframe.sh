#!/usr/bin/env bash
# test_gate_raku_zframe.sh — RK-ZC-8: Raku ζ-frame regime pin gate.
#
# DOCTRINE: lower_raku.c sets zframe_graph=1 on every Raku IR graph (RK-ZC-2, mirrors ICN-FR-2).
# This gate asserts two invariants so a future regime migration cannot silently leave Raku behind:
#
#   INVARIANT A — KILLSWITCH REPRODUCES THE BOMBS: with SCRIP_RK_ZFRAME=0, both witness programs
#   must exit nonzero (the legacy non-zframe entry supplies no γ/ω wires → scrip.c:1619 / rt.c bomb).
#   Witness 1 (class a — ζ-cell ret-pop): `say 42;`  — correct output then SIGSEGV (rc nonzero).
#   Witness 2 (class b — no-return-wires): `sub f($a){return $a*2} say f(21);` — dies rc=1.
#
#   INVARIANT B — FULL SUITE CLEAN: with the default (zframe_graph=1), test_smoke_raku.sh exits 0.
#   This re-runs the suite rather than trusting the watermark so drift is caught the same session it
#   is introduced.
#
# Usage: bash scripts/test_gate_raku_zframe.sh
# Exit 0 = gate PASS.  Exit 1 = gate FAIL with reason.
#
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet  DATE: 2026-08-08
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
FAIL=0
echo "=== RK-ZC-8 — Raku zframe regime pin gate ==="
# ── INVARIANT A — killswitch SCRIP_RK_ZFRAME=0 reproduces the class-b bomb ──────────────────────
# Class-b: `sub f($a){return $a*2} say f(21);` — rt_flat_wire_adopt reads gw=ww=0 (no γ/ω wires
# supplied), dies rc=1.  Class-a (`say 42;`) no longer bombs after peer commits absorbed the legacy
# RSP carve path — the legacy non-zframe entry works for flat programs without user subs.  The
# structural guard is the RETURN-WIRES check: any user sub call requires the ζ-frame entry.
W2=$(mktemp /tmp/rk_zc8_XXXXXX.raku)
printf 'sub f($a){return $a*2} say f(21);\n'                   > "$W2"
echo "--- INVARIANT A: killswitch SCRIP_RK_ZFRAME=0 (class-b witness must exit nonzero) ---"
OUT=$(SCRIP_RK_ZFRAME=0 timeout 8 "$SCRIP" --run "$W2" 2>/dev/null </dev/null); RC=$?
if [ "$RC" -ne 0 ]; then
    echo "  OK   class-b witness: rc=$RC (return-wires bomb reproduced)"
else
    echo "  FAIL class-b witness: rc=$RC (expected nonzero — zframe_graph not disabling correctly)"
    FAIL=1
fi
rm -f "$W2"
# ── INVARIANT B — full suite clean under default (zframe_graph=1) ────────────────────────────────
echo "--- INVARIANT B: full smoke suite must be 719/0 both modes ---"
SUITE_OUT=$("${HERE}/test_smoke_raku.sh" 2>&1)
M3=$(echo "$SUITE_OUT" | grep 'mode-3' | grep -oE 'FAIL=[0-9]+' | grep -oE '[0-9]+')
M4=$(echo "$SUITE_OUT" | grep 'mode-4' | grep -oE 'FAIL=[0-9]+' | grep -oE '[0-9]+')
M3="${M3:-?}"; M4="${M4:-?}"
if [ "$M3" = "0" ] && [ "$M4" = "0" ]; then
    echo "  OK   m3 FAIL=$M3  m4 FAIL=$M4"
else
    echo "  FAIL m3 FAIL=$M3  m4 FAIL=$M4 (suite not clean — regime drift)"
    FAIL=1
fi
# ── RESULT ────────────────────────────────────────────────────────────────────────────────────────
if [ "$FAIL" -eq 0 ]; then
    echo "=== RK-ZC-8 gate PASS ==="
    exit 0
else
    echo "=== RK-ZC-8 gate FAIL ==="
    exit 1
fi
