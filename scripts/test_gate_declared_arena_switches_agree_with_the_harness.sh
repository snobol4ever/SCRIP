#!/usr/bin/env bash
# test_gate_declared_arena_switches_agree_with_the_harness.sh -- A DECLARED HEAP AND STACK BECOME THE SAME SWITCHES ON EVERY ROAD (row
# instruments-ninety-two-scripts-and-the-makefile-size-the-arena-through-the-env-not-the-d-switch, the coo; CEO-1225/1226: SCRIP sizes the
# heap with -d and -i and the stack with -s, and a switch is recorded with the run where an environment knob is invisible).
#
# THE TWO SPELLINGS: corpus_suite_harness.py's _size_switches(heap_kb, stack_kb) for the masters, and lib_declared_arena.sh's
# declared_arena_switches / declared_switches_from_table for the shell runners. A runner that moves off the SCRIP_HEAP_KB export puts the
# lib's words on its own command line; if the two spellings drifted, the same declaration would size two boards differently.
# ARMS (hermetic: a fixture ALL.csv under mktemp; one scrip run for the last arm):
#   1 for heap only, stack only, both and neither, the lib's words equal the harness's list joined by spaces -- in both lib functions
#   2 a declared heap of 8192 KB run with the lib's switches and with the old SCRIP_HEAP_KB=8192 export reports the SAME window and the SAME
#     cap on the runtime's own arena line (SCRIP_GC_EXERCISE=1): the conversion moves the arena into the transcript and nothing else
#   3 a cell the lib refuses (heap_kb below the runtime's floor) refuses rc 2 through the switch function too, never reads as nothing
# FAIL_ONCE=1 drops the -i word from the lib's heap spelling before arm 1 compares, to prove the arm trips.
# rc 0 every arm holds; rc 1 a FAIL named; rc 2 REFUSED-TO-GRADE (no binary, no harness).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -f "$HERE/corpus_suite_harness.py" ] || refuse "no harness"
[ -x "$ROOT/scrip" ] || refuse "no ./scrip -- make first"
"$HERE/util_require_fresh.sh" --gate test_gate_declared_arena_switches_agree_with_the_harness >/dev/null 2>&1 || refuse "this tree's binary is stale -- run make"
. "$HERE/lib_declared_arena.sh" || refuse "cannot source lib_declared_arena.sh"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_arena_switches.XXXXXX")" || refuse "mktemp failed"; trap 'rm -rf "$W"' EXIT
fails=0; checks=0; ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: a declared heap and stack become the same switches in the lib and the harness ==="
printf 'rank,entry,origin,package,heap_kb,stack_kb\n1,heap_only,x,p,8192,\n2,stack_only,x,p,,131072\n3,both,x,p,16384,262144\n4,neither,x,p,,\n' > "$W/ALL.csv"
printf 'rank,entry,origin,package,heap_kb,stack_kb\n1,refused,x,p,16,\n' > "$W/REFUSED.csv"
tbl="$W/table.tsv"; declared_memory_table "$W/ALL.csv" > "$tbl" 2>/dev/null
bad=""
for e in heap_only stack_only both neither; do
  h=$(awk -F, -v e="$e" '$2==e{print $5}' "$W/ALL.csv"); s=$(awk -F, -v e="$e" '$2==e{print $6}' "$W/ALL.csv")
  want=$(cd "$HERE" && python3 -c 'import sys; import corpus_suite_harness as H; h, s = sys.argv[1], sys.argv[2]; print(" ".join(H._size_switches(int(h) if h else None, int(s) if s else None)))' "$h" "$s")
  got1=$(declared_arena_switches "$W/ALL.csv" "$e"); got2=$(declared_switches_from_table "$tbl" "$e")
  [ -n "${FAIL_ONCE:-}" ] && got1="${got1/ -i${h}k/}"
  [ "$got1" = "$want" ] && [ "$got2" = "$want" ] || bad="$bad $e(harness '$want' lib '$got1' table '$got2')"
done
[ -z "$bad" ] && ck ok "1 heap only, stack only, both and neither: the lib's two functions spell exactly the harness's _size_switches" || ck no "1 the spellings differ:$bad"
printf '        OUTPUT = DUPL("x", 1000)\nEND\n' > "$W/p.sno"
sw=$(declared_arena_switches "$W/ALL.csv" heap_only)
a1=$(cd "$W" && SCRIP_GC_EXERCISE=1 timeout 60 "$ROOT/scrip" --run $sw p.sno 2>&1 >/dev/null | grep -oE '(arena_kb|cap_kb|reserve_mb)=[0-9]+' | sort | tr '\n' ' ')
a2=$(cd "$W" && SCRIP_GC_EXERCISE=1 SCRIP_HEAP_KB=8192 timeout 60 "$ROOT/scrip" --run p.sno 2>&1 >/dev/null | grep -oE '(arena_kb|cap_kb|reserve_mb)=[0-9]+' | sort | tr '\n' ' ')
[ -n "$a1" ] && [ "$a1" = "$a2" ] && grep -q 'arena_kb=8192' <<<"$a1" && grep -q 'cap_kb=8192' <<<"$a1" \
  && ck ok "2 heap_kb 8192 as switches ($sw) and as the old export read the same arena line: $a1" \
  || ck no "2 the arena lines differ -- switches '$sw': '$a1', export: '$a2'"
declared_arena_switches "$W/REFUSED.csv" refused >/dev/null 2>&1; rr=$?
[ "$rr" = 2 ] && ck ok "3 a refused cell (heap_kb 16) refuses rc 2 through the switch function too" || ck no "3 a refused cell returned rc $rr through declared_arena_switches"
echo "population: $checks arm(s) graded, $fails FAIL"
[ "$fails" = 0 ] && { echo "GATE PASS [declared_arena_switches_agree_with_the_harness]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [declared_arena_switches_agree_with_the_harness]: $fails of $checks arms FAIL"; exit 1
