#!/usr/bin/env bash
# board_icon_master.sh — THE SET-LEVEL BOARD OVER corpus/tests/icon/ALL.icn (row icon-all-master-board).
# Authors: LCherryholmes · Claude Opus 5   DATE: 2026-08-30
#
# ⛔⭐⭐ WHY THIS EXISTS: UNTIL TODAY, ICON'S COVERAGE SAFETY RESTED ON A NAMING CONVENTION.
# `test_icon_all_rungs.sh` and its kin grade the master suite FAMILY BY FAMILY, by name (`SUITE
# rung30_builtins_misc: pass=5`, ...). That covers whatever families those scripts happen to name —
# and NOTHING checks that the named families still add up to the whole set. An entry whose family is
# renamed, or a family nobody re-points a grader at, simply stops being graded, and every board stays
# green because no board ever knew the denominator. MEASURED at mint: ALL.csv carries **534** entries;
# the aggregate rung grader names a strict subset. That gap is the same absorbed-population class that
# hid NINE gradings in the Prolog rungs and 143 more behind rc=2 refusals — this board closes the door
# on the Icon side by grading the SET and printing its own denominator.
#
# ⛔ THIS IS A WATERMARK CENSUS BOARD, NOT A FAIL=0 GATE, AND THAT IS DELIBERATE.
# MEASURED at mint (pristine -O0, both modes): total=534 · m3 pass=393 fail=122 crash=16 hang=2 ·
# m4 pass=393 fail=122 crash=1 hang=2 skip=15. Icon is nowhere near green on the whole set, so a
# FAIL=0 bar here would be a gate nobody can satisfy — and hq_P's standing ruling on exactly that
# shape (test_gate_optbypass_watermark.sh) is that such a gate gets `|| true`-d within a week, and an
# ignored gate is worse than no gate. So the verdict is a RATCHET: red only if the graded population
# SHRINKS or the pass counts REGRESS below the pinned watermark. Curing Icon is not this board's job;
# noticing that it moved is.
#
# ⛔ NOT WIRED INTO THE BLOCKING SET. `make test`'s membership is a ruling, not a script's to take —
# and a board that is red-by-construction must never be made blocking. Routed to ceo at mint.
#
# ⭐ FLOOR, NOT A PINNED TOTAL (RULES.md § the denominator law): growth needs no re-pin; only an
# ATTRIBUTED retirement may lower these, in the same commit that shrinks the master.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/icon}"
HARNESS="$HERE/corpus_suite_harness.py"
MASTER_ICN="$CORPUS/ALL.icn"
MASTER_REF="$CORPUS/ALL.ref"
MASTER_CSV="$CORPUS/ALL.csv"
ENTRY_FLOOR="${ICON_MASTER_ENTRY_FLOOR:-534}"
M3_PASS_FLOOR="${ICON_MASTER_M3_PASS_FLOOR:-393}"
M4_PASS_FLOOR="${ICON_MASTER_M4_PASS_FLOOR:-393}"

# ⛔ A BOARD THAT CANNOT MEASURE REFUSES rc=2 — never skip-as-success (RULES.md). Each arm below names
# what is missing, because "cannot enumerate" and "enumerated zero" are different facts and a single
# exit code for both is how a vanished corpus reads as a clean run.
if [ ! -x "$SCRIP" ];       then echo "⛔ BOARD REFUSES (rc=2): scrip not built at $SCRIP"; exit 2; fi
if [ ! -f "$HARNESS" ];     then echo "⛔ BOARD REFUSES (rc=2): corpus_suite_harness.py missing at $HARNESS"; exit 2; fi
if [ ! -f "$MASTER_ICN" ] || [ ! -f "$MASTER_REF" ]; then
    echo "⛔ BOARD REFUSES (rc=2): the Icon master suite is missing at $MASTER_ICN"
    echo "   Do NOT read a family-by-family rung grader's smaller total as this board — that substitution"
    echo "   is the exact failure this board was minted to make impossible. Pull corpus."
    exit 2
fi
if [ ! -f "$MASTER_CSV" ]; then echo "⛔ BOARD REFUSES (rc=2): ALL.csv missing at $MASTER_CSV — no per-entry provenance to attribute a shrink to"; exit 2; fi

