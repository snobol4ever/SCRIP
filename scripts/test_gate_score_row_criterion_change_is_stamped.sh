#!/usr/bin/env bash
# test_gate_score_row_criterion_change_is_stamped.sh -- A CRITERION CHANGE IS STAMPED BY THE WRITER, AND A DENOMINATOR MOVE
# WITHOUT ITS STAMP REFUSES BEFORE ANY WRITE (coo 2026-09-16, ceo CEO-785; row instruments-util-score-row-cannot-stamp-a-
# criterion-change-so-every-denominator-move-is-hand-edited-or-unstamped).
#
# THE DEFECT: util_score_row.py write had no flag that writes SUITES.tsv column 12 (criterion_changed), so the CEO-749 shape
# -- a denominator move NAMES why, in the row -- was unenforceable by the instrument that owns the row: the coo's Budne stamp
# of 09-13 was a hand edit (.github 11de13f9) and hq_pascal's PAT INSTRUMENT CHANGE of 2026-09-16 (SCRIP fe37edc72, the m4
# rejection arm compile-link-run, 284 -> 296) rewrote its row with NO stamp. Two HQs were hand-appending stamps.
#
# THE CURE UNDER TEST: (1) util_suite_banner.py --set ... --criterion-changed '<YYYY-MM-DD>:<reason>' APPENDS to column 12
# with the ' | ' separator, every other byte of every other row unchanged; a --set whose TOTAL differs from the row's previous
# today_total REFUSES rc=2 without the stamp, TSV byte-identical; (2) util_score_row.py write gains --criterion-changed,
# passes it through, and REFUSES rc=2 before touching SCORE.md or SUITES.tsv when the write moves the denominator or its
# --text names an INSTRUMENT CHANGE without the stamp (its own selftest carries those arms; this gate proves the file layer
# and the argparse surface hermetically, on COPIES of the live .github).
#
# ARMS (scratch copies of SUITES.tsv + SCORE.md under mktemp; the live files are never touched):
#   (a) a stampless denominator move through --set REFUSES rc=2 and the TSV is byte-identical
#   (b) the same move WITH --criterion-changed lands: today_pass/total/tree set, column 12 appended with ' | ', exactly ONE
#       line of the TSV differs from the original (numstat 1 1) and every other row is byte-identical
#   (c) a same-total --set without a stamp still lands (no false refusal)
#   (d) a malformed stamp (no YYYY-MM-DD: prefix) REFUSES rc=2, TSV byte-identical
#   (e) util_score_row.py write --help documents --criterion-changed, and a --dry-run write that moves a denominator without
#       it REFUSES rc=2 leaving both scratch files byte-identical; the same --dry-run with the stamp previews rc=0
# FAIL_ONCE=1 strips the stamp back out of the scratch TSV after arm (b)'s write, to prove the column assertion trips.
# rc 0 = every arm holds; rc 1 = a FAIL named; rc 2 = REFUSED-TO-GRADE (fixture could not be built).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GH="${S4E_GITHUB_DIR:-$HERE/../../.github}"
BANNER="$GH/scripts/util_suite_banner.py"; HELPER="$HERE/util_score_row.py"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -f "$GH/SUITES.tsv" ] && [ -f "$GH/SCORE.md" ] && [ -f "$BANNER" ] && [ -f "$HELPER" ] || refuse "need $GH/SUITES.tsv, SCORE.md, scripts/util_suite_banner.py and $HELPER"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_criterion_stamp.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/.github/scripts"; cp "$GH/SUITES.tsv" "$W/.github/SUITES.tsv"; cp "$GH/SCORE.md" "$W/.github/SCORE.md"; cp "$BANNER" "$W/.github/scripts/util_suite_banner.py"
TSV="$W/.github/SUITES.tsv"; cp "$TSV" "$W/orig.tsv"
KEY="$(awk -F'\t' '!/^#/ && $1!="key" && $10 ~ /^[0-9]+$/ {print $1; exit}' "$TSV")"; [ -n "$KEY" ] || refuse "no SUITES.tsv row with a numeric today_total to plant on"
OLDT="$(awk -F'\t' -v k="$KEY" '$1==k{print $10}' "$TSV")"; OLDP="$(awk -F'\t' -v k="$KEY" '$1==k{print $9}' "$TSV")"; NEWT=$((OLDT+7))
echo "    fixture: row $KEY reads $OLDP/$OLDT; the planted move is to $NEWT"
set_(){ S4E_SUITES_TSV="$TSV" S4E_SCORE_MD="$W/.github/SCORE.md" python3 "$BANNER" --set "$@" 2>&1; }
fails=0; checks=0; ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: a criterion change is stamped by the writer; a stampless denominator move refuses (CEO-785) ==="
# (a)
out="$(set_ "$KEY" "$OLDP" "$NEWT" 2026-09-16 feedbeef1)"; rc=$?
if [ "$rc" = 2 ] && cmp -s "$TSV" "$W/orig.tsv" && grep -q 'criterion-changed' <<<"$out"; then ck ok "(a) stampless denominator move $OLDT -> $NEWT REFUSED rc=2, TSV byte-identical, the refusal names the flag"
else ck no "(a) rc=$rc identical=$(cmp -s "$TSV" "$W/orig.tsv" && echo yes || echo no) -- got: $(tail -1 <<<"$out" | cut -c1-160)"; fi
# (b)
STAMP="2026-09-16:gate fixture -- the planted move $OLDT -> $NEWT"
out="$(set_ "$KEY" "$OLDP" "$NEWT" 2026-09-16 feedbeef1 --criterion-changed "$STAMP")"; rc=$?
[ -n "${FAIL_ONCE:-}" ] && sed -i "s/ | $STAMP\$//; s/\t$STAMP\$/\t/" "$TSV"
row="$(awk -F'\t' -v k="$KEY" '$1==k' "$TSV")"; c9="$(cut -f9 <<<"$row")"; c10="$(cut -f10 <<<"$row")"; c11="$(cut -f11 <<<"$row")"; c12="$(cut -f12 <<<"$row")"
ndiff="$(diff "$W/orig.tsv" "$TSV" | grep -c '^>')"
if [ "$rc" = 0 ] && [ "$c9" = "$OLDP" ] && [ "$c10" = "$NEWT" ] && [ "$c11" = feedbeef1 ] && [ "$ndiff" = 1 ] && { [ "$c12" = "$STAMP" ] || [ "${c12%" | $STAMP"}" != "$c12" ]; }; then
  ck ok "(b) the stamped move landed: $KEY -> $OLDP/$NEWT tree feedbeef1, column 12 appended with ' | ', exactly 1 TSV line changed"
