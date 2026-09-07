#!/usr/bin/env bash
# test_gate_sno_package_runners_append_progress_rows.sh -- THE SNOBOL4 PACKAGE RUNNERS RECORD ONE PROGRESS ROW PER
# PROGRAM PER MODE, ON THE STREAM-EQUAL BASIS, AND NEVER FROM A SCRATCH SUITE INTO THE LIVE TABLE.
# Row snobol4-snoflake-aisnobol-and-dotnet-runners-wired-onto-lib-inventory-with-their-sidecars (coo, 2026-09-07;
# CEO-383 rulings 2 and 3: "the measure counts only runner-appended rows, so the runner is defective, not the flip";
# a board published under an inventory REFUSE(2) is void). Before this landing test_snoflake_suite.sh and
# test_snobol4_dotnet_suite.sh appended NOTHING (util_progress_flips.py --coverage: snoflake MISSING, dotnet MISSING),
# so Flake's 131 -> 134 was unpaid and invisible; aisnobol records through corpus_suite_harness.py already.
# HERMETIC: S4E_PROGRESS_DB points every recording arm at a scratch table under mktemp; the suites are scratch copies
# (SNOFLAKE_SUITE / DOTNET_SUITE), so the live leaderboard and the live table are never touched. Needs this tree's
# built binary (util_require_fresh refuses rc=2 on a stale or absent one) and the sbl -bf oracle.
#   1  snoflake, scratch suite + scratch table: exactly 6 rows (3 fixtures x 2 modes)
#   2  a byte-equal fixture is PASS in both modes
#   3  an error-number-only fixture is UNGRADED (never PASS) in mode 3 -- the CEO-383 basis
#   4  the board prints the table's reading: SNOFLAKE_BOARD ... both_modes_stream_pass=1
#   5  the inventory does not refuse: PACKAGE_INVENTORY ... graded_narrow=2 (NARROW.tsv names both narrow fixtures)
#   6  the same scratch suite WITHOUT S4E_PROGRESS_DB records nothing (says NOT recorded) and writes no SCORE.md cell
#   7  the live table is exactly as long after arm 6 as before it
#   8  dotnet, scratch suite + scratch table: exactly 4 rows (2 programs x 2 modes), all PASS
#   9  dotnet prints the table's reading: DOTNET_AND both_modes_pass=2/2
# Fail-once (2026-09-07 09:0x CDT, coo, measured): with the pre-landing runners restored from scratch copies, 7 of 9 arms red
# (1-4, 6, 8, 9); arm 5 stays green because the scratch NARROW.tsv already names both narrow fixtures, arm 7 by construction.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
G=sno_package_runners_append_progress_rows
SNO="$S4E/corpus/packages/snobol4/snoflake_suite"; DOT="$S4E/corpus/packages/snobol4/dotnet"
[ -d "$SNO" ] && [ -d "$DOT" ] || { echo "GATE UNPROVEN(2) [$G]: package dirs missing under $S4E/corpus/packages/snobol4"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_gate_$G "$ROOT/scrip" "$ROOT/out/libscrip_rt.so" >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$G]: this tree's binary is stale or unbuilt -- run make"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "GATE UNPROVEN(2) [$G]: lib_oracle_flags.sh unloadable"; exit 2; }
[ -x "$(sbl_correctness_bin)" ] || { echo "GATE UNPROVEN(2) [$G]: sbl -bf oracle absent"; exit 2; }
LIVE_DB="${S4E_PROGRESS_DB:-/home/resources/progress/results.tsv}"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_pkgprog.XXXXXX")" || { echo "GATE UNPROVEN(2) [$G]: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
PASS=0; FAIL=0; red() { FAIL=$((FAIL+1)); echo "  ⛔ arm $1 RED: $2"; }; ok() { PASS=$((PASS+1)); echo "  ✅ arm $1: $2"; }
rows() { awk -F'\t' 'NR>1 && NF>=10' "$1" 2>/dev/null | wc -l | tr -d ' '; }
outcome() { awk -F'\t' -v p="$2" -v m="$3" 'NR>1 && $8==p && $9==m {print $10}' "$1" 2>/dev/null | tail -1; }
# --- snoflake scratch suite: one byte-equal fixture, two error-number-only fixtures, the gimpel includes, a NARROW.tsv naming exactly the two
mkdir -p "$W/sno"; for f in indirect-real-illegal-type stlimit-nonnegative gimpel-general-purpose-macro; do
    [ -f "$SNO/$f.sno" ] || { echo "GATE UNPROVEN(2) [$G]: fixture $f.sno missing from $SNO"; exit 2; }; cp "$SNO/$f.sno" "$W/sno/"; done
cp -r "$SNO/gimpel" "$W/sno/gimpel"
printf 'gimpel-general-purpose-macro.sno\tERRNO\terror compared by number only (m3)\nstlimit-nonnegative.sno\tERRNO\terror compared by number only (m3)\n' > "$W/sno/NARROW.tsv"
OUT1="$(cd "$ROOT" && S4E_PROGRESS_DB="$W/db1.tsv" SNOFLAKE_SUITE="$W/sno" ARM_CSN=0 timeout 300 bash "$HERE/test_snoflake_suite.sh" 2>&1)"
n="$(rows "$W/db1.tsv")"; [ "$n" = 6 ] && ok 1 "6 rows recorded in the scratch table" || red 1 "expected 6 rows in $W/db1.tsv, got $n"
o3="$(outcome "$W/db1.tsv" indirect-real-illegal-type m3)"; o4="$(outcome "$W/db1.tsv" indirect-real-illegal-type m4)"
[ "$o3" = PASS ] && [ "$o4" = PASS ] && ok 2 "byte-equal fixture PASS in both modes" || red 2 "indirect-real-illegal-type m3=$o3 m4=$o4 (want PASS/PASS)"
e3="$(outcome "$W/db1.tsv" stlimit-nonnegative m3)"; g3="$(outcome "$W/db1.tsv" gimpel-general-purpose-macro m3)"
[ "$e3" = UNGRADED ] && [ "$g3" = UNGRADED ] && ok 3 "error-number-only fixtures UNGRADED in mode 3" || red 3 "stlimit-nonnegative m3=$e3 gimpel-general-purpose-macro m3=$g3 (want UNGRADED)"
printf '%s\n' "$OUT1" | grep -qE '^SNOFLAKE_BOARD total=3 .*both_modes_stream_pass=1( |$)' && ok 4 "SNOFLAKE_BOARD states both_modes_stream_pass=1" || red 4 "no SNOFLAKE_BOARD line with both_modes_stream_pass=1"
if printf '%s\n' "$OUT1" | grep -q 'INVENTORY REFUSES'; then red 5 "the inventory refused on the scratch suite"; else
    printf '%s\n' "$OUT1" | grep -qE '^PACKAGE_INVENTORY package=snoflake_suite .*graded_narrow=2( |$)' && ok 5 "PACKAGE_INVENTORY graded_narrow=2, no refusal" || red 5 "no PACKAGE_INVENTORY line with graded_narrow=2"; fi
# --- the scratch suite without a scratch table must record nothing, anywhere
before="$(rows "$LIVE_DB")"
OUT6="$(cd "$ROOT" && env -u S4E_PROGRESS_DB SNOFLAKE_SUITE="$W/sno" ARM_CSN=0 timeout 300 bash "$HERE/test_snoflake_suite.sh" 2>&1)"
after="$(rows "$LIVE_DB")"
if printf '%s\n' "$OUT6" | grep -q 'NOT recorded' && printf '%s\n' "$OUT6" | grep -q 'SCORE.md: scratch suite'; then ok 6 "scratch suite says NOT recorded and writes no SCORE.md cell"; else red 6 "scratch suite without S4E_PROGRESS_DB did not say NOT recorded / SCORE.md not written"; fi
[ "$before" = "$after" ] && ok 7 "live table unchanged ($before rows)" || red 7 "live table grew $before -> $after during a scratch run"
# --- dotnet scratch suite: two programs that answer byte-equal to the oracle in both modes
mkdir -p "$W/dot"; for f in chap8_funcs pattern_demo; do
    [ -f "$DOT/$f.sno" ] || { echo "GATE UNPROVEN(2) [$G]: program $f.sno missing from $DOT"; exit 2; }; cp "$DOT/$f.sno" "$W/dot/"; for e in IN in input; do [ -f "$DOT/$f.$e" ] && cp "$DOT/$f.$e" "$W/dot/"; done; done
OUT8="$(cd "$ROOT" && S4E_PROGRESS_DB="$W/db2.tsv" DOTNET_SUITE="$W/dot" timeout 300 bash "$HERE/test_snobol4_dotnet_suite.sh" 2>&1)"
n="$(rows "$W/db2.tsv")"; np="$(awk -F'\t' 'NR>1 && $10=="PASS"' "$W/db2.tsv" 2>/dev/null | wc -l | tr -d ' ')"
[ "$n" = 4 ] && [ "$np" = 4 ] && ok 8 "4 dotnet rows, all PASS" || red 8 "dotnet rows=$n pass=$np (want 4/4)"
printf '%s\n' "$OUT8" | grep -qE '^DOTNET_AND both_modes_pass=2/2( |$)' && ok 9 "DOTNET_AND both_modes_pass=2/2" || red 9 "no DOTNET_AND both_modes_pass=2/2 line"
echo "------------------------------------------------------------"
if [ "$FAIL" = 0 ]; then echo "GATE PASS(0) [$G]: $PASS of 9 arms green (denominator: 9 arms over 3 snoflake fixtures + 2 dotnet programs, scratch table + scratch suites)"; exit 0
else echo "GATE FAIL(1) [$G]: $FAIL of 9 arms red"; exit 1; fi
