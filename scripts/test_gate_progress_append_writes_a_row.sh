#!/usr/bin/env bash
# test_gate_progress_append_writes_a_row.sh -- THE PROGRESS DATABASE IS WRITTEN BY EVERY CANONICAL SUITE RUN, AND
# NEVER BY A SCRATCH ONE (Lon 2026-09-06: "enhance your database to allow for time based queries. Get it fixed.";
# GOAL-CEO CEO-331; the rule is /home/resources/progress/README.md, CEO-319). Hermetic: S4E_PROGRESS_DB points every
# arm at a scratch table under mktemp; the live table is never touched. Arms 5-7 grade a real one-entry suite, so
# they need this tree's built binary (util_require_fresh refuses rc=2 on a stale or absent one).
#   1  the writer appends a 13-field row whose ts_utc is the run's own clock (within 300 s of now, UTC)
#   2  an unwritable table is a LOUD refusal, rc=2, and says NOT recorded
#   3  S4E_PROGRESS_OFF=1 records nothing and says so
#   4  the harness on a NON-canonical suite path (a scratch copy) records nothing
#   5  the harness on a canonical master path (corpus/tests/<lang>/ALL.<ext>) records one row per entry per mode, class=master
#   6  the harness on a canonical package path (corpus/packages/<lang>/<pkg>/ALL.<ext>) records class=package with the SUITES.tsv key
#   7  the query tool sees the flip: a FAIL reading then a PASS reading of the same program is one newly-passing program
# Fail-once proof (2026-09-06, ceo): with util_progress_append.py's append_rows body replaced by `return 0`, arms 1, 5, 6, 7 red.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
H="$HERE/corpus_suite_harness.py"; PY="$HERE/util_progress_append.py"; Q="$S4E/.github/scripts/util_progress_flips.py"
[ -f "$H" ] && [ -f "$PY" ] || { echo "GATE UNPROVEN(2) [progress_append_writes_a_row]: harness or writer missing under $HERE"; exit 2; }
[ -f "$Q" ] || { echo "GATE UNPROVEN(2) [progress_append_writes_a_row]: query tool missing at $Q (pull .github)"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_gate_progress_append_writes_a_row >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [progress_append_writes_a_row]: this tree's binary is stale or unbuilt -- arms 5-7 run a real program. Run 'make'."; exit 2; }
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_progress.XXXXXX")" || { echo "GATE UNPROVEN(2): mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
export S4E_PROGRESS_DB="$W/db.tsv"
unset S4E_PROGRESS_OFF
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: every canonical suite run writes the progress database; scratch runs never do ==="
echo "--- ARM 1: the writer appends a row with the run's own UTC clock ---"
now=$(date -u +%s)
python3 "$PY" append --class master --suite snobol4-master --lang snobol4 --program gate_probe --mode m3 --outcome FAIL >/dev/null 2>&1; rc=$?
[ "$rc" = 0 ] && ck ok "append rc=0" || ck no "append rc=$rc"
row=$(tail -1 "$W/db.tsv"); nf=$(printf '%s\n' "$row" | awk -F'\t' '{print NF}')
[ "$nf" = 13 ] && ck ok "row has 13 fields" || ck no "row has $nf fields: $row"   # 12 -> 13 on 2026-09-06 (hq_T, CEO-338): the `fingerprint` column. ⭐ THIS ARM CAUGHT THE SCHEMA CHANGE THE MOMENT IT LANDED, which is the arm doing its job -- the count is updated in the SAME commit as the column, never after, because a field-count assertion that lags its schema is a red on origin that every seat learns to step over.
ts=$(printf '%s' "$row" | cut -f1); tse=$(date -u -d "${ts}Z" +%s 2>/dev/null || echo 0); d=$(( tse - now ))
[ "$d" -ge -300 ] && [ "$d" -le 300 ] && ck ok "ts_utc $ts is the run's own clock (delta ${d}s)" || ck no "ts_utc $ts is not now (delta ${d}s)"
[ "$(printf '%s' "$row" | cut -f4)" != "" ] && [ "$(printf '%s' "$row" | cut -f4)" != "unknown-seat" ] && ck ok "measurer is $(printf '%s' "$row" | cut -f4)" || ck no "measurer empty or placeholder"
echo "--- ARM 2: an unwritable table refuses rc=2, loudly ---"
out=$(S4E_PROGRESS_DB=/nonexistent-dir-$$/db.tsv python3 "$PY" append --class master --suite s --lang l --program p --mode m3 --outcome PASS 2>&1); rc=$?
[ "$rc" = 2 ] && ck ok "unwritable -> rc=2" || ck no "unwritable -> rc=$rc"
grep -q 'NOT recorded' <<<"$out" && ck ok "the refusal says NOT recorded" || ck no "refusal silent: $out"
echo "--- ARM 3: S4E_PROGRESS_OFF=1 records nothing and says so ---"
n0=$(wc -l < "$W/db.tsv")
out=$(S4E_PROGRESS_OFF=1 python3 "$PY" append --class master --suite s --lang l --program p --mode m3 --outcome PASS 2>&1); rc=$?
[ "$rc" = 0 ] && [ "$(wc -l < "$W/db.tsv")" = "$n0" ] && ck ok "OFF: rc=0, no row" || ck no "OFF: rc=$rc rows $(wc -l < "$W/db.tsv") (was $n0)"
grep -q 'NOT recorded' <<<"$out" && ck ok "OFF says NOT recorded" || ck no "OFF is silent: $out"
echo "--- ARM 4: the harness on a scratch (non-canonical) copy records nothing ---"
mkdir -p "$W/scratch" "$W/corpus/tests/snobol4" "$W/corpus/packages/snobol4/gimpel"
printf " OUTPUT = 'alive';END;* gate_alive\n" > "$W/scratch/ALL.sno"; printf 'alive\n' > "$W/scratch/ALL.ref"
n0=$(wc -l < "$W/db.tsv")
python3 "$H" run "$W/scratch/ALL.sno" "$W/scratch/ALL.ref" --modes m3 >/dev/null 2>&1; rc=$?
[ "$rc" = 0 ] && ck ok "scratch suite grades green (rc=0)" || ck no "scratch suite rc=$rc"
[ "$(wc -l < "$W/db.tsv")" = "$n0" ] && ck ok "scratch path recorded nothing" || ck no "scratch path recorded $(( $(wc -l < "$W/db.tsv") - n0 )) row(s)"
echo "--- ARM 5: a canonical master path records one row per entry per mode, class=master ---"
cp "$W/scratch/ALL.sno" "$W/corpus/tests/snobol4/ALL.sno"; cp "$W/scratch/ALL.ref" "$W/corpus/tests/snobol4/ALL.ref"
n0=$(wc -l < "$W/db.tsv")
out=$(python3 "$H" run "$W/corpus/tests/snobol4/ALL.sno" "$W/corpus/tests/snobol4/ALL.ref" --modes m3,m4 2>&1); rc=$?
added=$(( $(wc -l < "$W/db.tsv") - n0 ))
[ "$added" = 2 ] && ck ok "master path: 2 rows (m3, m4) for 1 entry" || ck no "master path: $added row(s) added (rc=$rc): $(tail -c 300 <<<"$out")"
grep -q 'PROGRESS_RECORDED suite=snobol4-master class=master rows=2' <<<"$out" && ck ok "the run says PROGRESS_RECORDED" || ck no "no PROGRESS_RECORDED line: $(tail -c 200 <<<"$out")"
tail -1 "$W/db.tsv" | awk -F'\t' '$5=="master" && $6=="snobol4-master" && $7=="snobol4" && $8=="gate_alive" && $10=="PASS"' | grep -q . && ck ok "row carries class/suite/lang/program/outcome" || ck no "row fields wrong: $(tail -1 "$W/db.tsv")"
echo "--- ARM 6: a canonical package path records class=package under the SUITES.tsv key ---"
cp "$W/scratch/ALL.sno" "$W/corpus/packages/snobol4/gimpel/ALL.sno"; cp "$W/scratch/ALL.ref" "$W/corpus/packages/snobol4/gimpel/ALL.ref"
python3 "$H" run "$W/corpus/packages/snobol4/gimpel/ALL.sno" "$W/corpus/packages/snobol4/gimpel/ALL.ref" --modes m3 >/dev/null 2>&1
tail -1 "$W/db.tsv" | awk -F'\t' '$5=="package" && $6=="gimpel" && $9=="m3"' | grep -q . && ck ok "package path: class=package suite=gimpel" || ck no "package row wrong: $(tail -1 "$W/db.tsv")"
echo "--- ARM 7: the query tool sees the flip ---"
python3 "$PY" append --class master --suite snobol4-master --lang snobol4 --program gate_probe --mode m3 --outcome PASS >/dev/null 2>&1
out=$(python3 "$Q" --db "$W/db.tsv" --since 1h --per hour --mode m3 2>&1)
grep -qE 'TOTAL newly-passing in window: master 1,' <<<"$out" && ck ok "gate_probe FAIL->PASS counted as 1 newly-passing master program" || ck no "flip not seen: $out"
out=$(python3 "$Q" --db "$W/db.tsv" --register --program gate_probe 2>&1)
grep -qP '^snobol4-master\tgate_probe\tmaster\tsnobol4\tWORKING\t20' <<<"$out" && ck ok "register: gate_probe WORKING with its began_working_utc" || ck no "register wrong: $out"
GATE_EXAMINED="$checks arms"
gate_verdict "$fails" "progress-database arm(s) red -- a suite run that leaves the table untouched, or a scratch run that writes it"
