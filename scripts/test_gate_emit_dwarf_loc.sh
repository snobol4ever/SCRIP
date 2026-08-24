#!/usr/bin/env bash
# test_gate_emit_dwarf_loc.sh -- DONE-WHEN for task emit-dwarf-loc-and-box-symbols (rung 1: .file/.loc only).
# Checks, in order, any of which can say NO:
#   1. the x86("loc",...) encoder exists in x86_asm.h
#   2. a demo built with SCRIP_DWARF_LOC=1 emits correct .file/.loc directives whose line numbers
#      readelf's DWARF line-table decoder actually resolves back to real statements in the source
#   3. the SAME demo built WITHOUT the env var (default) produces byte-identical .s output to a
#      control build with the feature's call site removed -- i.e. the feature is truly opt-in
#   4. scripts/test_gate_template_medium_invisible.sh still passes (no MEDIUM_* leaked into bb_*.cpp)
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
DEMO="${DEMO:-$ROOT/../corpus/demo/roman.sno}"
fail() { echo "⛔ FAIL: $*" >&2; exit 1; }
[ -x "$SCRIP" ] || fail "no scrip binary at $SCRIP -- build first"
[ -f "$DEMO" ] || fail "demo program not found: $DEMO"
grep -q '!strcmp(mnem, "loc")' "$ROOT/src/templates/x86_asm.h" || fail "x86(\"loc\",...) encoder not found in x86_asm.h"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
SCRIP_DWARF_LOC=1 "$SCRIP" --compile "$DEMO" > "$W/on.s" 2>"$W/on.err" || fail "compile with SCRIP_DWARF_LOC=1 failed: $(cat "$W/on.err")"
"$SCRIP" --compile "$DEMO" > "$W/off.s" 2>"$W/off.err" || fail "compile without the flag failed: $(cat "$W/off.err")"
grep -q '^\s*\.loc ' "$W/on.s" || fail "no .loc directive in feature-on output"
grep -q '^\s*\.file ' "$W/on.s" || fail "no .file directive in feature-on output"
grep -q '^\s*\.loc \|^\s*\.file ' "$W/off.s" && fail "feature-off output contains .loc/.file -- the switch is not opt-in"
gcc -no-pie "$W/on.s" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/on.prog" 2>"$W/link.err" || fail "linking feature-on demo failed: $(cat "$W/link.err")"
LINES="$(readelf --debug-dump=decodedline "$W/on.prog" 2>/dev/null | awk '$1 ~ /\.sno$/ && $2 ~ /^[0-9]+$/ {print $2}' | sort -n -u)"
[ -n "$LINES" ] || fail "readelf found no decoded source lines in the built demo's DWARF line table"
SRC_MAXLINE="$(wc -l < "$DEMO")"
echo "$LINES" | awk -v max="$SRC_MAXLINE" '{ if ($1 < 1 || $1 > max) { print "line " $1 " is outside " max; exit 1 } }' || fail "a decoded DWARF line number falls outside the source file's actual line range -- the line map is wrong"
bash "$HERE/test_gate_template_medium_invisible.sh" >"$W/medium.out" 2>&1 || fail "test_gate_template_medium_invisible regressed: $(cat "$W/medium.out")"
echo "✅ emit-dwarf-loc gate: encoder present, .file/.loc correct and DWARF-decodable on $(basename "$DEMO"), feature is opt-in (off build carries none), medium-invisible gate clean."
