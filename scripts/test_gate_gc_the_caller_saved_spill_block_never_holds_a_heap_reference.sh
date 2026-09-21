#!/usr/bin/env bash
# test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh -- THE INVARIANT BEHIND DELETING A ROOT
# REGISTRATION, MEASURED INSTEAD OF WRITTEN DOWN (cto 2026-09-20, MODE TENET; asked CTO-153, ruled by the cfo).
#
# THE FINDING.  rtcc_init.c registers the runtime's caller-saved spill block as a GC root range --
# rt_gc_root_range_add_seamsafe(&rtccb[0], &rtccb[32]) -- and gc_heap.c's ONLY walk over the root-range table opens
# `if (g_gc_rrng[i].hi) continue;`, which is exactly how that range and only that range is registered.  The other
# caller, the Prolog trail, uses the topword form with a null hi and IS visited.  g_gc_rrng_ss is incremented at
# registration and read nowhere; g_gc_segs is collected and read nowhere, so no conservative segment scan covers it
# either -- correctly, per CEO-812.  THE REGISTRATION'S ONLY EFFECT IS TO INCREMENT A COUNTER NOBODY READS: to every
# reader rtccb is a registered GC root, and to the collector it is not.
#
# ⛔ THE cfo RULED THAT VISITING IT IS NOT AN OPTION AT ALL, AND THE LAW DECIDES IT RATHER THAN TASTE.  rtccb is
# thirty-two RAW MACHINE WORDS WITH NO TYPE TAGS.  Visiting them means asking of each word "does this look like it
# points into a heap block", WHICH IS A CONSERVATIVE SCAN BY DEFINITION, and CEO-812 froze that out: everything on
# the emitted stack is a DESCR and its type field is the only tag, the heap walks typed visitors, no conservative
# scan and no pinning in any form.  A precise visit is impossible because the words carry nothing to be precise
# about.  So the registration and the seamsafe API are DELETED (the cfo's landing, gc_heap.c is their file) and
# THIS GATE IS WHAT MAKES THAT SAFE.
#
# ⛔⭐ THE MEASUREMENT THAT BUILT THIS GATE WAS WRONG, AND BUILDING THE GATE IS WHAT CAUGHT IT.  I reported to the
# cfo, the ceo and three HQs that the block was LATENT, NOT LIVE -- "47 witnesses at SCRIP_HEAP_MB=1
# SCRIP_GC_STRESS=3, ZERO slots holding a heap pointer".  THAT SWEEP RAN WITHOUT SCRIP_GC_MAPS=1, AND
# gc_walk_print RETURNS EARLY WHEN gc_maps_on() IS FALSE, so neither [GC-RTCCB] nor [GC-WALK-RTCCB] COULD HAVE
# PRINTED UNDER ANY CIRCUMSTANCES.  I grepped for a line the run was incapable of emitting and read the silence as
# a zero -- an instrument reporting success while it is not looking, which is the exact class this row exists to
# remove, produced by me while curing it in three other places the same evening.
# ⭐ THE TRUE READING, reporter enabled, same arena and stress: 806 occurrences over EIGHT SNOBOL4 witnesses --
# hb_dvec_sort_match 604, hb_nv 190, hb_datblk 4, hb_dvec_data_convert 3, hb_blob_span_defer 2, and one each in
# hb_defer_subject, hb_dtp, hb_nested_match_outer_subject.  EVERY ONE IS SLOT 5 = RTCC_SLOT_R8, the register
# rtcc.h declares as the ANCHOR.  hq_prolog's null STANDS: they ran catch_1 with SCRIP_GC_MAPS=1 at stress 3, 5,
# 10 and 25 and read zero, so Prolog genuinely does not park a heap pointer there -- despite owning 4740 of the
# 6705 stores into the block.  Two lanes, two different answers, and only the one that enabled the reporter was
# measuring.
# ⛔ WHAT IS STILL NOT ESTABLISHED, AND THE GATE SAYS SO IN ITS OWN FAILURE TEXT: none of the eight witnesses
# answers its oracle wrongly.  A pointer in an unvisited slot is NECESSARY BUT NOT SUFFICIENT for a lost value,
# the same way a store below the region base was (CTO-101), because the same block may be rooted elsewhere.  The
# sharper hazard is STALENESS rather than sweeping: an unvisited slot cannot be rewritten when a root walk
# FORWARDS a moving block, and the emitted reload after the poll restores the pre-move word.
#
# THE INVARIANT, as the cfo states it and as this grades it: rtccb is CALLER-SAVED SCRATCH and nothing that must
# survive a collection may live there -- r8 the anchor, r9 the GVA, r10 and r11 scratch, per the declared
# convention in rtx/rtcc.h.  A value that must survive a poll goes in a frame slot covered by that frame's static
# map, or is handed to the shield as a TAGGED DESCR cell in arr[], which the poll's own refusal text says in words
# (CEO-972).
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
# ⛔ THE FRESHNESS GUARD, AND IT IS HERE BECAUSE THIS GATE'S OWN CLASS OF DEFECT REACHED THE GATES BUILT TO CHASE
# IT (the coo, 2026-09-21).  They built at 15:01:58, the cto landed a cure at 15:01:39, they started a 381-arm
# set at 15:03:52 without rebuilding, and 260 arms refused one at a time for that one reason -- while the DARK
# run finished in HALF the wall clock of the honest one, so a seat optimising for a quick gate was being
# rewarded for measuring nothing.  A gate that grades ./scrip with no guard will one day grade a binary older
# than the cure it is testing and call it green.  One line, and the staleness rule itself lives in exactly one
# place (gate_require_fresh in lib_gate.sh) -- this is its calling convention, never a second copy of it.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
SCRIP="$ROOT/scrip"
WD="$ROOT/scripts/gc_witnesses"
STRESS="${GC_RTCCB_STRESS:-3}"
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [gc_the_caller_saved_spill_block_never_holds_a_heap_reference]: $1"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -d "$WD" ] || refuse "no $WD -- nothing to grade"
echo "ARENA SCRIP_HEAP_MB=${SCRIP_HEAP_MB:-1} (the tiny arena is the default of GC testing -- CEO-931/934) · stress $STRESS"

