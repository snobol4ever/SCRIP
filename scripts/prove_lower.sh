#!/usr/bin/env bash
# prove_lower.sh — topology proof gate for the ground-zero unified lowerer (lower.c).
# Compiles lower.c + scrip_ir.c + prove_lower.c (standalone, NOT the production lower.c) and runs the
# harness, which lowers Proebsting's Figure-1 ASTs and dumps each IR node's α/β/γ/ω ports. Idempotent.
set -euo pipefail
SCRIP="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$SCRIP"
INC="-Isrc -Isrc/include -Isrc/contracts -Isrc/lower -Isrc/machine -Isrc/emitter -Isrc/interp -Isrc/runtime/core -Isrc/runtime"
OUT=/tmp/prove_lower
gcc -O0 -g -ffunction-sections -fdata-sections $INC -DDYN_ENGINE_LINKED -c src/lower/lower_icon.c -o /tmp/p_lower_icon.o
gcc -O0 -g -ffunction-sections -fdata-sections $INC -DDYN_ENGINE_LINKED -c src/lower/lower_raku.c -o /tmp/p_lower_raku.o
gcc -O0 -g -ffunction-sections -fdata-sections $INC -DDYN_ENGINE_LINKED -c src/lower/lower_pascal.c -o /tmp/p_lower_pascal.o
gcc -O0 -g -ffunction-sections -fdata-sections $INC -DDYN_ENGINE_LINKED -c src/lower/lower_common.c -o /tmp/p_lower_common.o
gcc -O0 -g $INC -DDYN_ENGINE_LINKED                   -c src/contracts/scrip_ir.c  -o /tmp/p_scrip_ir.o
gcc -O0 -g -ffunction-sections -fdata-sections $INC                     -c src/tools/prove_lower.c -o /tmp/p_prove.o
gcc /tmp/p_lower_icon.o /tmp/p_lower_raku.o /tmp/p_lower_pascal.o /tmp/p_lower_common.o /tmp/p_scrip_ir.o /tmp/p_prove.o -Wl,--gc-sections -lgc -lm -o "$OUT"
"$OUT"
