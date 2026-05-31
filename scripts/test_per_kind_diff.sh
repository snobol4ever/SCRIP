#!/usr/bin/env bash
# scripts/test_per_kind_diff.sh — EC-UNI-PER-KIND-DIFF regression gate.
#
# Runs ./scrip --audit-per-kind in a scratch dir, normalizes each cell, and
# diffs against the committed baseline at baselines/per_kind/.
#
# Output: a kind × backend PASS/FAIL/STUB/NEW table.
#   PASS — normalized output matches baseline.norm exactly.
#   FAIL — differs; the diff is regression.  Exit 1.
#   STUB — both sides are 0-byte (honest no-op stub for this cell).  PASS.
#   NEW  — no baseline entry for this cell (a kind was added since freeze).
#          Treated as informational, NOT fail (run freeze to refresh).
#   GONE — baseline has a cell that the current audit didn't produce.  FAIL.
#
# Per RULES.md: self-contained, no other gates' artifacts assumed.
#
# Usage:  bash scripts/test_per_kind_diff.sh
set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
BASE="$ROOT/baselines/per_kind"
RUN="/tmp/audit_diff_$$"
NORMALIZER="$HERE/normalize_per_kind_cell.py"

if [ ! -x "$ROOT/scrip" ]; then
    echo "diff: $ROOT/scrip not built — run 'make -j4 scrip' first" >&2
    exit 1
fi
if [ ! -d "$BASE" ]; then
    echo "diff: baseline missing at $BASE; run freeze_per_kind_baseline.sh first" >&2
    exit 1
fi
chmod +x "$NORMALIZER" 2>/dev/null || true

mkdir -p "$RUN"
"$ROOT/scrip" --audit-per-kind "$RUN" > /dev/null

# Tally counters
pass=0; fail=0; stub=0; new=0; gone=0
fail_lines=()
new_lines=()
gone_lines=()

# Pass 1 — every current cell vs baseline.
for raw in $(find "$RUN" -type f | sort); do
    rel="${raw#$RUN/}"                # x86/text/BB_PAT_LIT.s
    name="${rel##*/}"                 # BB_PAT_LIT.s
    dir="${rel%/*}"                   # x86/text  (backend/submode)
    ext="${name##*.}"

    base_norm="$BASE/$dir/$name.norm"
    if [ ! -f "$base_norm" ]; then
        new=$((new + 1))
        new_lines+=("NEW  $dir/$name")
        continue
    fi

    # x86/binary: BITWISE MASKED COMPARE.  Both the committed baseline.norm and
    # this freshly-normalized cell pass through normalize_per_kind_cell.py's
    # mask_bin, which zeros build-variant address operands (movabs imm64 + large
    # mov r32,imm32) over opcode-driven (offset,length) spans.  After masking,
    # every remaining byte is build-stable, so a bit-identical cmp is correct AND
    # catches real regressions in non-masked bytes (was: length-only structural
    # compare, which was blind to same-length byte changes).
    cur_tmp="/tmp/audit_cur_$$_$RANDOM"
    "$NORMALIZER" --ext "$ext" "$raw" > "$cur_tmp" 2>/dev/null

    base_sz="$(stat -c '%s' "$base_norm" 2>/dev/null || echo 0)"
    cur_sz="$(stat -c '%s' "$cur_tmp" 2>/dev/null || echo 0)"

    if [ "$base_sz" -eq 0 ] && [ "$cur_sz" -eq 0 ]; then
        stub=$((stub + 1))
    elif cmp -s "$cur_tmp" "$base_norm"; then
        pass=$((pass + 1))
    else
        fail=$((fail + 1))
        fail_lines+=("FAIL $dir/$name")
    fi
    rm -f "$cur_tmp"
done

# Pass 2 — baseline cells that didn't appear in the current run.
for base_norm in $(find "$BASE" -name '*.norm' -type f | sort); do
    rel="${base_norm#$BASE/}"        # x86/text/BB_PAT_LIT.s.norm
    name="${rel##*/}"
    name="${name%.norm}"             # BB_PAT_LIT.s
    # rel without the trailing /name.norm  →  x86/text
    dir="${rel%/*}"
    if [ ! -f "$RUN/$dir/$name" ]; then
        gone=$((gone + 1))
        gone_lines+=("GONE $dir/$name")
    fi
done

echo "=== EC-UNI-PER-KIND-DIFF result ==="
printf "PASS=%-5d FAIL=%-5d STUB=%-5d NEW=%-5d GONE=%-5d\n" \
       "$pass" "$fail" "$stub" "$new" "$gone"
echo ""

if [ ${#fail_lines[@]} -gt 0 ]; then
    echo "--- FAIL cells (run diff manually for detail) ---"
    printf '  %s\n' "${fail_lines[@]}" | head -50
    if [ ${#fail_lines[@]} -gt 50 ]; then
        echo "  ... and $((${#fail_lines[@]} - 50)) more"
    fi
    echo ""
fi
if [ ${#gone_lines[@]} -gt 0 ]; then
    echo "--- GONE cells (baseline had these, current audit missing) ---"
    printf '  %s\n' "${gone_lines[@]}" | head -20
    echo ""
fi
if [ ${#new_lines[@]} -gt 0 ]; then
    echo "--- NEW cells (current audit produced; no baseline yet) ---"
    printf '  %s\n' "${new_lines[@]}" | head -20
    echo "  (Re-run freeze_per_kind_baseline.sh to record them.)"
    echo ""
fi

# Hint for human reading the FAIL list
if [ "$fail" -gt 0 ] || [ "$gone" -gt 0 ]; then
    echo "Investigate a FAIL with:"
    echo "  diff <($NORMALIZER --ext s $RUN/x86/text/BB_PAT_REM.s) $BASE/x86/text/BB_PAT_REM.s.norm"
    echo ""
fi

rm -rf "$RUN"
[ "$fail" -eq 0 ] && [ "$gone" -eq 0 ]
