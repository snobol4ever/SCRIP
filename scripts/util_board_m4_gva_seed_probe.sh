#!/bin/bash
# BOARD B-0 instrument: measures how much of the m4 collapse is the missing r9/GVA seed.
# Patches ONLY the generated .s in a scratch dir. Zero compiler bytes.
set -u
S=/home/claude/SCRIP/scrip; RTOUT=/home/claude/SCRIP/out
DIR=/home/claude/corpus/probe/bb/probes; W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
FIX=0; STILLBAD=0; NOSEED=0; PRE=0
for f in "$DIR"/*.sno; do
  id=$(basename "$f" .sno); ref="$DIR/$id.ref"; [ -f "$ref" ] || continue
  timeout 60s "$S" --compile "$f" </dev/null > "$W/p.s" 2>/dev/null || { STILLBAD=$((STILLBAD+1)); continue; }
  [ -s "$W/p.s" ] || { STILLBAD=$((STILLBAD+1)); continue; }
  # baseline (unpatched)
  gcc "$W/p.s" -no-pie -L "$RTOUT" -lscrip_rt -Wl,-rpath,"$RTOUT" -o "$W/base" 2>/dev/null \
    && timeout 15s env LD_LIBRARY_PATH="$RTOUT" "$W/base" </dev/null > "$W/gb" 2>/dev/null
  cmp -s "$W/gb" "$ref" 2>/dev/null && { PRE=$((PRE+1)); continue; }
  # patched
  if grep -q "xor              esi, esi" "$W/p.s"; then
    python3 - "$W/p.s" <<'PY'
import sys
p=sys.argv[1]; s=open(p).read()
o="                        xor              esi, esi\n"
n="                        mov              edi, 1\n                        call             rt_gva_island@PLT\n                        mov              r9, rax\n"+o
open(p,'w').write(s.replace(o,n,1))
PY
  else NOSEED=$((NOSEED+1)); fi
  gcc "$W/p.s" -no-pie -L "$RTOUT" -lscrip_rt -Wl,-rpath,"$RTOUT" -o "$W/fx" 2>/dev/null \
    && timeout 15s env LD_LIBRARY_PATH="$RTOUT" "$W/fx" </dev/null > "$W/gf" 2>/dev/null
  if cmp -s "$W/gf" "$ref" 2>/dev/null; then FIX=$((FIX+1)); echo "  RECOVERED $id"; else STILLBAD=$((STILLBAD+1)); fi
done
echo "-----------------------------------------------"
echo "  already-passing(m4): $PRE"
echo "  RECOVERED by r9 seed: $FIX"
echo "  still failing:        $STILLBAD"
echo "  no seed point found:  $NOSEED"