CSV_ENTRIES=$(( $(wc -l < "$MASTER_CSV") - 1 ))
echo "=== Icon MASTER board — corpus/tests/icon/ALL.icn ==="
# ⛔ `|| true` IS LOAD-BEARING, NOT SLOPPINESS: the harness exits NON-ZERO whenever any entry fails, and
# this board is a census over a suite that is legitimately red today. Under `set -euo pipefail` the bare
# pipeline aborted the script after printing only its header — a board that dies silently on the very
# condition it exists to report. The refusal arms below are what distinguish "could not measure" from
# "measured a red suite"; the exit status of the harness never does.
_raw=$(timeout 900 python3 "$HARNESS" run "$MASTER_ICN" "$MASTER_REF" --lang icon --modes m3,m4 2>/dev/null || true)
board=$(printf '%s\n' "$_raw" | grep '^SUITE_BOARD ' | tail -1 || true)
if [ -z "$board" ]; then
    echo "⛔ BOARD REFUSES (rc=2): harness produced no SUITE_BOARD line for the Icon master suite"
    echo "   Measured nothing. That is NOT a pass — see RULES.md: a test that cannot measure refuses."
    exit 2
fi
field() { echo "$board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
mt=$(field total)
m3p=$(field m3_pass); m3f=$(field m3_fail); m3c=$(field m3_crash); m3h=$(field m3_hang); m3u=$(field m3_unproven); m3x=$(field m3_xfail); m3xp=$(field m3_xpass)
m4p=$(field m4_pass); m4f=$(field m4_fail); m4c=$(field m4_crash); m4h=$(field m4_hang); m4u=$(field m4_unproven); m4s=$(field m4_skip); m4x=$(field m4_xfail); m4xp=$(field m4_xpass)
if [ -z "$mt" ] || [ "$mt" -eq 0 ]; then
    echo "⛔ BOARD REFUSES (rc=2): the harness graded ZERO entries over a master file that exists"; exit 2
fi

echo "entries=$mt  (ALL.csv rows=$CSV_ENTRIES, floor=$ENTRY_FLOOR)"
echo "mode-3 (--run):     PASS=$m3p FAIL=$m3f CRASH=$m3c HANG=$m3h UNPROVEN=$m3u XFAIL=$m3x XPASS=$m3xp   / $mt"
echo "mode-4 (--compile): PASS=$m4p FAIL=$m4f CRASH=$m4c HANG=$m4h UNPROVEN=$m4u SKIP=$m4s XFAIL=$m4x XPASS=$m4xp   / $mt"
echo "rerun a single mode: python3 $HARNESS run $MASTER_ICN $MASTER_REF --lang icon --modes m3   (per-entry attributes: ALL.csv)"

RED=0
# ⛔ THE DENOMINATOR CHECK IS THE POINT OF THIS BOARD — a shrink is the silent-orphan class itself.
if [ "$mt" -lt "$ENTRY_FLOOR" ]; then
    echo "⛔ RED: graded population $mt is UNDER the floor $ENTRY_FLOOR — $((ENTRY_FLOOR-mt)) entries vanished from the master,"
    echo "        or this checkout is behind origin. An entry that stops being graded is exactly what this board exists to catch."
    RED=1
fi
if [ "$mt" -ne "$CSV_ENTRIES" ]; then
    echo "⚠️  NOTE: harness graded $mt entries but ALL.csv carries $CSV_ENTRIES rows — the suite file and its"
    echo "        provenance index disagree. Neither number is wrong on its face; they must not drift apart."
fi
# ⭐ PASS WATERMARKS: red on regression only. These are NOT a claim that the remainder is acceptable —
# 122 m3 failures are real and belong to hq_C's lane; this board's job is to notice movement.
if [ "$m3p" -lt "$M3_PASS_FLOOR" ]; then echo "⛔ RED: m3 PASS $m3p regressed below watermark $M3_PASS_FLOOR"; RED=1; fi
if [ "$m4p" -lt "$M4_PASS_FLOOR" ]; then echo "⛔ RED: m4 PASS $m4p regressed below watermark $M4_PASS_FLOOR"; RED=1; fi
if [ "$((m3p+m4p))" -gt "$((M3_PASS_FLOOR+M4_PASS_FLOOR))" ]; then
    echo "⭐ WATERMARK MOVED UP (m3 $m3p vs $M3_PASS_FLOOR, m4 $m4p vs $M4_PASS_FLOOR) — re-pin the floors in the commit that earned it."
fi
[ "$((m3xp+m4xp))" -gt 0 ] && echo "⭐ XPASS>0: a bug got FIXED and its XFAIL marker was never promoted — as actionable as a failure, in the opposite direction."

if [ "$RED" -ne 0 ]; then echo "⛔ ICON MASTER BOARD RED"; exit 1; fi
echo "✅ ICON MASTER BOARD OK: entries=$mt at/above floor $ENTRY_FLOOR · m3 PASS=$m3p · m4 PASS=$m4p (watermarks held)"
