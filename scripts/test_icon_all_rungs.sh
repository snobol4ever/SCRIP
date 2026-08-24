#!/usr/bin/env bash
# scripts/test_icon_all_rungs.sh — Icon rung ladder runner under scrip --run.
# Self-contained. Run from anywhere with no env vars.
# Usage: bash scripts/test_icon_all_rungs.sh [--rung RUNG] [--scrip PATH] [--corpus PATH]
#
# Runs rung01–rung36 (or a specific rung) of the Icon corpus against
# scrip --run and reports PASS/FAIL/XFAIL vs .expected files.
# Files with a matching .xfail marker are skipped as known-unimplemented (XFAIL).
# All rungs use timeout 8s. rung36_jcon_subjpos quarantined via .xfail (infinite
# loop in --run subject/&pos path, hangs to timeout; see GOAL-ICON-BB). No
# legitimate rung36 program exceeds 8s (timing audit 2026-05-26).
#
# Replaces test_icon_all_rungs.sh (deleted, was --run-based and gated on
# the now-amputated Icon AST walker). The reference path for Icon is --run.
#
# Authors: LCherryholmes · Claude Sonnet 4.6 · Claude Opus 4.7
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/icon}"
RUNG=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rung)   RUNG="$2";   shift 2 ;;
        --scrip)  SCRIP="$2";  shift 2 ;;
        --corpus) CORPUS="$2"; shift 2 ;;
        *) echo "Usage: $0 [--rung RUNG] [--scrip PATH] [--corpus PATH]" >&2; exit 1 ;;
    esac
done

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip binary not found at $SCRIP" >&2
    exit 0
fi
if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS" >&2
    echo "     clone snobol4ever/corpus to $S4E/corpus to run this suite" >&2
    exit 0
fi

PASS=0; FAIL=0; XFAIL=0; BADEXIT=0

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
                PASS)    R36_CAT_P["$cat"]=$((${R36_CAT_P["$cat"]:-0}+1)) ;;
                FAIL)    R36_CAT_F["$cat"]=$((${R36_CAT_F["$cat"]:-0}+1)) ;;
                BADEXIT) R36_CAT_F["$cat"]=$((${R36_CAT_F["$cat"]:-0}+1)) ;;
                XFAIL) R36_CAT_X["$cat"]=$((${R36_CAT_X["$cat"]:-0}+1)) ;;
            esac
            ;;
    esac
}

run_one() {
    local icn="$1"
    local tmo="${2:-8}"
    local exp="${icn%.icn}.expected"
    [ -f "$exp" ] || return 0
    local base="${icn%.icn}"
    local name
    name=$(basename "$icn" .icn)
    if [ -f "${base}.xfail" ]; then
        echo "XFAIL $name"
        XFAIL=$((XFAIL+1))
        r36_tally "$name" XFAIL
        return 0
    fi
    local stdin_file="${base}.stdin"
    local tdir tfn
    tdir=$(dirname "$icn"); tfn=$(basename "$icn")
    local got want rc=0 want_rc=0
    # ⛔ EXIT STATUS IS PART OF THE ANSWER (hq_P s272). This runner used to end both arms with `|| true`,
    # discarding rc entirely and grading stdout alone -- so a program that printed the right answer and then
    # ABORTED counted PASS. That false-green is worth exactly 75 programs on this suite and is why this board
    # read 244 while test_icon_rung_suite.sh (which enforces rc) read 169 on the SAME tree, SAME 293 files,
    # SAME 30 XFAIL -- two boards that looked directly comparable and were not.
    # ⭐ Graded against the ORACLE, not against 0: real iconx exits 0 for a main that FAILS (the normal end of
    # `every`), but a few programs legitimately exit nonzero, so a `<base>.exitcode` sidecar names the expected
    # code where it is not 0. Wrong-rc lands in its OWN bucket, never silently inside FAIL.
    if [ -f "$stdin_file" ]; then
        got=$( (cd "$tdir" && timeout "$tmo" "$SCRIP" --run "$tfn") < "$stdin_file" 2>/dev/null) || rc=$?
    else
        got=$( (cd "$tdir" && timeout "$tmo" "$SCRIP" --run "$tfn") < /dev/null     2>/dev/null) || rc=$?
    fi
    [ -f "${base}.exitcode" ] && want_rc=$(tr -dc '0-9' < "${base}.exitcode")
    want=$(cat "$exp")
    if [ "$got" = "$want" ] && [ "$rc" != "$want_rc" ]; then
        echo "BADEXIT $name (stdout correct, exit $rc, expected $want_rc)"
        BADEXIT=$((BADEXIT+1))
        r36_tally "$name" BADEXIT
    elif [ "$got" = "$want" ]; then
        echo "PASS $name"
        PASS=$((PASS+1))
        r36_tally "$name" PASS
    else
        echo "FAIL $name"
        echo "  want: $(echo "$want" | tr '\n' '|')"
        echo "  got:  $(echo "$got"  | tr '\n' '|')"
        FAIL=$((FAIL+1))
        r36_tally "$name" FAIL
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
    p=${R36_CAT_P[$cat]:-0}
    f=${R36_CAT_F[$cat]:-0}
    x=${R36_CAT_X[$cat]:-0}
    total=$((p+f+x))
    printf "  rung36_%-12s  total=%2d  PASS=%2d  FAIL=%2d  XFAIL=%2d\n" "$cat" "$total" "$p" "$f" "$x"
done

echo "--- Icon --run: PASS=$PASS FAIL=$FAIL BADEXIT=$BADEXIT XFAIL=$XFAIL TOTAL=$((PASS+FAIL+BADEXIT+XFAIL)) ---"
if [ "$BADEXIT" -gt 0 ]; then
    echo "--- BADEXIT = stdout matched .expected but the process exit status did not. Before hq_P s272 these"
    echo "--- counted as PASS (rc was discarded), which is why this board previously read PASS=$((PASS+BADEXIT))."
    echo "--- This is NOT a regression: it is the same tree, graded on exit status for the first time."
fi
[ "$FAIL" -eq 0 ] && [ "$BADEXIT" -eq 0 ]
