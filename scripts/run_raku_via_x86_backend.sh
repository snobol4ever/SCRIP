#!/usr/bin/env bash
# run_raku_via_x86_backend.sh — RAKU-BB RK-BB-1
# Compile a .raku file via `scrip --compile --target=x86`, assemble, link
# against libscrip_rt.so, run the native binary, echo its stdout. Mirrors
# run_prolog_via_x86_backend.sh.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$SCRIP/scrip}"
LIBRT="${LIBRT:-$SCRIP/out/libscrip_rt.so}"
RK="${1:?Usage: run_raku_via_x86_backend.sh <file.raku>}"
[ -x "$SCRIP" ] || { echo "FAIL scrip not built: $SCRIP"; exit 1; }
[ -f "$LIBRT" ] || { echo "FAIL libscrip_rt.so not built: $LIBRT (run 'make libscrip_rt' first)"; exit 1; }
[ -f "$RK"    ] || { echo "FAIL no such file: $RK"; exit 1; }
WORK="$(mktemp -d /tmp/rk_x86_XXXXXX)"
trap 'rm -rf "$WORK"' EXIT
( cd "$WORK" && timeout 8 "$SCRIP" --compile --target=x86 "$RK" > "$WORK/prog.s" 2>"$WORK/scrip.err" < /dev/null ) || {
    echo "FAIL scrip emit failed:"; cat "$WORK/scrip.err"; exit 1
}
as --64 -o "$WORK/prog.o" "$WORK/prog.s" 2>"$WORK/as.err" || {
    echo "FAIL assembler:"; head -20 "$WORK/as.err"; exit 1
}
gcc -no-pie -o "$WORK/prog_bin" "$WORK/prog.o" "$LIBRT" -lgc -lm -lstdc++ -Wl,-rpath,"$(dirname "$LIBRT")" 2>"$WORK/ld.err" || {
    echo "FAIL link:"; head -20 "$WORK/ld.err"; exit 1
}
timeout 8 "$WORK/prog_bin" < /dev/null 2>&1
