#!/usr/bin/env bash
# test_gate_wreg_claim.sh — WREG r10/r11 CLAIM SCOPE GATE (GOAL-PASSTHRU-RBP-ERAD, LADDER WREG, rung WREG-0).
#
# WHY THIS GATE EXISTS.  LADDER WREG reserves rΓ=r10 and rΩ=r11 as PRODUCT-WIDE dynamic wire registers: every
# glue kind carries γ in r10 and ω in r11, so a blob needs ZERO receiving code.  A reservation of that shape is
# a GLOBAL fact about all emitted code, but templates are edited by many concurrent seats and NOTHING in the
# build objects when a template grabs r10 or r11 as ordinary scratch.  That is precisely the class that cost
# s6/s7 on r9 (see test_gate_rtcc_claimed_regs.sh's header).  This gate makes the WREG version MECHANICAL, and
# it exists BEFORE the claim goes live so the sweep has a burn-down number instead of an opinion.
#
# RELATIONSHIP TO test_gate_rtcc_claimed_regs.sh — TWO DIFFERENT INVARIANTS, BOTH WANTED:
#   RTCC gate  = COLLISION class.  "Who writes a live-claimed register while also reading through GVARQ?"
#   THIS gate  = SCOPE class.      "Who MENTIONS r10/r11 at all, outside the sites licensed to own the wires?"
# A file can pass the RTCC gate and still fail this one: using r10 as scratch is harmless to GVA and fatal to γ.
#
# ⛔ SUB-REGISTER SPELLINGS ARE PART OF THE CLAIM (WREG-0 finding, 2026-08-10).  LADDER WREG's design measured
# the sweep at 178 sites by counting the quoted full-register spellings "r10" (105) and "r11" (73).  That count
# reproduces EXACTLY at bce9a4b0 — but it is NOT the whole surface.  r10d/r11d/r10b/r11b name the SAME PHYSICAL
# REGISTERS and are invisible to a "r10"-shaped grep.  That blind spot is not theoretical: it is how the ONE
# pre-existing r10 claim in the product hid from the design's census —
#     src/templates/bb_func_activate.cpp:25-26
#       #define AB_TC_REG   ((g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? "r10"  : "r9")
#       #define AB_TC_REG_D ((g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? "r10d" : "r9d")
# — the AB return type-code register, moved r9 -> r10 by the s8 RTCC-safety fix precisely BECAUSE r10 was, in
# that fix's own words, "scratch-tier claimed with NO global assigned".  WREG assigns a global to r10 and thereby
# retires the property that fix depends on.  This gate therefore matches EVERY spelling, and it matches through
# the macro (a name-indirected register is still a use).
#
# WHAT IS CHECKED (textual, deliberately — a dataflow proof is out of scope for a grep gate):  every mention of
# r10/r11 in any spelling, in CODE (comments stripped), under src/templates/ and src/emitter/, minus the
# whitelist.  The whitelist encodes the four site classes LADDER WREG licenses to own the wires:
#   glue emitters · blob-exit emitters · x86_asm.h encoder internals · suspension-capture site.
#
# STATUS TIERS (mirrors the RTCC gate's live/pending discipline — do not skip the middle one):
#   PENDING  (today)      — the claim is DESIGNED and the sweep is UNDONE.  Default run is INFORMATIONAL: it
#                           prints the burn-down and exits 0, so it blocks nobody before WREG-1/2 land.
#   LIVE     (at WREG-2)  — the moment site glue actually carries γ/ω in r10/r11, flip WREG_CLAIM_LIVE=1 below
#                           AND add "r10:GAMMA:WREG-2" + "r11:OMEGA:WREG-2" to LIVE_CLAIMS in
#                           test_gate_rtcc_claimed_regs.sh.  That second edit is what buys WREG the H2 veneer
#                           writeback protection; skipping it re-runs the s6/s7 r9 story on r10.
#
# Usage: bash scripts/test_gate_wreg_claim.sh [--strict]
#   default  : INFORMATIONAL — print the sweep burn-down, exit 0.
#   --strict : HARD GATE — exit 1 if any non-whitelisted site remains.  WREG-0's exit criterion.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
strict=0
[ "${1:-}" = "--strict" ] && strict=1

WHITELIST="scripts/wreg_claim_whitelist.txt"
WREG_CLAIM_LIVE=0

# EVERY spelling of the two claimed registers.  Word-boundary anchored so r10 does not match r100 and the
# quoted/bracketed/macro forms all land: "r10"  [r10 + 8]  "r10d"  "r11b".
REGPAT='\br1[01][dwb]?\b'

# strip_comments FILE — code only, so a comment naming a register does not count as a use.  Same shape as the
# RTCC gate's helper (// line comments then /* */ blocks, non-greedy, across newlines).
strip_comments() { sed -E 's://.*$::' "$1" | perl -0777 -pe 's{/\*.*?\*/}{}gs'; }

wl_has() { [ -f "$WHITELIST" ] && grep -qE "^[[:space:]]*$1[[:space:]]" "$WHITELIST"; }

