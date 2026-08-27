#!/bin/bash
# corpus coverage denominator -- a COMMAND, never a constant (hq_C s272 SS1). Recomputes total-minus-exclusions
# on the live tree every run and prints its own inputs -- do not pin this script's output into a doc or goal file.
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
CORPUS="$S4E/corpus"
[ -d "$CORPUS" ] || { echo "⛔ REFUSES: corpus not found at $CORPUS"; exit 2; }

count() {
    find "$1" -type f \( -name '*.sno' -o -name '*.icn' -o -name '*.pl' -o -name '*.sc' \
        -o -name '*.reb' -o -name '*.raku' -o -name '*.pas' \) 2>/dev/null | wc -l
}

TOTAL=$(count "$CORPUS")
echo "corpus coverage denominator -- recomputed live, $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "root: $CORPUS"
echo "total program files (.sno .icn .pl .sc .reb .raku .pas): $TOTAL"
echo

EXCLUDED=0
report_exclusion() {
    local label="$1" path="$2"
    if [ -d "$path" ]; then
        local n; n=$(count "$path")
        EXCLUDED=$((EXCLUDED + n))
        echo "  EXCLUDE $label: $n files at $path"
    else
        echo "  EXCLUDE $label: NOT FOUND at $path -- category absent from the current tree, excluding 0; this may mean the category was retired or renamed in a reorg, not that nothing needs excluding -- verify with hq_C before trusting this line"
    fi
}

report_exclusion "probe (witness ladders)"    "$CORPUS/probe"
report_exclusion "Icon IPL reference archive" "$CORPUS/packages/icon/ipl"
report_exclusion "jcon-ref (JCON BB-gen reference, was mis-transcribed as jcon-compiler/ref)" "$CORPUS/packages/icon/jcon-ref"
# oracle-unrunnable: RETIRED (Lon s269, corpus 53831d2e8) -- category deleted, not reported as NOT FOUND.
# Ruled by hq_P 2026-08-27 (settled via git log --diff-filter=A, not find): jcon-compiler/ref never existed
# under that name; the material it meant is jcon-ref, continuously present since 6df19d6ad under 3 names.

echo
echo "DENOMINATOR = $TOTAL - $EXCLUDED = $((TOTAL - EXCLUDED))"
