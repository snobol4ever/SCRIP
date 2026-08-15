#!/usr/bin/env bash
# scripts/test_icon_x64_all_rungs.sh — Icon rung ladder runner under scrip --compile (mode 4).
# REBUILT s230 (Z-0(a), FR-6d recipe): faithful clone of test_icon_all_rungs.sh with the run step
# swapped for the smoke-proven mode-4 recipe (--compile → stdout asm → gcc -no-pie -lscrip_rt → run).
# Replaces the deleted-era runner (-icn flag + nasm + /home/scrip path) whose 0/293 measured the
# harness, not the compiler (verified s230; see GOAL-ICON-100.md LEDGER v2 R-6).
# SAME 293 denominator as the m3 runner: every rung .icn with a .expected; .xfail markers honored.
# CRASH-AWARE: FAILs are classified emit/link/sig/tmo/out (DIRTY = emit+link+sig+tmo).
# Scorecard contract: prints "Results: P passed, Q failed, S skipped" (Q = FAIL+XFAIL so the
# denominator stays 293 per the DoD "XFAIL in denominator"); accepts positional corpus dir.
# Usage: bash scripts/test_icon_x64_all_rungs.sh [corpus-dir] [--rung RUNG] [--scrip PATH] [--corpus PATH]
# Authors: LCherryholmes · Jeffrey Cooper M.D. · Claude Fable 5

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
OUT="$(cd "$HERE/.." && pwd)/out"
RT_SO="$OUT/libscrip_rt.so"
CORPUS="${CORPUS:-/home/claude/corpus/programs/icon}"
RUNG=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rung)   RUNG="$2";   shift 2 ;;
        --scrip)  SCRIP="$2";  shift 2 ;;
        --corpus) CORPUS="$2"; shift 2 ;;
        -*) echo "Usage: $0 [corpus-dir] [--rung RUNG] [--scrip PATH] [--corpus PATH]" >&2; exit 1 ;;
        *)  CORPUS="$1"; shift ;;
    esac
done

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip binary not found at $SCRIP" >&2; exit 0; fi
if [ ! -f "$RT_SO" ]; then echo "SKIP libscrip_rt.so not found at $RT_SO (make libscrip_rt)" >&2; exit 0; fi
if [ ! -d "$CORPUS" ]; then echo "SKIP corpus not found at $CORPUS" >&2; exit 0; fi

PASS=0; FAIL=0; XFAIL=0; SKIP=0
D_EMIT=0; D_LINK=0; D_SIG=0; D_TMO=0
WORK=$(mktemp -d /tmp/icn_m4_XXXXXX)
trap 'rm -rf "$WORK"' EXIT

# rung36 per-category tally (sidecar map at corpus/rung36_categories.txt)
declare -A R36_CAT_P R36_CAT_F R36_CAT_X
R36_MAP_FILE="$CORPUS/rung36_categories.txt"
declare -A R36_NAME_TO_CAT
if [ -f "$R36_MAP_FILE" ]; then
    while IFS=$'\t' read -r r36_name r36_cat; do
        case "$r36_name" in ''|'#'*) continue ;; esac
        R36_NAME_TO_CAT["$r36_name"]="$r36_cat"
        R36_CAT_P["$r36_cat"]=${R36_CAT_P["$r36_cat"]:-0}
        R36_CAT_F["$r36_cat"]=${R36_CAT_F["$r36_cat"]:-0}
        R36_CAT_X["$r36_cat"]=${R36_CAT_X["$r36_cat"]:-0}
    done < "$R36_MAP_FILE"
fi

r36_tally() {
    local name="$1" status="$2"
    case "$name" in
        rung36_jcon_*)
            local stem="${name#rung36_jcon_}"
            local cat="${R36_NAME_TO_CAT[$stem]:-uncategorized}"
            case "$status" in
                PASS)  R36_CAT_P["$cat"]=$((${R36_CAT_P["$cat"]:-0}+1)) ;;
                FAIL)  R36_CAT_F["$cat"]=$((${R36_CAT_F["$cat"]:-0}+1)) ;;
                XFAIL) R36_CAT_X["$cat"]=$((${R36_CAT_X["$cat"]:-0}+1)) ;;
            esac
            ;;
    esac
}

