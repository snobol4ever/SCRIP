#!/usr/bin/env bash
# prove_lower2.sh — topology proof gate for the ground-zero unified lowerer (lower2.c).
# Compiles lower2.c + scrip_ir.c + prove_lower2.c (standalone, NOT the production lower.c) and runs the
# harness, which lowers Proebsting's Figure-1 ASTs and dumps each IR node's α/β/γ/ω ports. Idempotent.
set -euo pipefail
SCRIP="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$SCRIP"
INC="-Isrc -Isrc/include -Isrc/contracts -Isrc/lower -Isrc/machine -Isrc/emitter -Isrc/runtime/core -Isrc/runtime"
OUT=/tmp/prove_lower2
gcc -O0 -g $INC -DDYN_ENGINE_LINKED -c src/lower/lower.c     -o /tmp/p_lower2.o
gcc -O0 -g $INC -DDYN_ENGINE_LINKED                   -c src/contracts/scrip_ir.c  -o /tmp/p_scrip_ir.o
gcc -O0 -g $INC                     -c src/lower/prove_lower2.c -o /tmp/p_prove.o
gcc /tmp/p_lower2.o /tmp/p_scrip_ir.o /tmp/p_prove.o -lgc -lm -o "$OUT"
"$OUT"
