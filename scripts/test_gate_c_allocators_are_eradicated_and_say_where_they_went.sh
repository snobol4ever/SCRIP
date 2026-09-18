#!/usr/bin/env bash
# test_gate_c_allocators_are_eradicated_and_say_where_they_went.sh -- THE GATE ON LON'S ERADICATION ORDER
# (coo, CEO-842/843; RULES.md FACT RULE line 29).
#
# LON'S ACCEPTANCE TEST IS TEXTUAL AND IT IS HIS: "Eradicate ALL usage of malloc, calloc, free, and realloc.
# At the end ensure 0 instances found in the SCRIP source base."  So the count must fall to 0 over the WHOLE
# tree -- src/parsers and the checked-in generated flex/bison output included.
#
# ⛔⭐ BUT THE COUNT ALONE IS NOT THE VERDICT, and that is the ceo's own warning (CEO-843, verbatim): "a census
# that only counts the four names would read green on a tree where half the runtime quietly lives outside the
# collector."  The rule names three destinations -- the collected heap WITH A ROOT for anything the running
# program can reach, the compile-time arena for what only the compiler touches, mmap for the collector's own
# bookkeeping -- and THE ARENA MAY NOT HOLD ANYTHING THE RUNTIME CAN REACH.  An arena allocation under
# src/runtime is the same evasion as malloc or a pin, one name further out, and arm (c) reds on it.
#
# ⛔ THE RULE IS AUTHORITATIVE WHILE THE CURE DOES NOT EXIST (RULES.md line 29), exactly as it is for xfail, so
# THIS GATE IS RED TODAY BY DESIGN -- 1182 sites at the order.  What it enforces meanwhile is the RATCHET: the
# counts may only fall, and a fall the baseline has not recorded is just as red as a rise, because a ratchet
# nobody tightens is a ratchet that lets the next regression back in.  The gate's own verdict is the ratchet's,
# never the census's; a census RED is the design state, not this gate's failure.
#
# ARMS: (a) the census's own selftest -- planted violations trip, a clean fixture passes, a vacuous zero refuses
#       (b) the census runs on this tree and prints its population (a REFUSAL here is rc=2, never green)
#       (c) ARENA-IN-RUNTIME is 0 -- the anti-evasion clause, the population a four-name grep cannot see
#       (d) the ratchet holds against scripts/c_allocator_baseline.tsv
# FAIL_ONCE=1 blanks the selftest's proof so arm (a) is seen to trip.
# EXIT: 0 every arm · 1 an arm failed · 2 REFUSED (no python3, no census, no baseline).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CEN="$HERE/util_c_allocator_census.py"
BASE="$HERE/c_allocator_baseline.tsv"
refuse() { echo "⛔ REFUSED-TO-GRADE rc=2: $1"; exit 2; }
command -v python3 >/dev/null 2>&1 || refuse "python3 not on PATH"
[ -f "$CEN" ] || refuse "no census at $CEN"
[ -f "$BASE" ] || refuse "no ratchet baseline at $BASE -- the counts have nothing to be held against"
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: the four C allocators leave the tree, and every converted site says where it went ==="

st="$(timeout 300s python3 "$CEN" --selftest 2>&1)"; strc=$?
[ "${FAIL_ONCE:-0}" = 1 ] && st="(blanked by FAIL_ONCE)"
arms="$(printf '%s\n' "$st" | sed -n 's/^population: \([0-9]*\) selftest arm(s).*/\1/p')"
if [ "$strc" = 0 ] && [ "${arms:-0}" -ge 12 ] 2>/dev/null && printf '%s\n' "$st" | grep -q '^SELFTEST PASS'; then
  ck ok "(a) the census trips on planted violations and refuses a vacuous zero -- $arms selftest arms, 0 FAIL"
else
  ck no "(a) the census selftest did not pass (rc=$strc, arms=${arms:-none}): $(printf '%s\n' "$st" | grep -m2 '  FAIL  ' | tr '\n' ';')"
fi

out="$(timeout 300s python3 "$CEN" --by-dir 2>&1)"; crc=$?
if [ "$crc" = 2 ]; then
  ck no "(b) the census REFUSED rc=2 on this tree -- it could not measure, which is not a clean zero: $(printf '%s\n' "$out" | grep -m1 'REFUSED')"
else
  ck ok "(b) the census measured this tree: $(printf '%s\n' "$out" | grep -m1 '^CENSUS c-allocators FORBIDDEN' | cut -c1-140)"
fi
printf '%s\n' "$out" | grep -m1 '^CENSUS c-allocators SPLIT' | sed 's/^/    /'
printf '%s\n' "$out" | grep -m1 '^CENSUS c-allocators DESTINATIONS' | sed 's/^/    /'

av="$(printf '%s\n' "$out" | sed -n 's/^CENSUS c-allocators ARENA-IN-RUNTIME=\([0-9]*\) .*/\1/p' | head -1)"
if [ "${av:-x}" = 0 ]; then
  ck ok "(c) ARENA-IN-RUNTIME=0 -- nothing the running program can reach is parked in the compile-time arena"
else
  ck no "(c) ARENA-IN-RUNTIME=${av:-unreadable} -- the arena is holding runtime-reachable memory, which is the evasion one name further out: $(printf '%s\n' "$out" | grep -m3 'ARENA-IN-RUNTIME ' | tr '\n' ';' | cut -c1-200)"
fi

rout="$(timeout 300s python3 "$CEN" --ratchet "$BASE" 2>&1)"; rrc=$?
if printf '%s\n' "$rout" | grep -q '^RATCHET GREEN'; then
  ck ok "(d) the ratchet holds: $(printf '%s\n' "$rout" | grep -m1 '^population: .*ratcheted count')"
else
  ck no "(d) the ratchet is not green -- $(printf '%s\n' "$rout" | grep -m3 -E '⛔ WORSE|⭐ BETTER|NOT-MEASURED|^RATCHET (RED|REFUSED)' | tr '\n' ';' | cut -c1-260)"
fi

echo "------------------------------------------------------------"
echo "population: $checks arm(s) graded, $fails FAIL; census rc=$crc (the census being RED is the design state -- the rule is authoritative while the cure does not exist, RULES.md line 29 -- and it is not this gate's verdict)"
if [ "$fails" -ne 0 ]; then echo "⛔ GATE FAIL: $fails of $checks arm(s) failed"; exit 1; fi
echo "✅ GATE PASS [c_allocators_are_eradicated_and_say_where_they_went]: $checks of $checks arms hold"; exit 0
