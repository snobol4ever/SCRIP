#!/usr/bin/env bash
# test_gate_ladder_walk_v4_exempts_package_class_rows.sh -- THE ACCEPTANCE GATE for row ladder-walk-v4-exempts-package-
# class-rows (ceo CEO-230 -> hq_T 2026-09-04; law RULES.md § FACT RULES THE PACKAGE-CLASS RULE, on Lon 13:20: the
# percentage IS the V column).
#
# THE DEFECT: util_ladder_walk.py's V4 RANK INVERSION flagged EVERY FREE off-ladder row at rank 0/1 -- including the class
# rows minted from a vendored-package census (icon-arizona-class-…, pascal-fpc-class-…, prolog-inria-class-…), which are
# LADDER-RANK WORK by law and belong at rank 1. The ceo's 16:14 walk printed 18 such lines; on the 2026-09-04 evening
# queue the count was 27 of 62 V4 lines, so the report the ceo reads every tick was nearly half noise, and a report that
# is half noise is a report nobody reads (the same finding that turned V6 into a census).
#
# ⛔ THE CURE IS BY SHAPE, NOT BY A LIST OF PACKAGE NAMES, and this gate pins that: a list is a census that drifts
# silently the day a new suite is vendored -- its class rows would be flagged again and read as genuine inversions by
# whoever had stopped seeing the old ones. The shape the law defines is <lang>-<package>-class-<slug>: a recognised
# language prefix, exactly ONE package token, then -class-. Two tokens (prolog-master-red-class-…) is a MASTER-BOARD red
# class, not a package class, and stays a V4 candidate.
#
# ARMS, hermetic (a scratch plan + a scratch postoffice under mktemp; the LIVE postoffice is never read):
#   ARM 1  a <lang>-<package>-class-* row FREE at rank 1 is NOT flagged            (pre-cure: flagged -- the fail-once arm)
#   ARM 2  a genuine off-ladder rank-1 row IS still flagged                        (control: the cure did not delete V4's job)
#   ARM 3  the exemption is COUNTED on one visible line, never silent              (pre-cure: no such line)
#   ARM 4  by SHAPE: a two-token -master-red-class- row at rank 1 is still flagged
#   ARM 5  rank 2+ was never V4's business, either way
# FAIL-ONCE PROVEN 2026-09-04: SUT=<pre-cure util_ladder_walk.py> fails exactly ARM 1 and ARM 3, passes 2/4/5.
# EXIT: 0 all arms · 1 an arm failed · 2 REFUSED (no python3, SUT missing, mktemp failed, or the walk itself refused).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SUT="${SUT:-$HERE/util_ladder_walk.py}"
[ -f "$SUT" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: script under test not found: $SUT"; exit 2; }
command -v python3 >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE rc=2: python3 not on PATH"; exit 2; }
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_walk_v4.XXXXXX")" || { echo "⛔ REFUSED-TO-GRADE rc=2: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/po/claims" "$W/po/tasks"
printf '# scratch index\n' > "$W/po/QUEUE.done.tsv"
printf '# scratch index\n0\tsno-scratch-rung-row\thq_C\tFREE\n1\ticon-arizona-class-scratch-package-class\thq_B\tFREE\n1\ticon-scratch-genuine-off-ladder-row\thq_B\tFREE\n1\tprolog-master-red-class-scratch-two-tokens\thq_C\tFREE\n2\tpascal-fpc-class-scratch-rank-two\thq_P\tFREE\n' > "$W/po/QUEUE.tsv"
printf '### LADDER SNO — scratch\n\n| rung | row | lane | gate |\n|---|---|---|---|\n| SNO1 | sno-scratch-rung-row | hq_C | x |\n' > "$W/plan.md"
out="$(python3 "$SUT" --plan "$W/plan.md" --po "$W/po" --quiet 2>&1)"; rc=$?
# rc=1 is the only honest reading of this fixture: ARM 2's genuine inversion is always present, so a 0 means the walk
# printed nothing at all, and a 2 means it refused -- neither is a state this gate can grade.
[ "$rc" = 1 ] || { echo "⛔ REFUSED-TO-GRADE rc=2: the walk must exit 1 (violations printed) on the scratch fixture; it exited $rc:"; echo "$out"; exit 2; }
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
v4() { grep -E '^ *V4 RANK INVERSION ' <<<"$out" | grep -c -- "$1"; }
echo "=== gate: the ladder walk's V4 exempts package-class rows -- by shape, visibly, and only those ==="
echo "--- ARM 1: a <lang>-<package>-class-* row FREE at rank 1 is NOT a V4 inversion ---"
n=$(v4 'icon-arizona-class-scratch-package-class'); [ "$n" = 0 ] && ck ok "package class row at rank 1 -> no V4 line" || ck no "package class row still flagged ($n line(s)) -- it is ladder-rank by law"
echo "--- ARM 2 (control): a genuine off-ladder rank-1 row IS still flagged ---"
n=$(v4 'icon-scratch-genuine-off-ladder-row'); [ "$n" = 1 ] && ck ok "genuine off-ladder rank-1 row -> exactly one V4 line" || ck no "genuine inversion not flagged ($n line(s)) -- the cure deleted V4's job"
echo "--- ARM 3: the exemption is COUNTED, never silent ---"
grep -qE '^V4 EXEMPT \(package-class rule\): 1 ' <<<"$out" && ck ok "one visible census line names 1 exempted row" || ck no "no census line -- an invisible filter is indistinguishable from lost lines"
echo "--- ARM 4: by SHAPE -- two tokens before -class- is a master-board red class, not a package class ---"
n=$(v4 'prolog-master-red-class-scratch-two-tokens'); [ "$n" = 1 ] && ck ok "prolog-master-red-class-* at rank 1 -> still flagged" || ck no "two-token row wrongly exempted ($n line(s))"
echo "--- ARM 5: rank 2+ was never V4's business, either way ---"
n=$(v4 'pascal-fpc-class-scratch-rank-two'); [ "$n" = 0 ] && ck ok "package class row at rank 2 -> no V4 line" || ck no "rank-2 row flagged ($n) -- V4 looks only at rank 0/1"
echo "------------------------------------------------------------"
if [ "$fails" -ne 0 ]; then echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; fi
echo "✅ GATE PASS: $checks/$checks checks"; exit 0