# (a) THE REPORTER EXISTS AND IS REACHED.  ⛔ AN ARM THAT GRADES A COUNTER NOTHING INCREMENTS PASSES BY NEVER
# LOOKING, which is the failure THE INSTRUMENT LAWS exist to catch and the reason this arm comes first.
src="$ROOT/src/runtime/rt/gc_heap.c"
[ -f "$src" ] || refuse "no $src -- the reporter this arm grades cannot be located"
if grep -q "GC-WALK-RTCCB" "$src" && grep -q "g_gc_rtccb_heap" "$src"; then
  ck ok "(a) the runtime's own rtccb reporter is present in gc_heap.c -- this gate grades a counter that something increments"
else
  ck no "(a) gc_heap.c carries no GC-WALK-RTCCB reporter or no g_gc_rtccb_heap counter, so every arm below would read zero by never looking. If the reporter was deliberately removed, this gate must be retired in the same landing and the invariant re-homed, NOT left grading nothing"
fi

# (b) THE INVARIANT ITSELF, over every witness that collects.  A slot holding a pointer INTO A HEAP BLOCK at a
# collection is the whole defect: the block is unreachable from any root walk, so it is swept under a live value.
witn=0; coll=0; hits=0; named=""; slots=""
for f in "$WD"/*.sno "$WD"/*.icn "$WD"/*.pl "$WD"/*.raku "$WD"/*.sc; do
  [ -f "$f" ] || continue
  witn=$((witn+1))
  err="$(SCRIP_HEAP_MB="${SCRIP_HEAP_MB:-1}" SCRIP_GC_STRESS="$STRESS" SCRIP_GC_MAPS=1 timeout 120s "$SCRIP" "$f" 2>&1 >/dev/null)"
  n="$(printf '%s\n' "$err" | grep -c '^\[GC-WALK\]')"
  [ "${n:-0}" -gt 0 ] && coll=$((coll+1))
  h="$(printf '%s\n' "$err" | grep -c 'GC-WALK-RTCCB')"
  if [ "${h:-0}" -gt 0 ]; then
    hits=$((hits+h)); named="$named $(basename "$f"):$h"
    slots="$slots $(printf '%s\n' "$err" | sed -n 's/.*GC-WALK-RTCCB. slot=\([0-9]*\).*/\1/p' | sort -un | tr '\n' ',')"
  fi
done
[ "$witn" -gt 0 ] || refuse "no witness matched under $WD -- a census over an empty population reads zero by never looking"
[ "$coll" -gt 0 ] || refuse "ZERO of $witn witness(es) collected at arena ${SCRIP_HEAP_MB:-1} MB stress $STRESS -- this arm would be a statement about a population that never ran the collector, not about the spill block"
uslots="$(printf '%s' "$slots" | tr ', ' '\n\n' | grep -E '^[0-9]+$' | sort -un | tr '\n' ' ')"
if [ "$hits" = 0 ]; then
  ck ok "(b) THE INVARIANT HOLDS: over $witn witness(es), $coll of which actually collected, NO rtccb slot held a pointer into a heap block at any collection"
