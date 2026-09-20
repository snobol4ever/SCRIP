#!/usr/bin/env bash
export S4E_ONE_RUNNER_OVERRIDE="gate arm ${0##*/}: the snocone master graded as this lane's OWN board under CEO-775 (LANES: snocone=hq_snocone)"
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_snocone_gc_share_named_and_master_clean_under_forced_collection.sh
#
# THE DONE-WHEN of row snocone-gc-the-snocone-share-of-the-unmapped-slot-population-censused-by-name-and-the-
# master-clean-under-forced-collection (hq_snocone, 2026-09-20), which is this lane's FIRST row under MODE TENET:
# MODE line 2 / CEO-1011 -- "COMPLETENESS DOES NOT OPEN IN A LANGUAGE UNTIL THAT LANGUAGE'S GC SHARE IS MEASURED
# CLEAN BY ORACLE DIFF AT THE TINY ARENA", the census NAMING and never counting, the master graded by ORACLE and
# never by rc. ⛔ RED ON ORIGIN TODAY at ARM 3 (one entry, eval_datatype_defer_1) -- see the gc gate beside it.
#
# ⛔⭐ ARM 1 IS THE ARM THAT MAKES THE OTHER TWO MEAN ANYTHING, AND IT IS HERE BECAUSE THE OBVIOUS READING WAS
# ALREADY WRONG ONCE THIS SITTING. The SncM master reads 336/336 both modes at SCRIP_HEAP_MB=1 -- and a census of
# [ZGC] regeneration lines over the same 336 entries at that same arena reads COLLECTORS=0, NON-COLLECTORS=336,
# TOTAL REGENERATIONS=0. Not one entry allocates a megabyte, so the tiny arena never collects and that green
# board is a statement about this corpus, not about the collector. A board over a population that CANNOT fail the
# way you are asking about is not evidence, and no rc, no denominator and no FAIL=0 anywhere in it says so. So
# this gate REFUSES rc=2 -- never reports green -- when the graded population did not collect.
#
# ARM 2 is the unmapped-store census over the same population. ⛔ IT IS READ BY ITS PRINTED VERDICT AND NOT BY ITS
# rc: that census exits 1 whenever it names ANY member, and snocone's members are all BELOW-REGION, which its own
# header says is where spine words live BY CONSTRUCTION and was never the discriminator (CFO-114/CFO-136). What
# this lane owes is class 2, NO-MAP -- the per-language class the cto assigned to the HQs in writing -- so NO-MAP
# is the number this arm binds on, and the BELOW-REGION members are printed, named and attributed to the cto's
# classes 1 and 4 rather than silently counted into a pass.
#
# COST ~5 min (extract 336 entries, collect-census them, then the two-mode board). This is a LANE gate, run by
# hq_snocone per landing; it is deliberately NOT in `make test` -- the blocking set is the shared resource that
# does not scale with seats (MODE line 2 CONDITION 2) and wiring a five-minute arm into it is the coo's call.
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP_DIR="$S4E/SCRIP"
SCRIP="${SCRIP:-$SCRIP_DIR/scrip}"
M="$S4E/corpus/tests/snocone"
STRESS="${SNC_GC_STRESS:-1}"
ARENA="${SNC_GC_ARENA:-1}"
fail() { echo "⛔ RED: $*" >&2; exit 1; }
refuse() { echo "REFUSES rc=2: $*" >&2; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip not built at $SCRIP -- cannot measure"
for f in ALL.sc ALL.ref ALL.csv; do [ -f "$M/$f" ] || refuse "master file $M/$f missing -- cannot measure"; done
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cd "$SCRIP_DIR" || refuse "cannot cd to $SCRIP_DIR"
# --- materialize the graded population OUT of the master, by ORIGIN, never by a filename glob ---
mkdir -p "$W/e"
rows=0; got=0
while IFS= read -r org; do
    [ -n "$org" ] || continue
    rows=$((rows + 1))
    python3 "$HERE/corpus_suite_harness.py" extract "$M/ALL.sc" "$M/ALL.ref" --origin "$org" \
        "$W/e/$org.sc" --out-ref "$W/e/$org.ref" --out-in "$W/e/$org.in" >/dev/null 2>&1 && got=$((got + 1))
done < <(awk -F, 'NR>1{print $3}' "$M/ALL.csv")
[ "$rows" -gt 0 ] || refuse "ALL.csv carries no entry rows -- a census over an empty population reads zero by never looking"
[ "$got" -eq "$rows" ] || refuse "materialized $got of $rows entries out of the master -- a population that did not come out whole is not the population the board grades"
# --- ARM 1: DECIDABILITY. did the graded population collect at all? ---
coll=0; regens=0
for f in "$W/e"/*.sc; do
    b="${f%.sc}"; in="$b.in"; [ -f "$in" ] || in=/dev/null
    n=$(SCRIP_ZETA_TELEM=1 SCRIP_HEAP_MB="$ARENA" SCRIP_GC_STRESS="$STRESS" timeout 20 "$SCRIP" "$f" < "$in" 2>&1 >/dev/null | grep -c '^\[ZGC\] regeneration')
    [ "$n" -gt 0 ] && { coll=$((coll + 1)); regens=$((regens + n)); }
done
echo "ARM 1 DECIDABILITY arena_mb=$ARENA stress=$STRESS entries=$rows collectors=$coll non_collectors=$((rows - coll)) regenerations=$regens"
[ "$coll" -gt 0 ] || refuse "ZERO of $rows entries collected at arena_mb=$ARENA stress=$STRESS -- the board below would be a statement about this corpus and not about the collector, so it is not reported at all"
# --- ARM 2: the unmapped-store census, by NAME, over that same population ---
python3 "$HERE/util_gc_unmapped_store_census.py" "$W/e"/*.sc > "$W/census.txt" 2>&1
crc=$?
[ "$crc" -eq 2 ] && { sed -n '/REFUSED/p' "$W/census.txt" >&2; refuse "the unmapped-store census could not measure this population"; }
line="$(grep -m1 '^CENSUS unmapped-store witnesses=' "$W/census.txt")" || refuse "the census printed no verdict line -- nothing to read"
echo "ARM 2 $line"
grep -c '^CENSUS unmapped-store MEMBER' "$W/census.txt" | sed 's/^/ARM 2 members NAMED in full by the census: /'
nomap="$(printf '%s\n' "$line" | grep -o 'NO-MAP=[0-9]*' | cut -d= -f2)"
[ -n "$nomap" ] || refuse "the census verdict line carries no NO-MAP field -- the criterion cannot be read off it"
[ "$nomap" -eq 0 ] || fail "NO-MAP=$nomap -- this lane owns class 2 (a graph the compiler refuses a frame layout for has no map at all), and $nomap site(s) are in it: $(grep -m5 'MEMBER NO-MAP' "$W/census.txt")"
# --- ARM 3: the master graded against its ORACLE-CUT refs, under forced collection, both modes ---
out="$(SCRIP_HEAP_MB="$ARENA" SCRIP_GC_STRESS="$STRESS" timeout 3000 python3 "$HERE/corpus_suite_harness.py" run "$M/ALL.sc" "$M/ALL.ref" --lang snocone --modes m3,m4 2>&1)"
board="$(printf '%s\n' "$out" | grep -m1 '^SUITE_BOARD')" || { printf '%s\n' "$out" | tail -5 >&2; refuse "the master run printed no SUITE_BOARD line -- a run with no board is not a measurement"; }
echo "ARM 3 $board"
tot=$(printf '%s\n' "$board" | grep -o 'total=[0-9]*' | cut -d= -f2)
[ "${tot:-0}" -gt 0 ] || refuse "the board's own denominator is $tot -- a FAIL=0 over nothing is not a reading"
bad=0
for k in m3_fail m3_crash m3_hang m3_unproven m4_fail m4_crash m4_hang m4_unproven; do
    v=$(printf '%s\n' "$board" | grep -o "$k=[0-9]*" | cut -d= -f2)
    [ "${v:-0}" -eq 0 ] || { echo "  $k=$v"; bad=$((bad + v)); }
done
[ "$bad" -eq 0 ] || fail "$bad non-pass verdict(s) over the printed denominator $tot at arena_mb=$ARENA stress=$STRESS -- NAMED: $(printf '%s\n' "$out" | grep -E '^ +(FAIL|CRASH|HANG|UNPROVEN) ' | awk '{print $2"/"$3}' | sort -u | tr '\n' ' ')"
echo "✅ snocone GC share named (NO-MAP=0) and the master is clean over $tot entries under forced collection"
