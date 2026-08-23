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
for f in src/templates/bb_*.cpp src/templates/xa_*.cpp; do
    [ -f "$f" ] || continue
    name="$(basename "$f")"
    total_files=$((total_files + 1))
    em=$(strip "$f"    | grep -cE 'pBB->[αβγω]' || true)
    nw=$(strip "$f"    | sed 's/\.node->/\.NODE->/g' | grep -cE '\b(lf|fin|a0|a1|arm|node)->(α|β|γ|ω|t|op)\b' || true)
    bs=$(strip "$f"    | grep -cE 'b\.size\(' || true)
    rb=$(strip "$f"    | grep -cE 'x86_Lrec|x86_Jrec|x86_Drec|x86_b[123]\(|bytes\(|u8\(|u32le|u64le' || true)
    mt=$(strip "$f"    | grep -c 'MEDIUM_' || true)
    ef=$(strip "$f"    | grep -cE 'emit_fmt\(' || true)
    lc=$(grep -cE '^\s*//' "$f" || true)
    bl=$(grep -c '^[[:space:]]*$' "$f" || true)
    pe=$(strip "$f"    | grep -cE 'PORT_ALPHA|PORT_BETA|PORT_GAMMA|PORT_OMEGA' || true)
    lv=$(strip "$f"    | grep -cE '^\s+(std::string|const char ?\*|IR_t ?\*|void ?\*|uint64_t|int64_t|size_t|unsigned|double|float|short|long|bool|auto|char|int)[ \*]' || true)
    ret_all=$(strip "$f" | grep -cE '\breturn\b' || true); ret_lam=$(strip "$f" | grep -E '\breturn\b' | grep -cE '\[[&=][^]]*\][[:space:]]*\([^)]*\)[[:space:]]*\{' || true); ret=$(( ret_all - ret_lam )); rp=$(( ret > 2 ? ret - 2 : 0 ))
    st=$(grep -cE '^static' "$f" || true); hc=$(( st > 2 ? st - 2 : 0 ))
    sd=$(strip "$f"    | grep -cE '\)[[:space:]]*\{[[:space:]]*(std::string|int |long |const char|auto |bool |double |char |uint64_t|size_t|IR_t)' || true)
    cl=$(awk 'length>200' "$f" | wc -l)
    ml=$(sed 's/\\"//g; s/"[^"]*"//g' "$f" | grep -cE 'x86\(.*x86\(' || true)
    cmtA=$(grep -cE '/\*|//' "$f" || true); sep=$(grep -cE '^/\*[-=]+\*/[[:space:]]*$' "$f" || true); xc=$(( cmtA > sep ? cmtA - sep : 0 ))
    bp=$(strip "$f"    | grep -cE 'x86_(frame|ro|reg)_[a-z0-9_]*\(' || true)
    lb=$(strip "$f"    | grep -cE '\bg_(gvar_flat_chain|descr_flat_chain|icn_scan_regs_live|gvar_callarg_live)\b' || true)
    tot=$((em + nw + bs + rb + mt + ef + lc + bl + pe + lv + rp + hc + sd + cl + ml + xc + bp + lb))
    grand_violations=$((grand_violations + tot))
    if [ "$tot" -gt 0 ]; then
        total_dirty=$((total_dirty + 1))
        rows+=("$(printf "%05d %s eb=%d nw=%d bs=%d rb=%d mt=%d ef=%d lc=%d bl=%d pe=%d lv=%d rp=%d hc=%d sd=%d cl=%d ml=%d xc=%d bp=%d lb=%d TOTAL=%d" "$tot" "$name" "$em" "$nw" "$bs" "$rb" "$mt" "$ef" "$lc" "$bl" "$pe" "$lv" "$rp" "$hc" "$sd" "$cl" "$ml" "$xc" "$bp" "$lb" "$tot")")
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

# ⭐ V2-5 GATE HONESTY: this script had NO exit statement -- "83 file(s) need fixup" exited 0.
. "$(dirname "$0")/lib_gate.sh"
gate_floor "$total_files" 50 "bb_*/xa_* template files"
gate_verdict "$total_dirty" "template file(s) need fixup"
