#!/usr/bin/env bash
# test_gate_gc_every_credited_bare_poll_site_has_a_zero_collection_witness.sh -- THE BARE-POLL RE-SCREEN (cto 2026-09-23;
# row gc-the-116-bare-poll-sites-already-counted-as-done-are-re-screened-by-the-zero-collection-arm; ceo CEO-1137/1144).
#
# WHAT IT GRADES.  The safe-point census credits a site as POLLED when a poll is EMITTED; the bare form roots nothing and
# has three convictions as a wrong answer with the collector never run.  This gate reads the DECLARED table
# scripts/gc_bare_poll_witnesses.tsv (one row per credited bare site, naming a witness or UNWITNESSED) through
# util_gc_safe_point_contract.py --bare-poll: a site is WITNESSED only when its named witness's emission reaches the site
# (the poll helper stamps its template:line into the emitted text) AND that witness's run at stress 0 in a 512 MB arena
# reports collections=0 grew=0 on its own [GC-EXERCISE] line AND prints its oracle-cut .ref, in mode 3 and in mode 4.
# Everything else is NAMED: UNWITNESSED, STALE, NOT-ZERO, UNREAD, DIVERGING, RETIRED, UNDECLARED.
#
# ⛔ THE PIN.  SCRIP_HEAP_MB=512 with SCRIP_HEAP_KB removed, set INSIDE the checker (bare_run_env) and not here: the
# quantity measured is the poll's PRESENCE with the collector never running, and a tiny arena makes every reading
# NOT-ZERO by construction.  This gate is named in test_gate_gc_the_tiny_arena_is_the_default_of_gc_testing.sh's
# declared pinned set for that reason (CEO-931 pin rule: a pin names its reason at the pin and its name in the set).
#
# ARMS.  (a) the checker's selftest holds (the clause's ten plants included);  (b) the reading is not REFUSED and its
# notes-vs-calls count matches (a poll whose note the renderer dropped would read UNREACHED);  (c) DIVERGING is zero --
# a witness that prints the wrong answer with the collector never run is the conviction this row exists for;
# (d) the unwitnessed count is at or below the DECLARED CEILING, which falls with witnesses and RISES ONLY with a landing that ADDS credited bare sites, naming the arrivals (65 over 131 sites at the first landing; 82 over 158 when the 36-to-zero landing of 2026-09-23 added 27 credited bare sites of which 10 the existing witnesses reach:
# the 61 witnesses on disk reach 66; the other 65 are named in the reading and are the row's open work).
# FAIL_ONCE=1 plants a lower ceiling and requires arm (d) to red.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
G="$(basename "$0" .sh)"
CHK="$ROOT/scripts/util_gc_safe_point_contract.py"
TABLE="$ROOT/scripts/gc_bare_poll_witnesses.tsv"
CEILING="${BARE_POLL_UNWITNESSED_CEILING:-79}"
[ "${FAIL_ONCE:-0}" = 1 ] && CEILING=0
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
[ -x "$ROOT/scrip" ] || refuse "no scrip binary -- build before grading"
[ -f "$CHK" ] || refuse "the checker $CHK is missing"
[ -f "$TABLE" ] || refuse "the declared table $TABLE is missing; run: python3 scripts/util_gc_safe_point_contract.py scripts/gc_witnesses/*.{icn,sno,pl,raku,sc} --write-bare-poll-table"
export SCRIP_HEAP_MB=512   # THE PIN (CEO-931 pin rule): this gate measures the poll's PRESENCE with the collector NEVER RUNNING, so collections must read 0; a tiny arena would make every site NOT-ZERO by construction. The checker's bare_run_env sets the same value and removes SCRIP_HEAP_KB; this line is the declaration the tiny-arena gate counts.
unset SCRIP_HEAP_KB
echo "ARENA SCRIP_HEAP_MB=512 PINNED inside the checker for the stress-0 arm (collections must read 0; a tiny arena would make every site NOT-ZERO by construction) -- declared in the tiny-arena gate's pinned set"
echo "POPULATION (declared): $(grep -vc '^#' "$TABLE") credited bare-poll site(s) in $(basename "$TABLE"); ceiling on unwitnessed: $CEILING"
st="$(timeout 300s python3 "$CHK" --selftest 2>&1)"; src=$?
n="$(printf '%s\n' "$st" | sed -n 's/^SELFTEST \([0-9]*\)\/\([0-9]*\) arms green/\1 \2/p')"; set -- ${n:-0 0}
if [ "$src" = 0 ] && [ "${1:-0}" = "${2:-x}" ] && printf '%s\n' "$st" | grep -q "BARE-POLL PLANTED: a poll call WITHOUT its note"; then
  ck ok "(a) the checker's selftest holds, ${1} of ${2} arms, the bare-poll clause's plants among them"
