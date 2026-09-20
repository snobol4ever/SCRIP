#!/usr/bin/env bash
# test_gate_gc_a_safe_point_stores_into_a_mapped_slot.sh -- ARCH-GC SECTION 3's "INTO A MAPPED SLOT" CLAUSE, MADE
# CHECKABLE (cto 2026-09-20; row gc-the-planner-gives-a-call-result-live-across-a-safe-point-a-mapped-slot-...;
# CEO-996 the finding, CEO-1006 the re-lane, CEO-997 the grading rule; law RULES.md FACT RULE THE COLLECTOR
# GUESSES NOTHING, CEO-812).
#
# WHAT THIS GATE IS FOR, AND WHY IT IS RED THE DAY IT LANDS.  ARCH-GC section 3 has read, since the page was
# written, that a safe point is "placed AFTER the call's result has been stored to its mapped slot".  Section 3b is
# the finding that NOTHING HAS EVER ENFORCED THE CLAUSE -- and a design sentence stating a property the emitter is
# never checked against is indistinguishable, to every reader and every instrument, from a property the emitter
# has.  This gate is the thing that makes it distinguishable.  The property is FALSE on this tree, so arm (f) is
# RED and the gate is RED; that is the measurement, not a defect in the gate.  It goes green when the CURE lands.
#
# ⛔ WHY NOT AN rc-SHAPED ARM (CEO-997 on this exact road, CEO-556).  The failure this class produces exits 0 with
# a plausible wrong answer: the collector sweeps a live block, the arena re-issues it IN PLACE, the defer road
# resolves a garbage name and the program prints something.  No rc can tell a cure from a silencing, so the
# property is graded by ORACLE DIFF against a ref cut from BOTH SNOBOL4 oracles, and the POPULATION is graded off
# the emitted text by scripts/util_gc_unmapped_store_census.py.
#
# ⛔ WHAT THE CENSUS CANNOT SEE, NAMED SO NOBODY READS ITS ZEROES AS COVERAGE.  Class 3 of section 3c -- a raw heap
# pointer returned in a REGISTER PAIR (cfo CFO-134) -- presents no store target, because a frame map describes
# memory.  And 294 sites over the shared witness set are UNDECIDABLE rather than green: the wired regime enters
# those boxes through an indirect jump, so no path from the frame's own prologue reaches them.  Arm (d) exists
# because "0 members" for Prolog and "0 members" for Raku are different statements and only one of them is clean.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
CENSUS="$ROOT/scripts/util_gc_unmapped_store_census.py"
WIT="$ROOT/scripts/gc_witnesses/hb_mkexpr_unmapped_spine_store.sno"
REF="$ROOT/scripts/gc_witnesses/hb_mkexpr_unmapped_spine_store.ref"
SCRIP="$ROOT/scrip"
BASE_MEMBERS=162
BASE_UNDEC=294
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [gc_a_safe_point_stores_into_a_mapped_slot]: $1"; exit 2; }

[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -f "$CENSUS" ] || refuse "the census $CENSUS is missing -- nothing to grade"
[ -f "$WIT" ] || refuse "the row's witness $WIT is missing"
[ -f "$REF" ] || refuse "the witness has no oracle-cut ref at $REF -- a ref is cut from the oracle, never from our output"
echo "ARENA SCRIP_HEAP_MB=${SCRIP_HEAP_MB:-1} (the tiny arena is the default of GC testing -- CEO-931/934)"

# (a) the instrument grades itself before it grades the tree
st="$(timeout 120s python3 "$CENSUS" --selftest 2>&1)"; src=$?
n="$(printf '%s\n' "$st" | sed -n 's/^SELFTEST \([0-9]*\)\/\([0-9]*\) arms green/\1 \2/p')"
set -- ${n:-0 0}
if [ "$src" = 0 ] && [ "${1:-0}" -ge 12 ] && [ "${1:-0}" = "${2:-x}" ]; then
  ck ok "(a) the census passes its own selftest, ${1} of ${2} arms -- the arithmetic and the refusals are held against hand-built inputs"
