#!/usr/bin/env bash
# test_gate_from_progress_assembles_a_package_as_shipped_minus_excluded.sh -- THE ASSEMBLED PACKAGE ROW IS THE RUNNER'S ROW (ceo CEO-1802/1803; Lon
# 2026-10-15 via the ceo, verbatim: "Why is not the suite board a list of tests and not a list of suites? Then each set gets updated in round robin.").
# A package read a chunk at a time has its suite row ASSEMBLED by util_score_row.py from-progress from the latest per-program readings, and that row
# must be the one lib_container_package_runner.sh publishes from a whole run: shipped minus EXCLUDED, so shipped = denominator + Excl.
# THE FIXTURE (a scratch corpus and progress DB; the real table and DB are never read or written): package zzpkg ships a b c d e f g (.pl); the
# container ALL.pl/ALL.csv grades a b c d; e is shipped and ungraded (debt, no reading needed); f is an EXCLUDED.tsv row (NO_ORACLE_ANSWER); g is
# a CONTAINERS.tsv fragment. Readings: a, c and d PASS in both modes; b PASS m3, FAIL m4; d's m4 reading is named d.pl (GNU's spelling).
#   ARM 1  the row is 3/5 with excluded=2: pass 3 (a c d), denominator {a b c d e} = 5, Excl = 1 EXCLUDED row + 1 fragment.
#   ARM 2  SUITE_ROW_PACKAGE prints shipped=7 table=4 excluded=2 denominator=5 debt_unread_by_design=1.
#   ARM 3  a master key is untouched: the rebus rungs fixture keys its total on ALL.csv alone (no SUITE_ROW_PACKAGE line).
# PROVEN TO FAIL on the assembler before this rule (origin 717ddfca3's util_score_row.py): arms 1 and 2 FAIL -- it reads 3/4 with no excluded= (ALL.csv's count; d.pl unmatched, so d is half read).
# EXIT 0 every arm holds; 1 an arm failed; 2 could not measure.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SR="${FROM_PROGRESS_SR:-$HERE/util_score_row.py}"
[ -f "$SR" ] || { echo "GATE REFUSE(2): $SR is missing"; exit 2; }
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
P="$T/corpus/packages/prolog/zzpkg"; mkdir -p "$P" "$T/corpus/tests/rebus"
for n in a b c d e f g; do printf ':- initialization(main).\nmain :- write(%s), nl.\n' "$n" > "$P/$n.pl"; done
printf 'all\n' > "$P/ALL.pl"; printf 'a\nb\nc\nd\n' > "$P/ALL.ref"
printf 'rank,entry,origin\n1,a,x\n2,b,x\n3,c,x\n4,d,x\n' > "$P/ALL.csv"
printf '# EXCLUDED.tsv\nf.pl\tNO_ORACLE_ANSWER\tthe oracle prints nothing\n' > "$P/EXCLUDED.tsv"
printf '# CONTAINERS.tsv\ng.pl\tINCLUDED_BY\ta.pl\n' > "$P/CONTAINERS.tsv"
printf 'rank,entry,origin\n1,r1,x\n2,r2,x\n' > "$T/corpus/tests/rebus/ALL.csv"
DB="$T/results.tsv"
printf 'ts_utc\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\tsecs\tnote\tfingerprint\tconfig\n' > "$DB"
row() { printf '2026-10-15T20:00:%02d\tabc1234\tc0ffee0\tcoo\tpackage\t%s\t%s\t%s\t%s\t%s\t1.0\t\t\tpartition=chunk:01\n' "$1" "$2" "$3" "$4" "$5" "$6" >> "$DB"; }
row 1 zzpkg prolog a m3 PASS; row 2 zzpkg prolog a m4 PASS; row 3 zzpkg prolog b m3 PASS; row 4 zzpkg prolog b m4 FAIL
row 5 zzpkg prolog c m3 PASS; row 6 zzpkg prolog c m4 PASS; row 7 zzpkg prolog d m3 PASS; row 8 zzpkg prolog d.pl m4 PASS
row 9 rebus-rungs rebus r1 m3 PASS; row 10 rebus-rungs rebus r1 m4 PASS; row 11 rebus-rungs rebus r2 m3 PASS; row 12 rebus-rungs rebus r2 m4 PASS
fails=0; n=0
ck() { n=$((n+1)); if eval "$2"; then echo "  ok   $1"; else fails=$((fails+1)); echo "  FAIL $1"; fi; }
o1="$(S4E_CORPUS_ROOT="$T/corpus" S4E_PROGRESS_DB="$DB" python3 "$SR" from-progress --suite zzpkg --dry-run 2>&1)"; r1=$?
o3="$(S4E_CORPUS_ROOT="$T/corpus" S4E_PROGRESS_DB="$DB" python3 "$SR" from-progress --suite reb-rungs --dry-run 2>&1)"; r3=$?
[ "$r1" = 0 ] && [ "$r3" = 0 ] || { printf '%s\n%s\n' "$o1" "$o3" | tail -6; echo "GATE REFUSE(2): from-progress --dry-run did not run (rc $r1, $r3)"; exit 2; }
ck "1 the package row is 3/5 excluded=2 (shipped minus EXCLUDED, d.pl matched to d)" 'grep -q "would set zzpkg -> 3/5 excluded=2 " <<<"$o1"'
ck "2 SUITE_ROW_PACKAGE shipped=7 table=4 excluded=2 denominator=5 debt_unread_by_design=1" 'grep -q "^SUITE_ROW_PACKAGE suite=zzpkg shipped=7 table=4 excluded_rows=1 containers=1 excluded=2 denominator=5 debt_unread_by_design=1$" <<<"$o1"'
ck "3 a master key keeps ALL.csv as its population (reb-rungs 2/2, no SUITE_ROW_PACKAGE)" 'grep -q "would set reb-rungs -> 2/2 stamped" <<<"$o3" && ! grep -q "^SUITE_ROW_PACKAGE" <<<"$o3"'
[ "$fails" = 0 ] || printf '%s\n' "$o1" | grep -E '^SUITE_ROW|would set' | sed 's/^/      /'
if [ "$fails" = 0 ]; then echo "✅ GATE PASS [from_progress_assembles_a_package_as_shipped_minus_excluded]: $n of $n arms hold"; exit 0; fi
echo "⛔ GATE FAIL [from_progress_assembles_a_package_as_shipped_minus_excluded]: $fails of $n arm(s) failed"; exit 1
