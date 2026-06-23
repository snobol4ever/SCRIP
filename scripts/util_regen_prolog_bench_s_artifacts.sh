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
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
B="${BENCH_DIR:-/home/claude/corpus/benchmarks/prolog/bench}"
RUNG="${1:-prolog-bench-regen}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$B" ] || { echo "SKIP bench corpus missing: $B"; exit 0; }
changed=0; fenced=0; rejected=0; emitted=0
for pl in "$B"/*.pl; do
  s="${pl%.pl}"; base=$(basename "$s")
  asm=$(timeout 12 "$SCRIP" --compile --target=x86 "$pl" </dev/null 2>/tmp/regen_cerr.$$)
  if echo "$asm" | grep -qE '^\s*\.(intel_syntax|text|globl)'; then
    emitted=$((emitted+1))
    tmp="$(mktemp)"; printf '%s\n' "$asm" > "$tmp"
    if as --64 -o /dev/null "$tmp" 2>/tmp/regen_aserr.$$; then
      if [ ! -f "$s.s" ] || ! cmp -s "$tmp" "$s.s"; then mv "$tmp" "$s.s"; chmod 644 "$s.s"; changed=$((changed+1)); echo "  WROTE $base.s"; else rm -f "$tmp"; fi
      rm -f "$s.s.FENCED"
    else
      rm -f "$tmp"; rejected=$((rejected+1)); echo "  REJECTED-BY-AS $base (left untouched)"
    fi
  else
    fenced=$((fenced+1)); rm -f "$s.s"
    printf 'FENCED by pl_gz_admit — no codegen yet (%s)\n' "$(grep -oE 'pl_gz_admit[^"]*' /tmp/regen_cerr.$$ | head -1 | cut -c1-70)" > "$s.s.FENCED"
    echo "  FENCED $base -> $base.s.FENCED"
  fi
done
rm -f /tmp/regen_cerr.$$ /tmp/regen_aserr.$$
echo "regen[$RUNG]: emitted=$emitted changed=$changed fenced=$fenced rejected=$rejected"
