#!/bin/bash
# profile_callgrind.sh — Ir attribution for a SCRIP-compiled (mode-4) binary, blob-vs-C split (RUNG SPD-0a).
# Usage: profile_callgrind.sh <prog-binary> <input-file> [outdir]
# Runtime-emitted pattern blobs live in anonymous pages — --smc-check=all-non-file is what counts them.
# Prints: total Ir, top functions, and the blob share (Ir attributed to addresses outside any file-backed object).
set -u
PROG=$1; INP=$2; OUT=${3:-/tmp/cg-$$}; mkdir -p "$OUT"
ulimit -s unlimited
valgrind --tool=callgrind --smc-check=all-non-file --callgrind-out-file="$OUT/cg.out" "$PROG" < "$INP" > "$OUT/prog.out" 2> "$OUT/valgrind.log"
TOTAL=$(callgrind_annotate "$OUT/cg.out" 2>/dev/null | awk '/PROGRAM TOTALS/{gsub(/,/,"",$1); print $1}')
callgrind_annotate "$OUT/cg.out" 2>/dev/null | sed -n '/^-* *Ir/,/^$/p' | head -40 > "$OUT/top.txt"
BLOB=$(callgrind_annotate "$OUT/cg.out" 2>/dev/null | grep -E '\?\?\?|0x0000' | awk '{gsub(/,/,"",$1); s+=$1} END {print s+0}')
echo "TOTAL_Ir=$TOTAL BLOB_Ir=$BLOB BLOB_PCT=$(awk -v b="$BLOB" -v t="$TOTAL" 'BEGIN{if(t>0)printf "%.1f",100*b/t; else print 0}')"
echo "--- top functions ($OUT/top.txt) ---"
cat "$OUT/top.txt"
