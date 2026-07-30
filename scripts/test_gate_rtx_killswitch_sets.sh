#!/usr/bin/env bash
# test_gate_rtx_killswitch_sets.sh — KILL-SWITCH BYTE-IDENTITY, N>=4 PER ARM, HASH *SETS*.
#
# Minted s219 (2026-07-30).  Implements the correction ARCH-SNOBOL4-RTX.md §7 step 3
# received at s217 and which had no executable form until now.
#
# WHAT WAS WRONG WITH THE GATE AS PRACTICED: it ran ONE run per arm per program and
# compared the two hashes.  The suite contains at least one program that is
# NON-DETERMINISTIC RUN-TO-RUN ON THE PURE-C PATH (160_pat_alt_inner_gen_resume:
# three distinct hashes in four gate-OFF runs, measured s217).  Against such a
# program a single-run comparison reports MOVER or IDENTICAL BY COIN FLIP, so a
# recorded pass is luck rather than evidence — and the failure mode is severe in
# both directions: a later session either reverts a correct port and hunts a bug
# that does not exist, or ships a real leak because the dice fell the other way.
#
# THE DISCRIMINATION IS FREE: gate OFF *is* the C fallback, so instability that is
# visible with the gate OFF cannot have been caused by the asm.  Hence three
# verdicts, not two:
#   IDENTICAL   ON set == OFF set, both singletons  -> real evidence
#   QUARANTINE  either arm produced >1 distinct hash -> unfalsifiable HERE, and
#               that is a fact about the PROGRAM, reported, never silently skipped
#   MOVER       both arms stable and the hashes differ -> the port changed behaviour
#
# A QUARANTINE verdict is NOT a pass and NOT a failure; it is the third thing, and
# printing it is the entire point (same reason 0(f) prints COMMITS rather than a
# pass/fail).  Do NOT "fix" a quarantined program by excluding it wholesale: 160
# both fails in both modes AND is non-deterministic, and a blanket exclusion fuses
# two independent facts.
#
# Usage: bash scripts/test_gate_rtx_killswitch_sets.sh <FAMILY> [glob-dir] [N]
#   FAMILY = the SCRIP_RTX_<FAMILY> gate under test, e.g. MATCH
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FAM="${1:?usage: test_gate_rtx_killswitch_sets.sh <FAMILY> [dir] [N]}"
DIR="${2:-$ROOT/test/snobol4}"
N="${3:-4}"
GATE="SCRIP_RTX_${FAM}"
[ -x "$ROOT/scrip" ] || { echo "FATAL: $ROOT/scrip missing"; exit 1; }
ident=0; quar=0; mover=0; total=0
QLIST=""; MLIST=""
while IFS= read -r prog; do
  total=$((total+1))
  on_set=""; off_set=""
  for arm in 1 0; do
    for i in $(seq 1 "$N"); do
      out="$(cd "$(dirname "$prog")" && env "$GATE=$arm" timeout 30 "$ROOT/scrip" --run "$prog" 2>/dev/null)"; rc=$?
      h="$(printf '%s|rc=%d' "$out" "$rc" | md5sum | cut -c1-8)"
      if [ "$arm" = 1 ]; then on_set="$on_set$h
"; else off_set="$off_set$h
"; fi
    done
  done
  on_u="$(printf '%s' "$on_set" | sort -u | tr '\n' ',')"
  off_u="$(printf '%s' "$off_set" | sort -u | tr '\n' ',')"
  on_n="$(printf '%s' "$on_set" | sort -u | wc -l)"
  off_n="$(printf '%s' "$off_set" | sort -u | wc -l)"
  name="$(basename "$prog" .sno)"
  if [ "$on_n" -gt 1 ] || [ "$off_n" -gt 1 ]; then
    quar=$((quar+1)); QLIST="$QLIST  $name  ON{$on_u} OFF{$off_u}
"
  elif [ "$on_u" = "$off_u" ]; then
    ident=$((ident+1))
  else
    mover=$((mover+1)); MLIST="$MLIST  $name  ON{$on_u} OFF{$off_u}
"
  fi
done < <(find "$DIR" -name '*.sno' | sort)
echo "=== KILL-SWITCH HASH-SET GATE — $GATE, N=$N per arm, $total programs ==="
echo "  IDENTICAL  = $ident"
echo "  QUARANTINE = $quar   (non-deterministic in at least one arm; unfalsifiable here)"
echo "  MOVER      = $mover"
[ -n "$QLIST" ] && { echo "--- QUARANTINE LIST ---"; printf '%s' "$QLIST"; }
[ -n "$MLIST" ] && { echo "--- MOVERS (gate violation) ---"; printf '%s' "$MLIST"; }
[ "$mover" -eq 0 ] && { echo "GATE PASS (zero movers; quarantine reported, not hidden)"; exit 0; }
echo "GATE FAIL ($mover mover(s))"; exit 1
