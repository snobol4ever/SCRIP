#!/usr/bin/env bash
# run_prolog_via_x86_backend.sh — compile + link + run a .pl file via scrip --compile --target=x86
# Usage: bash scripts/run_prolog_via_x86_backend.sh <file.pl>
# Built for PJ-9d (predicate-registry emit) — feeds rt_register_predicates_pl + rt_pl_once (post IJ-HELLO-4).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
LIBRT="${LIBRT:-$ROOT/out/libscrip_rt.so}"
PL="${1:?Usage: run_prolog_via_x86_backend.sh <file.pl>}"
[ -x "$SCRIP" ] || { echo "FAIL scrip not built: $SCRIP"; exit 1; }
[ -f "$LIBRT" ] || { echo "FAIL libscrip_rt.so not built: $LIBRT (run 'make libscrip_rt' first)"; exit 1; }
[ -f "$PL"    ] || { echo "FAIL no such file: $PL"; exit 1; }
WORK="$(mktemp -d /tmp/pl_x86_XXXXXX)"
trap 'rm -rf "$WORK"' EXIT
PLABS="$(realpath "$PL")"
PLBASE="$(basename "$PL" .pl)"
ASM="$WORK/$PLBASE.s"
# CORPUS-S-HYGIENE (Lon 2026-06-04): .s + bb_macros.s land in the mktemp WORK dir, never next to the source.
# Tracked corpus *.s are frozen DEMO artifacts only (roman, wordcount, claws5, treebank, ...).
( cd "$WORK" && timeout 8 "$SCRIP" --compile --target=x86 "$PLABS" > "$ASM" 2>"$WORK/scrip.err" < /dev/null ) || {
    echo "FAIL scrip emit failed:"; cat "$WORK/scrip.err"; exit 1
}
( cd "$WORK" && as --64 -o "$WORK/prog.o" "$ASM" ) 2>"$WORK/as.err" || {
    echo "FAIL assembler:"; cat "$WORK/as.err" | head -20; exit 1
}
gcc -no-pie -o "$WORK/prog_bin" "$WORK/prog.o" "$LIBRT" -lm -lstdc++ -Wl,-rpath,"$(dirname "$LIBRT")" 2>"$WORK/ld.err" || {
    echo "FAIL link:"; cat "$WORK/ld.err" | head -20; exit 1
}
timeout 8 "$WORK/prog_bin" < /dev/null
