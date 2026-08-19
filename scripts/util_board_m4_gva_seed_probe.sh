#!/bin/bash
# util_board_m4_gva_seed_probe.sh — BOARD instrument (FINDING-2026-08-12c).
#
# WHAT IT MEASURES: how much of the mode-4 collapse is the MISSING r9/GVA SEED.
# The m4 preamble (src/driver/scrip.c:1473 SNOBOL4, :1265 Icon) calls rt_gva_island,
# hands the base to gva_register in RSI, and never lands it in r9 — so every global
# store hits [r9+0] with r9=0 until the first RTCC reload (mov r9,[r11+48]) happens
# to run.  Programs whose first global access precedes that reload die.
#
# HOW: compiles each probe, patches ONLY a scratch copy of the generated .s to seed
# r9, and compares against the pinned .ref.  ZERO compiler bytes — BOARD charter.
#
# ⛔ ANCHOR ON "call gva_register@PLT", WHICH IS UNIQUE PER PROGRAM.
#    "xor esi, esi" is NOT unique (4 occurrences in a typical earn0 program); anchoring
#    there patches a procedure body instead of main's preamble and SILENTLY UNDERCOUNTS.
#    That flaw produced a false 0/12 on earn0 before it was caught (BOARD s33).
#    A probe with no gva_register (no user globals) cannot be an r9 victim: counted NOSEED.
#
# USAGE:  bash util_board_m4_gva_seed_probe.sh [DIR]
#         DIR defaults to the 163-probe bb suite.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
S="${SCRIP:-$S4E/SCRIP/scrip}"
RTOUT="$(dirname "$S")/out"
DIR="${1:-$S4E/corpus/probe/bb/probes}"
[ -x "$S" ]   || { echo "no scrip at $S"; exit 2; }
[ -d "$DIR" ] || { echo "no dir $DIR"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT

cat > "$W/seed.py" <<'PYEOF'
import sys
p = sys.argv[1]; s = open(p).read()
a = "                        call             gva_register@PLT\n"
if s.count(a) != 1: sys.exit(2)
n = a + "                        mov              edi, 1\n                        call             rt_gva_island@PLT\n                        mov              r9, rax\n"
open(p, 'w').write(s.replace(a, n, 1))
PYEOF

PRE=0; FIX=0; STILLBAD=0; NOSEED=0
for f in "$DIR"/*.sno; do
    id="$(basename "$f" .sno)"; ref="$DIR/$id.ref"; [ -f "$ref" ] || continue
    timeout 60s "$S" --compile "$f" </dev/null > "$W/p.s" 2>/dev/null
    [ -s "$W/p.s" ] || { STILLBAD=$((STILLBAD+1)); echo "  COMPILE-FAIL $id"; continue; }
    gcc "$W/p.s" -no-pie -L "$RTOUT" -lscrip_rt -Wl,-rpath,"$RTOUT" -o "$W/base" 2>/dev/null \
        && timeout 15s env LD_LIBRARY_PATH="$RTOUT" "$W/base" </dev/null > "$W/gb" 2>/dev/null
    if cmp -s "$W/gb" "$ref" 2>/dev/null; then PRE=$((PRE+1)); continue; fi
    if ! python3 "$W/seed.py" "$W/p.s" 2>/dev/null; then
        NOSEED=$((NOSEED+1)); STILLBAD=$((STILLBAD+1)); continue
    fi
    gcc "$W/p.s" -no-pie -L "$RTOUT" -lscrip_rt -Wl,-rpath,"$RTOUT" -o "$W/fx" 2>/dev/null \
        && timeout 15s env LD_LIBRARY_PATH="$RTOUT" "$W/fx" </dev/null > "$W/gf" 2>/dev/null
    if cmp -s "$W/gf" "$ref" 2>/dev/null; then FIX=$((FIX+1)); echo "  RECOVERED $id"
    else STILLBAD=$((STILLBAD+1)); fi
done
echo "-------------------------------------------------------------------"
echo "  dir:                  $DIR"
echo "  already-passing (m4): $PRE"
echo "  RECOVERED by r9 seed: $FIX"
echo "  still failing:        $STILLBAD   (of which no-gva-preamble: $NOSEED)"
echo "-------------------------------------------------------------------"