else
  ck no "(b) ⛔ A CALLER-SAVED SPILL SLOT HELD A POINTER INTO A HEAP BLOCK AT A COLLECTION, $hits time(s), slot(s) [$uslots], in:$named. ⛔⭐ WHAT THIS ESTABLISHES AND WHAT IT DOES NOT, stated because the difference is this row's own subject: it establishes that the invariant the cfo stated -- rtccb is CALLER-SAVED SCRATCH and nothing that must survive a collection lives there -- IS NOT TRUE OF THIS TREE. It does NOT establish a lost value: a pointer in an unvisited slot is NECESSARY BUT NOT SUFFICIENT, exactly as a store below the region base was (CTO-101), because the same block may be rooted elsewhere -- the subject is, through the shield and through the visit of the subject pointer itself. ⭐ THE SHARPER HAZARD IS NOT SWEEPING BUT STALENESS: this block is never visited, so a root walk that FORWARDS a moving block cannot rewrite rtccb's copy, and the emitted reload after the poll restores the pre-move word. THE CURE IS NOT TO VISIT rtccb -- 32 untagged words cannot be visited precisely and a sniff is the conservative scan CEO-812 forbids: put the value in a frame slot covered by that frame's static map, or hand it to the shield as a TAGGED DESCR cell in arr[] (CEO-972). SCRIP_GC_MAPS=1 prints the slot index and the word"
fi

# (c) THE POPULATION IS DECIDABLE AND SAYS SO.  ⛔ hq_prolog MEASURED THAT AT THE TINY ARENA WITH NO STRESS PLANT
# ONLY 4 OF 23 PROLOG KERNELS COLLECT AT ALL, so an arm that does not ask whether the collector ran prints the
# arena in its verdict line while measuring nothing about the collector.
if [ "$coll" = "$witn" ]; then
  ck ok "(c) every one of the $witn witness(es) collected at this band point, so arm (b)'s zero is a zero over the whole population"
elif [ "$coll" -ge 1 ]; then
  ck ok "(c) $coll of $witn witness(es) collected; arm (b)'s zero is a zero over THOSE $coll and is NAMED as such rather than read as coverage of all $witn"
else
  ck no "(c) nothing collected -- unreachable, arm (b) refuses first"
fi

# (d) THE REGISTRATION IS GONE, OR IT IS NAMED AS STILL STANDING.  ⛔ THIS ARM IS THE ONE THAT KEEPS THE DELETION
# HONEST: while the seamsafe registration exists, every reader of rtcc_init.c believes the block is a GC root, and
# a reader who believes that will park a descriptor in r10 across a poll on purpose.
reg="$ROOT/src/runtime/rtx/rtcc_init.c"
if [ ! -f "$reg" ]; then
  ck ok "(d) rtcc_init.c is gone entirely -- nothing registers the block"
elif grep -q "rt_gc_root_range_add_seamsafe" "$reg"; then
  ck no "(d) ⛔ rtcc_init.c STILL REGISTERS the block as a root range, and gc_heap.c's only walk over that table skips exactly that shape. Until the registration is deleted the file says ROOT and the collector says nothing -- the cfo ruled DELETE IT (CTO-153), because visiting 32 untagged words is the conservative scan CEO-812 froze out. This arm is expected RED until that landing arrives and is the reason this gate is REPORTED and not blocking"
else
  ck ok "(d) the seamsafe registration is deleted -- rtcc_init.c no longer tells its readers the block is a GC root"
fi

# (e) an instrument nobody runs is not an instrument
if grep -q 'test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh' "$ROOT/Makefile" 2>/dev/null; then
  ck ok "(e) this gate is named in the Makefile"
else
  ck no "(e) this gate is not named in the Makefile -- a gate in no runner is not measuring (RULES.md THE INSTRUMENT LAWS)"
fi

echo "population: $checks arm(s) graded, $fails FAIL; $witn witness(es), $coll collected, $hits rtccb heap reference(s)"
[ "$fails" = 0 ] && { echo "GATE PASS [gc_the_caller_saved_spill_block_never_holds_a_heap_reference]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [gc_the_caller_saved_spill_block_never_holds_a_heap_reference]: $fails of $checks arms FAIL"; exit 1