# ⛔ NEVER `| grep -q` UNDER pipefail (s11 conviction, recorded in test_gate_rtcc_claimed_regs.sh): grep -q exits
# on first match and closes the pipe, the upstream sed|perl takes SIGPIPE(141), pipefail propagates it, and A
# MATCH READS AS A MISS.  grep -c reads to EOF and cannot SIGPIPE the upstream.  Every count below uses grep -c.

echo "=== WREG r10/r11 CLAIM SCOPE GATE ==="
echo "claim status: $([ "$WREG_CLAIM_LIVE" = 1 ] && echo 'LIVE (rGamma=r10, rOmega=r11 carry wires)' || echo 'PENDING (designed, sweep not yet done)')"
echo

total=0; wl_total=0; offenders=0
listed=""; wl_listed=""

for f in src/templates/*.cpp src/templates/*.h src/emitter/*.cpp src/emitter/*.h; do
  [ -e "$f" ] || continue
  n=$(strip_comments "$f" | grep -cE "$REGPAT")
  [ "$n" -gt 0 ] || continue
  b="$(basename "$f")"
  if wl_has "$b"; then
    wl_total=$((wl_total + n)); wl_listed="$wl_listed  LICENSED $b ($n)\n"
  else
    total=$((total + n)); offenders=$((offenders + 1)); listed="$listed  $n\t$b\n"
  fi
done

# ⛔ RTX HAND-WRITTEN ASM IS IN SCOPE — CROSS-GOAL BULLETIN (Lon-directed, .github 37e0273c), verbatim:
# "Any RTX asm that clobbers r10 or r11 silently breaks EVERY pattern blob in flight -- the wires are live
#  across the entire match, and a corrupted rGamma/rOmega is a wild jump, not a wrong answer. ...
#  WREG-0's claim gate must sweep RTX asm sources, not just src/templates/ -- raw-byte and hand-written asm
#  do not grep as \"r10\"."
# This region is ENTIRELY OUTSIDE the design's 178-site census, which counted quoted C-string spellings only.
# It is also the HIGHEST-RISK region in the product for WREG: rtx_match.S runs DURING a match, which is
# exactly when the wires are live. Reported separately so the two burn-downs never get conflated -- a
# template rename and an RTX asm rename are different work with different proofs.
rtx_total=0; rtx_offenders=0; rtx_listed=""
for f in src/runtime/rtx/*.S; do
  [ -e "$f" ] || continue
  n=$(strip_comments "$f" | grep -cE "%?$REGPAT")
  [ "$n" -gt 0 ] || continue
  b="$(basename "$f")"
  if wl_has "$b"; then
    wl_total=$((wl_total + n)); wl_listed="$wl_listed  LICENSED $b ($n)\n"
  else
    rtx_total=$((rtx_total + n)); rtx_offenders=$((rtx_offenders + 1)); rtx_listed="$rtx_listed  $n\t$b\n"
  fi
done

echo "--- LICENSED (whitelisted wire-owning sites) ---"
if [ -n "$wl_listed" ]; then printf "%b" "$wl_listed"; else echo "  (none yet — whitelist is empty until WREG-1 creates the glue emitters)"; fi
echo "  licensed mentions: $wl_total"
echo
echo "--- SWEEP SURFACE (must reach 0 for WREG-0 --strict) ---"
if [ -n "$listed" ]; then printf "%b" "$listed" | sort -rn; else echo "  (clean)"; fi
echo
echo "  files remaining : $offenders"
echo "  mentions remaining: $total"
echo
echo "--- RTX HAND-WRITTEN ASM (bulletin-mandated scope; NOT in the design's 178 census) ---"
if [ -n "$rtx_listed" ]; then printf "%b" "$rtx_listed" | sort -rn; else echo "  (clean)"; fi
echo "  files remaining : $rtx_offenders"
echo "  mentions remaining: $rtx_total"
echo "  ^ rtx_match.S is the sharpest edge: it executes DURING a match, i.e. while the wires are live."
echo

# The design's own census, kept here as a DRIFT CHECK: if this number moves without a sweep commit, some seat
# added r10/r11 scratch and the burn-down silently grew.  Quoted full-register spellings only, which is what
# the design counted -- deliberately the SAME narrow pattern, so the two numbers stay comparable.
quoted=0
for f in src/templates/*.cpp src/templates/*.h src/emitter/*.cpp src/emitter/*.h; do
  [ -e "$f" ] || continue
  n=$(strip_comments "$f" | grep -coE '"r1[01]"')
  quoted=$((quoted + n))
done
echo "  drift check: quoted \"r10\"/\"r11\" = $quoted   (LADDER WREG design measured 178 at bce9a4b0)"
echo "  delta vs all-spellings surface = $((total + wl_total - quoted))  <- the sub-register/bracketed tail the design's census could not see"
echo

if [ "$strict" = 1 ]; then
  if [ $((total + rtx_total)) -gt 0 ]; then
    echo "GATE: FAIL ($total template/emitter + $rtx_total RTX-asm non-whitelisted mentions across $((offenders + rtx_offenders)) files)"; exit 1
  fi
  echo "GATE: PASS"; exit 0
fi
echo "GATE: INFORMATIONAL (exit 0). Run with --strict once the WREG-0 sweep is complete."
exit 0