run_one() {
    local icn="$1"
    local tmo="${2:-8}"
    local exp="${icn%.icn}.expected"
    [ -f "$exp" ] || { SKIP=$((SKIP+1)); return 0; }
    local base="${icn%.icn}"
    local name
    name=$(basename "$icn" .icn)
    if [ -f "${base}.xfail" ]; then
        echo "XFAIL $name"
        XFAIL=$((XFAIL+1)); r36_tally "$name" XFAIL
        return 0
    fi
    local stdin_file="${base}.stdin"
    local tdir tfn asm bin rc got want
    tdir=$(dirname "$icn"); tfn=$(basename "$icn")
    asm="$WORK/$name.s"; bin="$WORK/$name.bin"
    (cd "$tdir" && timeout "$tmo" "$SCRIP" --compile "$tfn" > "$asm" 2>/dev/null < /dev/null); rc=$?
    if [ $rc -ne 0 ] || [ ! -s "$asm" ]; then
        echo "FAIL $name  [emit rc=$rc]"
        FAIL=$((FAIL+1)); D_EMIT=$((D_EMIT+1)); r36_tally "$name" FAIL
        return 0
    fi
    if ! gcc -no-pie "$asm" -L"$OUT" -lscrip_rt -Wl,-rpath,"$OUT" -o "$bin" 2>/dev/null; then
        echo "FAIL $name  [link]"
        FAIL=$((FAIL+1)); D_LINK=$((D_LINK+1)); r36_tally "$name" FAIL
        return 0
    fi
    if [ -f "$stdin_file" ]; then
        got=$( (cd "$tdir" && timeout "$tmo" "$bin") < "$stdin_file" 2>/dev/null); rc=$?
    else
        got=$( (cd "$tdir" && timeout "$tmo" "$bin") < /dev/null     2>/dev/null); rc=$?
    fi
    want=$(cat "$exp")
    if [ "$got" = "$want" ] && [ $rc -lt 124 ]; then
        echo "PASS $name"
        PASS=$((PASS+1)); r36_tally "$name" PASS
    else
        if   [ $rc -eq 124 ] || [ $rc -eq 137 ]; then echo "FAIL $name  [tmo]";      D_TMO=$((D_TMO+1))
        elif [ $rc -ge 128 ];                    then echo "FAIL $name  [sig=$((rc-128))]"; D_SIG=$((D_SIG+1))
        else                                          echo "FAIL $name  [out rc=$rc]"
        fi
        echo "  want: $(echo "$want" | tr '\n' '|')"
        echo "  got:  $(echo "$got"  | tr '\n' '|')"
        FAIL=$((FAIL+1)); r36_tally "$name" FAIL
    fi
}

if [ -n "$RUNG" ]; then
    for icn in "$CORPUS"/${RUNG}_*.icn; do
        [ -f "$icn" ] || continue
        run_one "$icn"
    done
else
    for icn in "$CORPUS"/rung0[1-9]_*.icn \
               "$CORPUS"/rung1[0-9]_*.icn \
               "$CORPUS"/rung2[0-9]_*.icn \
               "$CORPUS"/rung3[0-5]_*.icn; do
        [ -f "$icn" ] || continue
        run_one "$icn" 8
    done
    for icn in "$CORPUS"/rung36_*.icn; do
        [ -f "$icn" ] || continue
        run_one "$icn" 8
    done
    for icn in "$CORPUS"/rung37_*.icn; do
        [ -f "$icn" ] || continue
        run_one "$icn" 8
    done
fi

echo "--- rung36 by category ---"
{
    for cat in "${!R36_CAT_P[@]}" "${!R36_CAT_F[@]}" "${!R36_CAT_X[@]}"; do echo "$cat"; done
} | sort -u | while read -r cat; do
    [ -z "$cat" ] && continue
    p=${R36_CAT_P[$cat]:-0}; f=${R36_CAT_F[$cat]:-0}; x=${R36_CAT_X[$cat]:-0}
    total=$((p+f+x))
    printf "  rung36_%-12s  total=%2d  PASS=%2d  FAIL=%2d  XFAIL=%2d\n" "$cat" "$total" "$p" "$f" "$x"
done

DIRTY=$((D_EMIT+D_LINK+D_SIG+D_TMO))
echo "--- Icon --compile: PASS=$PASS FAIL=$FAIL XFAIL=$XFAIL TOTAL=$((PASS+FAIL+XFAIL)) (DIRTY=$DIRTY: emit=$D_EMIT link=$D_LINK sig=$D_SIG tmo=$D_TMO) ---"
echo "Results: $PASS passed, $((FAIL+XFAIL)) failed, $SKIP skipped"
[ "$FAIL" -eq 0 ]
