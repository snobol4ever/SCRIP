#!/usr/bin/env bash
# scripts/freeze_per_kind_baseline.sh — EC-UNI-PER-KIND-DIFF baseline freeze.
#
# Records the CURRENT scrip binary's per-(BB-kind × backend) emission as the
# golden baseline.  Run this ONCE at a trusted commit (the "oracle SHA"); the
# resulting tree under baselines/per_kind/ is then committed and becomes the
# reference for all future scripts/test_per_kind_diff.sh runs.
#
# Pipeline per cell:
#   1. ./scrip --audit-per-kind /tmp/audit_freeze
#   2. for each file in /tmp/audit_freeze/<backend>/<KIND>.<ext>:
#        normalize_per_kind_cell.py --ext <ext> >  baselines/per_kind/<backend>/<KIND>.<ext>.norm
#        cp                                          baselines/per_kind/<backend>/<KIND>.<ext>.raw
#   3. write baselines/per_kind/MANIFEST.txt summarizing kind×backend coverage.
#
# Both .norm and .raw are committed:
#   - .norm is what the diff script compares against (filter-stable form).
#   - .raw is forensic — diffing .raw shows the unfiltered emission that the
#     normalizer was hiding.  Cheap insurance, ~few MB total.
#
# Self-contained per RULES.md — no other gates' artifacts assumed.
#
# Usage:  bash scripts/freeze_per_kind_baseline.sh [out_dir]
#   out_dir defaults to baselines/per_kind/ (under repo root)
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
OUT="${1:-$ROOT/baselines/per_kind}"
RAW="/tmp/audit_freeze_$$"
NORMALIZER="$HERE/normalize_per_kind_cell.py"

if [ ! -x "$ROOT/scrip" ]; then
    echo "freeze: $ROOT/scrip not built — run 'make -j4 scrip' first" >&2
    exit 1
fi
if [ ! -x "$NORMALIZER" ]; then
    chmod +x "$NORMALIZER"
fi

echo "freeze: running audit → $RAW"
mkdir -p "$RAW"
"$ROOT/scrip" --audit-per-kind "$RAW"

echo "freeze: clearing existing baseline at $OUT"
rm -rf "$OUT"
mkdir -p "$OUT"

cells=0; non_empty=0
for raw_file in $(find "$RAW" -type f | sort); do
    rel="${raw_file#$RAW/}"           # x86/text/BB_PAT_LIT.s
    name="${rel##*/}"                  # BB_PAT_LIT.s
    dir="${rel%/*}"                    # x86/text
    ext="${name##*.}"                  # s

    mkdir -p "$OUT/$dir"
    "$NORMALIZER" --ext "$ext" "$raw_file" > "$OUT/$dir/$name.raw"
    "$NORMALIZER" --ext "$ext" "$raw_file" > "$OUT/$dir/$name.norm"

    cells=$((cells + 1))
    [ -s "$raw_file" ] && non_empty=$((non_empty + 1)) || true
done

# Manifest — one line per cell, with size + md5 of normalized form.
{
    echo "# EC-UNI-PER-KIND-DIFF baseline manifest"
    echo "# Format: <backend>/<submode> <kind> <raw_bytes> <norm_md5>"
    for norm in $(find "$OUT" -name '*.norm' -type f | sort); do
        rel="${norm#$OUT/}"                  # x86/text/BB_PAT_LIT.s.norm
        dir="${rel%/*}"                       # x86/text
        kind_ext="$(basename "${rel%.norm}")" # BB_PAT_LIT.s
        raw="${norm%.norm}.raw"
        raw_sz="$(stat -c '%s' "$raw" 2>/dev/null || echo 0)"
        norm_md5="$(md5sum "$norm" | awk '{print $1}')"
        printf "%-18s %-30s %8s %s\n" "$dir" "$kind_ext" "$raw_sz" "$norm_md5"
    done
} > "$OUT/MANIFEST.txt"

# x86 assemble-then-md5 — captures the SEMANTIC ground truth that survives
# text-format drift.  For each non-empty x86/text cell, try to assemble it
# and record the .o md5.  Failures are recorded with 'NOASM'.
ASM_MD5="$OUT/x86_text_assembled_md5.txt"
{
    echo "# x86/text assembled-object md5 (semantic ground truth)"
    for s in "$OUT"/x86/text/*.s.raw; do
        [ -s "$s" ] || continue
        kind="$(basename "${s%.s.raw}")"
        tmp="/tmp/audit_asm_$$_${kind}.o"
        if as --64 "$s" -o "$tmp" 2>/dev/null; then
            md5="$(md5sum "$tmp" | awk '{print $1}')"
        else
            md5="NOASM"
        fi
        rm -f "$tmp"
        printf "%-30s %s\n" "$kind" "$md5"
    done
} > "$ASM_MD5"

echo "freeze: $cells cells written ($non_empty non-empty)"
echo "freeze: manifest at $OUT/MANIFEST.txt"
echo "freeze: assembled md5 table at $ASM_MD5"
echo "freeze: ready to commit baselines/per_kind/"
rm -rf "$RAW"
