#!/usr/bin/env bash
# util_regen_prolog_bench_s_artifacts.sh — regenerate the mode-4 (--compile x86)
# .s artifact beside every benchmark .pl in corpus/benchmarks/prolog/bench.
#
# Mirrors util_regen_benchmark_s_artifacts.sh (SNOBOL4). Per RULES.md the .s is
# the HONEST CURRENT compiler output, never a pinned golden:
#   - a program that compiles to real x86 -> its .s is (re)written and, if the
#     bytes changed, committed.
#   - a program still FENCED by pl_gz_admit (no codegen yet) -> NO .s; instead a
#     one-line <name>.s.FENCED marker records the frontier honestly. When the
#     ladder admits that shape, the regen replaces the marker with a real .s.
#   - an assembler-rejected .s is left flagged, not silently committed.
# scrip --compile is deterministic, so an unchanged compiler yields no change.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
B="${BENCH_DIR:-$S4E/corpus/benchmarks/prolog/bench}"
RUNG="${1:-prolog-bench-regen}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$B" ] || { echo "SKIP bench corpus missing: $B"; exit 0; }
changed=0; fenced=0; rejected=0; emitted=0; timedout=0; errored=0
for pl in "$B"/*.pl; do
  s="${pl%.pl}"; base=$(basename "$s"); tmp="$(mktemp)"
  # Capture via REDIRECT-TO-TEMP (not command-substitution): a shell pipe `$(...)` can truncate bursty multi-KB asm
  # under load — the documented transient empty-emit. Redirecting straight to a file is the robust sibling pattern
  # (util_regen_benchmark/feature/demo all do this) and removes the flap at the source.
  timeout 30 "$SCRIP" --compile --target=x86 "$pl" </dev/null >"$tmp" 2>/tmp/regen_cerr.$$; rc=$?
  if grep -qE '^\s*\.(intel_syntax|text|globl)' "$tmp"; then
    emitted=$((emitted+1))
    if as --64 -o /dev/null "$tmp" 2>/tmp/regen_aserr.$$; then
      if [ ! -f "$s.s" ] || ! cmp -s "$tmp" "$s.s"; then mv "$tmp" "$s.s"; chmod 644 "$s.s"; changed=$((changed+1)); echo "  WROTE $base.s"; else rm -f "$tmp"; fi
      rm -f "$s.s.FENCED"
    else
      rm -f "$tmp"; rejected=$((rejected+1)); echo "  REJECTED-BY-AS $base (left untouched)"
    fi
  elif grep -q '\[PL-GZ FENCE\]' /tmp/regen_cerr.$$; then
    # GENUINE fence: the compiler RAN and pl_gz_admit rejected the shape (positive evidence in stderr). Only HERE is the
    # destructive delete of an existing .s legitimate — replace it with the honest FENCED marker.
    rm -f "$tmp"; fenced=$((fenced+1)); rm -f "$s.s"
    printf 'FENCED by pl_gz_admit — no codegen yet (%s)\n' "$(grep -oE 'pl_gz_admit[^"]*' /tmp/regen_cerr.$$ | head -1 | cut -c1-70)" > "$s.s.FENCED"
    echo "  FENCED $base -> $base.s.FENCED"
  elif [ "$rc" -eq 124 ]; then
    # TIMEOUT (the destructive-delete bug this guard fixes): the compile was KILLED, not fenced. NEVER delete the .s on a
    # timeout — a flaky 30s contention spike must not erase a good artifact. Leave it untouched and flag loudly.
    rm -f "$tmp"; timedout=$((timedout+1)); echo "  TIMEOUT $base (compile exceeded 30s — .s LEFT UNTOUCHED, NOT fenced)"
  else
    # No asm, no fence marker, no timeout = a crash/error. Also non-destructive: preserve the .s and flag for a human.
    rm -f "$tmp"; errored=$((errored+1)); echo "  ERROR $base (no asm + no fence signal, rc=$rc — .s LEFT UNTOUCHED)"
  fi
done
rm -f /tmp/regen_cerr.$$ /tmp/regen_aserr.$$
echo "regen[$RUNG]: emitted=$emitted changed=$changed fenced=$fenced rejected=$rejected timedout=$timedout errored=$errored"
if [ "$timedout" -ne 0 ] || [ "$errored" -ne 0 ]; then
  echo "WARNING: $timedout timed out + $errored errored — their .s were PRESERVED (re-run, or compile standalone). A timeout is NOT a fence."
fi