else
  ck no "(a) the census selftest did not hold (rc=$src): $(printf '%s\n' "$st" | tail -3 | tr '\n' ' ')"
fi

# (b) the class is REAL and the instrument SEES it -- proven red on the row's witness before any cure
w="$(timeout 300s python3 "$CENSUS" "$WIT" 2>&1)"; wrc=$?
if [ "$wrc" = 1 ] && printf '%s\n' "$w" | grep -q 'MEMBER BELOW-REGION .* graph=main .* off=-24 '; then
  ck ok "(b) the census NAMES the row's witness: $(printf '%s\n' "$w" | grep -m1 'off=-24 ' | sed 's/^CENSUS unmapped-store //' | cut -c1-110)"
else
  ck no "(b) the census did not name the witness at off=-24 on graph=main (rc=$wrc) -- SCRIP_GC_MAPS=1 prints [GC-WALK-SPINE] graph=main off=-24 at this very site, so an instrument that misses it is measuring something else"
fi

# (c) PLANTED: the mangling fact of ARCH-GC 6.2d, withheld, must REFUSE rather than read zero
plant="$(timeout 300s python3 - "$CENSUS" "$SCRIP" "$WIT" <<'PY' 2>&1
import sys, os, subprocess, tempfile, importlib.util
census_path, scrip, wit = sys.argv[1], sys.argv[2], sys.argv[3]
spec = importlib.util.spec_from_file_location("uc", census_path)
uc = importlib.util.module_from_spec(spec); spec.loader.exec_module(uc)
with tempfile.TemporaryDirectory() as wd:
    asm, rep, err = uc.emit_and_read(scrip, wit, wd)
    if err: print("PLANT-REFUSED-TO-SET-UP", err); sys.exit(3)
    doctored = "\n".join(l for l in rep.split("\n") if "graph=main " not in l)
    m, u, ex, refusal = uc.census_asm(asm, doctored, "planted")
    print("PLANT-RESULT", "REFUSED" if refusal else f"ACCEPTED members={len(m or [])}", refusal or "")
PY
)"
if printf '%s\n' "$plant" | grep -q '^PLANT-RESULT REFUSED'; then
  ck ok "(c) PLANTED -- withholding main's [GC-MAP] line makes the census REFUSE, not read zero members for main: $(printf '%s\n' "$plant" | sed 's/^PLANT-RESULT REFUSED //' | cut -c1-96)"
else
  ck no "(c) the census ACCEPTED a report whose map label matches no graph -- every site under that label would read green by accident, which is the mangling fact of ARCH-GC 6.2d going unheld: $plant"
fi

# (d) an undecidable site is never folded into a green count
pop="$(timeout 900s python3 "$CENSUS" "$ROOT"/scripts/gc_witnesses/*.icn "$ROOT"/scripts/gc_witnesses/*.sno "$ROOT"/scripts/gc_witnesses/*.pl "$ROOT"/scripts/gc_witnesses/*.raku 2>&1)"; prc=$?
[ "$prc" = 2 ] && refuse "the census refused over the shared witness set: $(printf '%s\n' "$pop" | grep -m1 REFUSED)"
bad=0
while read -r lang mem und; do
  [ -z "${lang:-}" ] && continue
  if [ "$mem" = 0 ] && [ "$und" != 0 ]; then
    printf '%s\n' "$pop" | grep -q "LANG $lang .*ZERO MEMBERS HERE IS NOT A CLEAN READING" || bad=$((bad+1))
  fi
done <<EOF
$(printf '%s\n' "$pop" | sed -n 's/^CENSUS unmapped-store LANG \([a-z]*\) witnesses=[0-9]* members=\([0-9]*\) undecidable=\([0-9]*\).*/\1 \2 \3/p')
EOF
if [ "$bad" = 0 ]; then
  ck ok "(d) every language reading zero members WITH undecidable sites says so in its own line -- $(printf '%s\n' "$pop" | grep -c '^CENSUS unmapped-store LANG ') language rows printed"
