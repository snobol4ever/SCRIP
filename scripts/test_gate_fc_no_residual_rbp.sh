#!/usr/bin/env bash
# ZB-VAL-8b GATE (s182, closes s181 FINDING-2026-07-26f HEADLINE 6 "THE FALLBACK IS SILENT").
#
# THE HAZARD: x86_fc_hit(off) returns false when a GRANTED box's flat offset falls outside its own
# fc window, and FR()/FRQ() then QUIETLY emit [rbp+off] instead of [rsp+off-base].  An undersized
# window does not crash and does not emit a wrong address -- it just leaves the box on rbp.  So
# "I converted it" and "it converted" are different facts and nothing in the build distinguished
# them.  Given per-graph flat layout (lt1 and lt2 allocate the SAME construct at different offsets),
# mis-sizing is near-certain at least once as the conversion widens.
#
# WHAT THIS ASSERTS: compile the corpus with SCRIP_FC_AUDIT=1 and require ZERO [FC-MISS] events.
# A miss is emitted by x86_fc_hit exactly when the box IS granted (w>0, base>=0) but the offset
# misses the window -- i.e. precisely the silent fallback, not merely "an rbp reference exists"
# (ungranted boxes legitimately speak rbp; grepping the .s for '[rbp+' cannot tell the two apart,
# which is why this gate reads the COMPILER's own verdict instead of the emitted text).
#
# BASELINE AT LANDING (s182): 0 misses across 52 demo+feat programs / 643 graphs.
cd "$(dirname "$0")/.." || exit 2
SCRIP=${SCRIP:-./scrip}
CORPUS=${CORPUS:-/home/claude/corpus/programs/snobol4}
[ -x "$SCRIP" ] || { echo "GATE FAIL: no $SCRIP (run: make scrip)"; exit 2; }
total=0; n=0; offenders=""
while IFS= read -r f; do
  n=$((n+1))
  m=$(SCRIP_FC_AUDIT=1 timeout 20 "$SCRIP" --compile "$f" 2>&1 >/dev/null | grep -c '\[FC-MISS\]')
  if [ "$m" -gt 0 ]; then total=$((total+m)); offenders="$offenders $(basename "$f")($m)"; fi
done < <(find "$CORPUS/demo" "$CORPUS/feat" -name '*.sno' 2>/dev/null)
echo "FC silent-fallback events across $n program(s): $total  (MUST be 0)"
if [ "$total" -ne 0 ]; then
  echo "OFFENDERS:$offenders"
  echo "Re-run one with SCRIP_FC_AUDIT=1 to see each window: SCRIP_FC_AUDIT=1 $SCRIP --compile <prog>.sno >/dev/null"
  echo "GATE FAIL."
  exit 1
fi
echo "OK: every granted box addresses inside its own fc window (zero residual-rbp fallbacks)."
