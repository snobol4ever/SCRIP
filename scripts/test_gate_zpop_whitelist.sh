#!/usr/bin/env bash
# BP-9 (ii) trivial-β whitelist gate: emits the 16-bench snobol4 corpus fresh and verifies every
# driver-whitelisted op's β body is exactly [add rsp,K]* jmp (analyze_jump_redundancy.py --gate).
# Backs flat_trivial_beta (src/emitter/emit.cpp); run after any match-template or hook edit.
set -u
HERE="$(cd "$(dirname "$0")/.." && pwd)"
SCRIP="$HERE/scrip"
SRC="${1:-/home/claude/corpus/benchmarks/snobol4}"
W="$(mktemp -d)"
trap 'rm -rf "$W"' EXIT
fail=0
for sno in "$SRC"/*.sno; do
  b="$(basename "$sno" .sno)"
  timeout 30 "$SCRIP" --compile "$sno" > "$W/$b.s" 2>/dev/null </dev/null || { echo "EMIT FAIL: $b"; fail=1; }
done
python3 "$HERE/scripts/analyze_jump_redundancy.py" --gate "$W/*.s" || fail=1
exit $fail