else
  ck no "(d) $bad language row(s) printed zero members over an unmeasured population without saying so -- that is a zero nobody could have failed"
fi

# (e) THE RATCHET: the class cannot grow back in silence, and it cannot shrink without the baseline being moved
sum="$(printf '%s\n' "$pop" | grep -m1 '^CENSUS unmapped-store witnesses=')"
mem="$(printf '%s\n' "$sum" | sed -n 's/.* members=\([0-9]*\) .*/\1/p')"
und="$(printf '%s\n' "$sum" | sed -n 's/.* undecidable=\([0-9]*\) .*/\1/p')"
if [ "${mem:-x}" = "$BASE_MEMBERS" ] && [ "${und:-x}" = "$BASE_UNDEC" ]; then
  ck ok "(e) THE RATCHET holds at members=$BASE_MEMBERS undecidable=$BASE_UNDEC -- $sum"
elif [ "${mem:-999999}" -gt "$BASE_MEMBERS" ] 2>/dev/null; then
  ck no "(e) THE CLASS GREW: members=$mem against a baseline of $BASE_MEMBERS -- a new safe point stores outside its frame map"
else
  ck no "(e) the population moved to members=${mem:-?} undecidable=${und:-?} against members=$BASE_MEMBERS undecidable=$BASE_UNDEC -- a win nobody recorded is a loose ratchet; move the baseline in the same landing that earns it"
fi

# (f) THE PROPERTY ITSELF, graded by ORACLE DIFF and not by rc (CEO-997)
want="$(cat "$REF")"
diffs=0; band=""
for s in 0 1 2 3 4; do
  got="$(SCRIP_HEAP_MB="${SCRIP_HEAP_MB:-1}" SCRIP_GC_STRESS=$s timeout 120s "$SCRIP" "$WIT" 2>/dev/null)"
  if [ "$got" = "$want" ]; then band="$band ."; else band="$band X"; diffs=$((diffs+1)); fi
done
if [ "$diffs" = 0 ]; then
  ck ok "(f) THE PROPERTY HOLDS: the witness answers its oracle-cut ref at every stress point in the band [0 1 2 3 4] --$band"
else
  ck no "(f) THE PROPERTY IS FALSE, which is this row's whole reason to exist: the witness diverges from its oracle-cut ref at $diffs of 5 stress points --$band (want '$want'). The census names the site: a call result stored to a spine cell BELOW main's frame-map region base, which gc_walk_range walks as untyped words and never visits as a root. THE CURE IS THE PLANNER'S: give every call result that can be live across a safe point a mapped slot, or refuse to place the safe point there."
fi

# (g) an instrument nobody runs is not an instrument
if grep -q 'test_gate_gc_a_safe_point_stores_into_a_mapped_slot.sh' "$ROOT/Makefile" 2>/dev/null; then
  ck ok "(g) this gate is named in the Makefile, as a REPORTED arm -- run and classified by every seat, blocking none of them while arm (f) is red by design (CEO-513 declare-when-you-hold-one-out, CEO-463 a known-red blocking arm makes every seat pay for one open row). It moves to a blocking arm the day (f) goes green"
else
  ck no "(g) this gate is not named in the Makefile -- a gate in no runner is not measuring (RULES.md THE INSTRUMENT LAWS), and a gate landed unwired is the CEO-381 class (CEO-513)"
fi

echo "population: $checks arm(s) graded, $fails FAIL; census over the shared witness set rc=$prc, witness rc=$wrc"
[ "$fails" = 0 ] && { echo "GATE PASS [gc_a_safe_point_stores_into_a_mapped_slot]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [gc_a_safe_point_stores_into_a_mapped_slot]: $fails of $checks arms FAIL"; exit 1
