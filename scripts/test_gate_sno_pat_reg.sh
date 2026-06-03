#!/usr/bin/env bash
# test_gate_sno_pat_reg.sh — SNOBOL4 PATTERN-FAMILY REGISTER-CONFORMANCE gate (REG ladder, GOAL-SNOBOL4-BB.md).
#
# The SNOBOL pattern boxes must run on the ratified subject model — Σ=R13 (subject base), δ=R14 (cursor),
# Δ=R15 (length), ζ=R12 (frame) — and must NOT bake the emitter-process global addresses &Σ / &Σlen
# (TEMPLATE_ADDR_SIGMA / TEMPLATE_ADDR_SIGLEN), which are mode-3-in-process-only and block mode-4 relocation.
#
# TWO TIERS (mirrors test_gate_no_vstack.sh / test_gate_no_handencoded_bytes.sh house style):
#   TIER 1 — TEMPLATE_ADDR_SIG* : HARD. Already at ZERO family-wide (the &Σ/&Σlen bake removal is DONE).
#            --strict exits 1 if any reappears. This is the convention-compliance invariant the gate LOCKS.
#   TIER 2 — r10 residue        : INFORMATIONAL until REG-RO completes. The cursor is r14, but the pattern
#            family still carries r10 in two dead-but-present forms: the [r10] cursor-MIRROR writes in bb_lit
#            (still read by the SHARED xa_flat non-frame epilogue — see GOAL REG-RO/REG-LADDER notes) and the
#            push r10 / pop r10 guards around memcmp/strchr. REG-RO removes both (and migrates xa_flat's
#            `movsxd rcx, dword ptr [r10]` cursor read to r14) — at which point this tier flips to HARD.
#
# Usage: bash scripts/test_gate_sno_pat_reg.sh [--strict]
#   (no --strict) : report both tiers; exit 0 unless TIER 1 is non-zero.
#   --strict      : TIER 1 must be 0 (enforced); TIER 2 still informational until REG-RO lands (see NOTE below).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
strict=0
[ "${1:-}" = "--strict" ] && strict=1

# The SNOBOL pattern family: all bb_pat_*.cpp leaves/combinators + the literal matcher bb_lit.cpp. (bb_capture
# and bb_arbno no longer exist — capture folded away, ARBNO is bb_pat_arb — so they are not listed; a glob is
# used so the set tracks the directory rather than a stale hand-list.)
FAMILY=$(ls src/emitter/BB_templates/bb_pat_*.cpp src/emitter/BB_templates/bb_lit.cpp 2>/dev/null)

# strip_comments FILE — code only (so a comment naming a token does not count as a live reference).
strip_comments() { sed -E 's://.*$::' "$1" | perl -0777 -pe 's{/\*.*?\*/}{}gs'; }

# ---- TIER 1: TEMPLATE_ADDR_SIG* (HARD) ----
sig_total=0
sig_report=""
for f in $FAMILY; do
  h=$(strip_comments "$f" | grep -cE 'TEMPLATE_ADDR_SIGMA|TEMPLATE_ADDR_SIGLEN')
  if [ "$h" -gt 0 ]; then sig_total=$((sig_total + h)); sig_report="${sig_report}  ${h}\t${f}\n"; fi
done

# ---- TIER 2: r10 residue (INFORMATIONAL until REG-RO) ----
r10_total=0
r10_report=""
for f in $FAMILY; do
  h=$(strip_comments "$f" | grep -cE '\br10\b')
  if [ "$h" -gt 0 ]; then r10_total=$((r10_total + h)); r10_report="${r10_report}  ${h}\t${f}\n"; fi
done

echo "=== REG-FENCE: SNOBOL4 pattern-family register conformance (code refs, comments stripped) ==="
echo ""
echo "TIER 1 (HARD) — TEMPLATE_ADDR_SIGMA / TEMPLATE_ADDR_SIGLEN bake: ${sig_total}  (MUST be 0)"
if [ "$sig_total" -gt 0 ]; then printf "%b" "$sig_report" | sort -rn; fi
echo ""
echo "TIER 2 (informational until REG-RO) — r10 references: ${r10_total}"
if [ "$r10_total" -gt 0 ]; then printf "%b" "$r10_report" | sort -rn; fi
echo ""

if [ "$sig_total" -ne 0 ]; then
  echo "FAIL (TIER 1): a SNOBOL pattern box re-baked &Σ/&Σlen — that is the mode-4 relocation blocker; use r13/r15."
  exit 1
fi

if [ "$strict" -eq 1 ]; then
  # NOTE: REG-RO is not yet complete; TIER 2 stays informational even under --strict until xa_flat's [r10]
  # cursor read is migrated to r14 and the bb_lit mirror writes + memcmp/strchr push/pop r10 guards are dropped.
  # When REG-RO lands, change the next line to enforce r10_total == 0.
  echo "OK (TIER 1 strict): zero &Σ/&Σlen bake in the SNOBOL pattern family. TIER 2 (r10=${r10_total}) informational pending REG-RO."
else
  echo "OK (TIER 1): zero &Σ/&Σlen bake in the SNOBOL pattern family."
fi
exit 0
