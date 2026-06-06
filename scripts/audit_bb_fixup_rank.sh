#!/usr/bin/env bash
# audit_bb_fixup_rank.sh — whole-tree TEMPLATE SPEC v2 lap-progress table for GOAL-BB-FIXUP.md
# Prints a sorted table of all BB_templates/*.cpp files with their violation counts.
# Clean files (total=0) are shown at the bottom; dirty files ranked worst-first.
# Run at session open and close to measure lap progress.
# Usage: bash scripts/audit_bb_fixup_rank.sh [--dirty-only]
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
dirty_only=0
[ "${1:-}" = "--dirty-only" ] && dirty_only=1
strip() { perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$1"; }
declare -a rows
total_files=0
total_dirty=0
total_clean=0
grand_violations=0
for f in src/emitter/BB_templates/bb_*.cpp; do
    [ -f "$f" ] || continue
    name="$(basename "$f")"
    total_files=$((total_files + 1))
    em=$(strip "$f"    | grep -cE 'pBB->[αβγω]' || true)
    nw=$(strip "$f"    | grep -cE '\b(lf|fin|a0|a1|arm|node)->(α|β|γ|ω|t)\b' || true)
    bs=$(strip "$f"    | grep -cE 'b\.size\(' || true)
    rb=$(strip "$f"    | grep -cE 'x86_Lrec|x86_Jrec|x86_Drec|x86_b[123]\(|bytes\(|u8\(|u32le|u64le' || true)
    mb=$(strip "$f"    | grep -cE 'IF\(MEDIUM_BINARY|IF\(MEDIUM_MACRO_DEF' || true)
    ef=$(strip "$f"    | grep -cE 'emit_fmt\(' || true)
    lc=$(grep -cE '^\s*//' "$f" || true)
    bl=$(grep -c '^[[:space:]]*$' "$f" || true)
    pe=$(strip "$f"    | grep -cE 'PORT_ALPHA|PORT_BETA|PORT_GAMMA|PORT_OMEGA' || true)
    lv=$(strip "$f"    | grep -cE '^\s+(int|long|size_t|std::string|const char \*|IR_t \*|void \*|uint64_t|int64_t)[[:space:]]' || true)
    tot=$((em + nw + bs + rb + mb + ef + lc + bl + pe + lv))
    grand_violations=$((grand_violations + tot))
    if [ "$tot" -gt 0 ]; then
        total_dirty=$((total_dirty + 1))
        rows+=("$(printf "%05d %s eb=%d nw=%d bs=%d rb=%d mb=%d ef=%d lc=%d bl=%d pe=%d lv=%d TOTAL=%d" "$tot" "$name" "$em" "$nw" "$bs" "$rb" "$mb" "$ef" "$lc" "$bl" "$pe" "$lv" "$tot")")
    else
        total_clean=$((total_clean + 1))
        [ "$dirty_only" -eq 0 ] && rows+=("$(printf "00000 %s CLEAN" "$name")")
    fi
done
echo "=== BB-FIXUP LAP PROGRESS TABLE ($(date '+%Y-%m-%d %H:%M')) ==="
printf "%-52s %s\n" "FILE" "VIOLATIONS"
echo "--------------------------------------------------------------------"
if [ ${#rows[@]} -gt 0 ]; then
    printf '%s\n' "${rows[@]}" | sort -rn | while IFS= read -r row; do
        score="${row%% *}"
        rest="${row#* }"
        name="${rest%% *}"
        detail="${rest#* }"
        if [ "$score" = "00000" ]; then
            printf "  %-50s CLEAN\n" "$name"
        else
            printf "  %-50s %s\n" "$name" "$detail"
        fi
    done
fi
echo "--------------------------------------------------------------------"
printf "  FILES: %d total / %d dirty / %d clean\n" "$total_files" "$total_dirty" "$total_clean"
printf "  GRAND TOTAL violations: %d\n" "$grand_violations"
if [ "$total_dirty" -eq 0 ]; then
    echo "  LAP STATUS: ALL CLEAN — lap complete"
else
    echo "  LAP STATUS: $total_dirty file(s) need fixup"
fi