else
  ck no "(a) the checker's selftest did not hold (rc=$src): $(printf '%s\n' "$st" | grep -i 'fail' | head -2 | tr '\n' ' ')"
fi
OUT="$(mktemp -t gc_bare_poll_reading.XXXXXX)"; trap 'rm -f "$OUT"' EXIT
WIT="$(grep -v '^#' "$TABLE" | cut -f3 | grep -v '^UNWITNESSED$' | sort -u | sed "s|^|$ROOT/scripts/gc_witnesses/|")"
timeout 900s python3 "$CHK" $WIT --bare-poll > "$OUT" 2>&1; rrc=$?
summ="$(grep '^CONTRACT BARE-POLL sites=' "$OUT" | head -1)"
notes="$(grep '^CONTRACT BARE-POLL-NOTES ' "$OUT" | head -1)"
if [ "$rrc" = 0 ] && [ -n "$summ" ] && printf '%s\n' "$notes" | grep -q 'mismatched=0'; then
  ck ok "(b) the reading was taken and every witness's poll notes match its poll calls: ${notes#CONTRACT BARE-POLL-NOTES }"
else
  ck no "(b) the reading was REFUSED or unreadable (rc=$rrc): $(grep 'REFUSED' "$OUT" | head -1)"
fi
field() { printf '%s\n' "$summ" | grep -o " $1=[0-9]*" | head -1 | cut -d= -f2; }
div="$(field diverging)"; unw="$(field unwitnessed)"; wit="$(field witnessed)"; sites="$(field sites)"
if [ "${div:-x}" = 0 ]; then
  ck ok "(c) diverging=0: no named witness prints a wrong answer or dies with the collector never run (witnessed=$wit of $sites)"
else
  ck no "(c) diverging=${div:-?}: $(grep '^CONTRACT BARE-POLL-SITE .* DIVERGING' "$OUT" | head -3 | tr '\n' ' ')"
fi
if [ -n "$unw" ] && [ "$unw" -le "$CEILING" ]; then
  ck ok "(d) unwitnessed=$unw is at or below the declared ceiling $CEILING (a ceiling only falls; lower it in this gate when a landing adds witnesses)"
else
  ck no "(d) unwitnessed=${unw:-?} is above the declared ceiling $CEILING -- a site lost its witness (STALE), a witness stopped reading zero (NOT-ZERO), or the census credits a new bare site (UNDECLARED): $(grep -E '^CONTRACT BARE-POLL-SITE .* (STALE|NOT-ZERO|UNREAD|UNDECLARED)' "$OUT" | head -3 | tr '\n' ' ')"
fi
echo "READING: $summ" | cut -c1-260
echo "population: 4 arm(s) over the declared table; the full reading names every site: $OUT (deleted at exit; re-run the checker with --bare-poll to keep it)"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$G]: every credited bare-poll site is either WITNESSED at zero collections in both modes or NAMED ($checks arms)"; exit 0; fi
echo "GATE FAIL(1) [$G]: $fails of $checks arm(s) red"; exit 1
