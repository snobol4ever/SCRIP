#!/usr/bin/env bash
# test_gate_no_zeta_frame_switches.sh — THE PRISON for RULES.md FACT RULE "THE ζ TIER IS DERIVED, NEVER ENUMERATED"
# (Lon 2026-09-09 09:2x CDT: "Get rid of enumerated shape class now!!!"; ceo CEO-447; landed by the cto).
#
# A box's tier (ζ-SPINE on RSP vs ζ-ACTIVATION-FRAME on RBP) is DERIVED per graph from the two frame-placement
# conditions in ONE selector. Every env switch that once chose a frame placement by hand -- the 2026-09-09 census
# under src/ was 29 names matching RBP|FRAME|PIN|ZFRAME|CARVE -- is a ζ mode, and ZETA HAS NO MODES. This gate is a
# pure SOURCE CENSUS, no build: it reads ZERO getenv("SCRIP_*") names matching that class under src/, and names every
# survivor. It is wired blocking in the same commit as the deletion (MASTER-PLAN: an instrument in no runner is not
# measuring). ~0.05s.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
[ -d "$ROOT/src" ] || { echo "⛔ REFUSE(2): no src/ under $ROOT -- a census over nothing"; exit 2; }
hits=$(grep -rnoE 'getenv\("SCRIP_[A-Z0-9_]*(RBP|FRAME|PIN|ZFRAME|CARVE)[A-Z0-9_]*"\)' "$ROOT/src" 2>/dev/null)
n=$(printf '%s' "$hits" | grep -c . || true)
if [ "$n" -ne 0 ]; then
  echo "⛔ FAIL: $n zeta frame switch(es) under src/ -- the tier is DERIVED, never enumerated (RULES.md, CEO-447):"
  printf '%s\n' "$hits" | sed "s#^$ROOT/##"
  exit 1
fi
# CEO-448 (Lon: "get rid of all that ZLS and ZETA #define's"): no ZC_/ZLS identifier, no rt_zls store symbol, no
# zeta_choices/zeta_alloc file may exist under src/ -- the frame layout is typed per activation, never a store.
zhits=$(grep -rnoE '\b(ZC_[A-Z0-9_]+|ZLS[A-Z0-9_]*|rt_zls2?_[a-z_]+)\b' "$ROOT/src" 2>/dev/null; ls "$ROOT/src/ir/zeta_choices.h" "$ROOT/src/runtime/rt/zeta_alloc.c" "$ROOT/src/runtime/rt/zeta_alloc.h" 2>/dev/null)
zn=$(printf '%s' "$zhits" | grep -c . || true)
if [ "$zn" -ne 0 ]; then
  echo "⛔ FAIL: $zn ZC_/ZLS identifier(s) or zeta store file(s) under src/ -- the ζ stores and defines died in CEO-448:"
  printf '%s\n' "$zhits" | sed "s#^$ROOT/##"
  exit 1
fi
echo "✅ PASS: 0 zeta frame switches (getenv SCRIP_*{RBP,FRAME,PIN,ZFRAME,CARVE}*) and 0 ZC_/ZLS identifiers or zeta store files under src/ -- the tier is derived, the layout is typed"
exit 0
