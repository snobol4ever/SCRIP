#!/usr/bin/env bash
# test_gate_gc_a_board_can_say_whether_the_collector_ran.sh -- ARENA_MB STATES A CONFIGURATION; THIS MEASURES THE EXERCISE.
#
# ⛔⭐ THE GAP THIS CLOSES (cfo 2026-09-21, MODE TENET, GC commitment items 1-2 INSTRUMENTED and VISIBILITY).  Lon's rule
# is that ALL GC testing runs at a tiny arena to exasperate the collector (2026-09-19 16:1x, CEO-931/934/938, RULES.md
# § THE INSTRUMENT LAWS BATCH 28 CLAUSE 2), and the fleet holds it by mechanism: the Makefile exports SCRIP_HEAP_MB ?= 1,
# the harness prints an ARENA line and an arena_mb= board field, and a gate polices the pins.  ⛔ EVERY ONE OF THOSE
# REPORTS THE CONFIGURATION AND NONE OF THEM REPORTS THE EXERCISE.  A board that prints arena_mb=1 is saying what it
# ASKED FOR, not what happened, and a tiny-arena green can come from programs the collector never ran on even once.
# MEASURED HERE, which is why this is a gate and not an opinion: hb_file_name_unrooted.icn collects ONCE at 1 MB and
# ZERO TIMES at 4 MB -- identical to the shipped 512 -- so a seat who picked 4 MB believing it "small" measured nothing
# and every instrument in the tree would have agreed with them.
#
# ⛔⛔ AND THE ARENA FIELD IN THE RUNTIME'S OWN AT-EXIT LINE WAS REPORTING A CONSTANT.  Before SCRIP <this landing>,
# rt_gcheap_report printed `arena=%dMB` from (int)GC_HEAP_MB -- a COMPILE-TIME #define of 512 -- while the window that
# actually ran is a LOCAL in rt_gcheap_init.  Measured both ways on one binary through the env switch: at SCRIP_HEAP_MB=1
# (which collected) and at 512 (which did not), the [ZHP] line read `arena=512MB` BOTH TIMES.  Under a fleet-wide law
# that every seat run at 1 MB, the one runtime line naming the arena named the wrong one, always.
#
# ⭐ WHAT THE RECEIPT IS.  SCRIP_GC_EXERCISE=1 makes rt_gcheap_report emit ONE line per program at exit --
#   [GC-EXERCISE] arena_mb= reserve_mb= stress= collections= blocks= bytes=
# -- built from the RESOLVED window (g_hp_chunk), the RESOLVED reserve, and rt_gc_runs_count(), all of which already
# existed and none of which anything read.  ONE LINE PER PROGRAM, NOT PER COLLECTION, on purpose: SCRIP_ZETA_TELEM
# already prints a [ZGC] line per regeneration, and a board cannot afford 1613 lines per entry to learn one number.
#
# ⛔ THE FAIL-ONCE IS BUILT IN RATHER THAN PLANTED, which is the discipline that matters here: a do-nothing instrument
# -- the code as it stood this morning -- passes any single-arena arm.  Arms 2 and 3 read the SAME field at THREE
# arenas and require it to TRACK, so a hardcoded 512 (the actual pre-cure behaviour, measured above) reds arm 2 and an
# absent or constant collections= reds arm 3.  FAIL_ONCE=1 additionally asserts the pre-cure expectation and must red.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
G="$(basename "${BASH_SOURCE[0]}" .sh)"
S4E="$(cd "$ROOT/.." && pwd)"
BENCH="$S4E/corpus/benchmarks/icon/bench_icnstr_concat_table.icn"
THIN="$ROOT/scripts/gc_witnesses/hb_file_name_unrooted.icn"
[ -x "$ROOT/scrip" ] || { echo "⛔ REFUSE(2) [$G]: no ./scrip to measure"; exit 2; }
[ -f "$BENCH" ] || { echo "⛔ REFUSE(2) [$G]: the churning witness $BENCH is absent -- this gate needs a program that collects hundreds of times at 1 MB"; exit 2; }
[ -f "$THIN" ]  || { echo "⛔ REFUSE(2) [$G]: the thin witness $THIN is absent"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; examined=0
# receipt <file> <arena> <stress> <program> -- one run, stdout discarded, the receipt kept.  ⛔ Each run gets its OWN
# scratch file: a shared one between two configurations reports false flips (cfo, measured).
receipt() { env SCRIP_HEAP_MB="$2" SCRIP_GC_STRESS="$3" SCRIP_GC_EXERCISE=1 timeout 120s "$ROOT/scrip" "$4" 2>"$1" >/dev/null; grep -m1 '^\[GC-EXERCISE\]' "$1"; }
fld() { printf '%s\n' "$1" | grep -oE "$2=[0-9]+" | head -1 | cut -d= -f2; }
# ARM 1 -- SILENT WHEN UNASKED.  A receipt that prints unconditionally changes the stderr of every board and witness in
# the tree, and a gate somewhere is grading stderr byte for byte.  The knob is the whole contract.
examined=$((examined + 1))
env SCRIP_HEAP_MB=1 timeout 120s "$ROOT/scrip" "$THIN" 2>"$T/quiet.txt" >/dev/null
if [ "$(grep -c 'GC-EXERCISE' "$T/quiet.txt")" = 0 ]; then echo "  arm 1 PASS: no receipt without SCRIP_GC_EXERCISE -- existing boards and stderr-grading gates are untouched"
else echo "  arm 1 FAIL: the receipt printed with the knob unset, so it is spam and it changes what every other instrument reads"; RC=1; fi
# ARM 2 -- THE ARENA FIELD TRACKS THE WINDOW.  This is the arm the pre-cure code fails: it read (int)GC_HEAP_MB, a
# compile-time 512, so it reported 512 at every window.  Three arenas, because two could be a coincidence of defaults.
examined=$((examined + 1))
r1=$(receipt "$T/a2_1.txt" 1 0 "$THIN"); r4=$(receipt "$T/a2_4.txt" 4 0 "$THIN"); r5=$(receipt "$T/a2_512.txt" 512 0 "$THIN")
g1=$(fld "$r1" arena_mb); g4=$(fld "$r4" arena_mb); g5=$(fld "$r5" arena_mb)
w1=1; [ "${FAIL_ONCE:-0}" = 1 ] && w1=512
if [ "${g1:-x}" = "$w1" ] && [ "${g4:-x}" = 4 ] && [ "${g5:-x}" = 512 ]; then echo "  arm 2 PASS: arena_mb tracks the resolved window (1 -> ${g1} · 4 -> ${g4} · 512 -> ${g5}); a compile-time constant reports 512 three times and cannot pass this"
else echo "  arm 2 FAIL: arena_mb does not track the window -- read [${g1:-none}] [${g4:-none}] [${g5:-none}] wanted [$w1] [4] [512]. The runtime is naming a configuration it did not run under."; RC=1; fi
# ARM 3 -- COLLECTIONS SEPARATES EXERCISE FROM CONFIGURATION, which is the whole point of the receipt.  The churning
# witness collects hundreds of times at 1 MB and a handful at the shipped arena; the THIN one collects once and then
# ⭐ ZERO AT 4 MB, the trap this gate exists to make visible -- "small" is not a synonym for "exercised".
examined=$((examined + 1))
cb=$(receipt "$T/a3_b1.txt" 1 0 "$BENCH"); cs=$(receipt "$T/a3_b512.txt" 512 0 "$BENCH")
nb=$(fld "$cb" collections); ns=$(fld "$cs" collections); nt4=$(fld "$r4" collections); nt1=$(fld "$r1" collections)
if [ -n "${nb:-}" ] && [ -n "${ns:-}" ] && [ "${nb:-0}" -gt 50 ] && [ "${ns:-0}" -lt "${nb:-0}" ] && [ "${nt1:-0}" -gt 0 ] && [ "${nt4:-1}" -eq 0 ]; then
  echo "  arm 3 PASS: collections separates exercise from configuration -- churning witness ${nb} at 1 MB against ${ns} at 512; thin witness ${nt1} at 1 MB and ${nt4} at 4 MB, so a 4 MB 'small arena' run exercises the collector ZERO times and only this field can say so"
else echo "  arm 3 FAIL: collections cannot distinguish an exercised run from an unexercised one -- churning [${nb:-none}]@1MB [${ns:-none}]@512MB, thin [${nt1:-none}]@1MB [${nt4:-none}]@4MB. A constant or absent field reads exactly like this."; RC=1; fi
# ARM 4 -- THE RESERVE DOES NOT FOLLOW THE WINDOW.  ⭐ THIS TURNS A CORRECTION INTO A MECHANISM: the cap used to shrink
# with the window, so small-arena testing needed SCRIP_HEAP_MAX_MB raised or a live set was refused.  SCRIP e6aab28c7
# cut that tie and SCRIP_HEAP_MB=1 ALONE became the instrument -- a fact every seat now relies on and nothing measured.
# If the tie ever comes back, small-arena runs start dying of [ZHP] heap-exhausted aborts that read as collector bugs.
examined=$((examined + 1))
v1=$(fld "$r1" reserve_mb); v5=$(fld "$r5" reserve_mb)
if [ -n "${v1:-}" ] && [ "${v1:-x}" = "${v5:-y}" ]; then echo "  arm 4 PASS: reserve_mb is ${v1} at BOTH a 1 MB and a 512 MB window -- the cap does not follow the window, so SCRIP_HEAP_MB=1 alone is the instrument and no SCRIP_HEAP_MAX_MB is owed"
else echo "  arm 4 FAIL: reserve_mb follows the window (1 MB -> ${v1:-none}, 512 MB -> ${v5:-none}). A tiny arena now refuses live sets and every [ZHP] abort it causes will be misread as a collector defect."; RC=1; fi
# ARM 5 -- THE STRESS AXIS MOVES IT TOO.  A board declares TWO GC axes (arena and stress) and the receipt must read
# both, or it answers half the question a lane's config= column now asks.
examined=$((examined + 1))
s0=$(fld "$(receipt "$T/a5_s0.txt" 1 0 "$THIN")" collections); s3=$(fld "$(receipt "$T/a5_s3.txt" 1 3 "$THIN")" collections)
e3=$(grep -m1 '^\[GC-EXERCISE\]' "$T/a5_s3.txt" | grep -oE 'stress=[0-9]+' | cut -d= -f2)
if [ "${s3:-0}" -gt "${s0:-0}" ] && [ "${e3:-x}" = 3 ]; then echo "  arm 5 PASS: the receipt reads the stress axis too -- ${s0} collections at stress=0 against ${s3} at stress=3 on one arena, and the line names the stress it ran under (${e3})"
else echo "  arm 5 FAIL: the stress axis is invisible to the receipt -- stress=0 gave [${s0:-none}], stress=3 gave [${s3:-none}], line reported stress=[${e3:-none}]"; RC=1; fi
if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: a board can now say whether the collector RAN, not merely which arena it asked for -- the receipt tracks the resolved window, the resolved reserve, the stress axis and the collection count, and a 4 MB 'small arena' is caught reporting zero exercise (examined $examined arms)"
else echo "GATE FAIL(1) [$G]: the exercise of the collector is still unmeasurable from a board line (examined $examined arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