else ck no "(b) rc=$rc c9=$c9 c10=$c10 c11=$c11 lines-changed=$ndiff c12=...${c12: -60}"; fi
# (c)
cp "$TSV" "$W/afterb.tsv"
out="$(set_ "$KEY" "$((OLDP))" "$NEWT" 2026-09-16 feedbeef2)"; rc=$?
[ "$rc" = 0 ] && [ "$(awk -F'\t' -v k="$KEY" '$1==k{print $11}' "$TSV")" = feedbeef2 ] && ck ok "(c) a same-total --set without a stamp still lands (tree moved to feedbeef2, no false refusal)" || ck no "(c) rc=$rc -- got: $(tail -1 <<<"$out" | cut -c1-160)"
# (d)
cp "$TSV" "$W/afterc.tsv"
out="$(set_ "$KEY" "$OLDP" "$((NEWT+1))" 2026-09-16 feedbeef3 --criterion-changed "no date prefix")"; rc=$?
[ "$rc" = 2 ] && cmp -s "$TSV" "$W/afterc.tsv" && ck ok "(d) a malformed stamp (no YYYY-MM-DD: prefix) REFUSED rc=2, TSV byte-identical" || ck no "(d) rc=$rc identical=$(cmp -s "$TSV" "$W/afterc.tsv" && echo yes || echo no)"
# (e)
python3 "$HELPER" write --help 2>&1 | grep -q -- '--criterion-changed' && ck ok "(e1) util_score_row.py write --help documents --criterion-changed" || ck no "(e1) write --help does not mention --criterion-changed"
cp "$W/orig.tsv" "$TSV"; cp "$GH/SCORE.md" "$W/.github/SCORE.md"; m0="$(cat "$TSV" "$W/.github/SCORE.md" | md5sum)"
out="$(S4E_HOME="$W" python3 "$HELPER" write --lang icon --column vendor --suite Arizona --text 'Arizona: m3 46/124 · m4 46/124 (`test_icon_arizona_suite.sh`)' --measurer coo --dry-run 2>&1)"; rc=$?
m1="$(cat "$TSV" "$W/.github/SCORE.md" | md5sum)"
[ "$rc" = 2 ] && [ "$m0" = "$m1" ] && grep -q 'criterion-changed' <<<"$out" && ck ok "(e2) write --dry-run moving Arizona's denominator without a stamp REFUSED rc=2 before any write, both files byte-identical" || ck no "(e2) rc=$rc identical=$([ "$m0" = "$m1" ] && echo yes || echo no) -- got: $(tail -2 <<<"$out" | cut -c1-200)"
out="$(S4E_HOME="$W" python3 "$HELPER" write --lang icon --column vendor --suite Arizona --text 'Arizona: m3 46/124 · m4 46/124 (`test_icon_arizona_suite.sh`)' --measurer coo --dry-run --criterion-changed '2026-09-16:gate fixture preview' 2>&1)"; rc=$?
[ "$rc" = 0 ] && grep -q 'WOULD' <<<"$out" && ck ok "(e3) the same --dry-run with the stamp previews rc=0" || ck no "(e3) rc=$rc -- got: $(tail -2 <<<"$out" | cut -c1-200)"
echo "population: $checks arm(s) graded, $fails FAIL"
[ "$fails" = 0 ] && { echo "GATE PASS [score_row_criterion_change_is_stamped]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [score_row_criterion_change_is_stamped]: $fails of $checks arms FAIL"; exit 1
