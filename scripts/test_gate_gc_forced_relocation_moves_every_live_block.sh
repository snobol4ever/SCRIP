#!/usr/bin/env bash
# test_gate_gc_forced_relocation_moves_every_live_block.sh -- THE DELIVERABLE OF THE cfo's ROW
# `gc-every-live-block-relocates-on-every-collection-so-a-stale-pointer-cannot-accidentally-still-work`
# (ceo-minted 2026-09-21, MODE TENET).
#
# ⛔ THE NUMBER THIS ROW EXISTS FOR, MEASURED BEFORE ANY CURE WAS WRITTEN AND NOT PROJECTED.  Over
# bench_icnstr_concat_table.icn at the mandated 1 MB arena -- 1613 collections, 1058128 live-block forwardings --
# 896273 of those forwardings (84.7%) LEFT THE BLOCK EXACTLY WHERE IT WAS.  Steady state was sharper than the
# total: collection 1 moved 655 of 656 blocks, and every collection after it moved 100 and left 556 sitting.
# THAT is why a lost root so often prints a plausible answer: the pointer is stale in principle and, five times
# out of six, still valid in fact.  A compaction is not a relocation.
#
# WHAT THE KNOB DOES.  SCRIP_GC_RELOC=1 makes the forwarding loop refuse to hand any live block its own address:
# when the packed destination lands on the block's current address, the destination is advanced by one minimum
# legal block (2*sizeof(rt_hblk_t) = 32 bytes -- the heap verifier's own floor, which is what caught the first
# version of this cure using 16) and the gap becomes a walkable HB_FILL.  Every live block therefore relocates on
# every collection, so a surviving reference to an old address is wrong IMMEDIATELY rather than eventually.
# ⭐ IT COMPOSES WITH THE POISON-TRAP ROW (cfo CFO-124, SCRIP_GC_TRAP, default ON): relocation guarantees the old
# address is vacated, PROT_NONE guarantees that reading it faults.  Neither is sufficient alone -- the trap's own
# census found 15 of 17 silently-wrong entries stayed dark precisely because their blocks never moved.
#
# ⛔ WHY ARM (b) IS NOT DECORATION.  A gate that only checks `unmoved=0 under the knob` passes on a population
# where nothing would have stayed put anyway, and then a cure that was deleted still reads green.  So the SAME
# BINARY is run with the knob OFF and the gate REFUSES -- not reds, REFUSES -- unless the off arm shows blocks
# staying put.  A green here is a COMPARISON THAT HAPPENED.  That is the cfo's own attribution method (one binary,
# one env switch, never a copied compiler) and the rule this seat took from arena_mb: a configuration is not an
# exercise, so arm (a) counts collections and a zero-collection arm is a refusal too.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
SCRIP="$ROOT/scrip"
LIBDIR="$ROOT/out"
WITDIR="$ROOT/scripts/gc_witnesses"
WITS="${GC_RELOC_WITS:-hb_big_in_aggregate.icn hb_dvec_list_slice.icn hb_scan_nested.icn hb_cv_spine_plain_redo.icn hb_datblk.sno hb_blob_span_defer.sno hb_deferexpr_a_dupl.sno}"
PTS="${GC_RELOC_BAND:-0 3 16}"
ARENA="${SCRIP_HEAP_MB:-1}"
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [gc_forced_relocation_moves_every_live_block]: $1"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -f "$LIBDIR/libscrip_rt.so" ] || refuse "no out/libscrip_rt.so -- the m4 arm would have measured nothing"
. scripts/util_require_fresh.sh 2>/dev/null || true
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
echo "ARENA SCRIP_HEAP_MB=$ARENA (the tiny arena is the default of GC testing -- CEO-931/934)"
echo "    band: $PTS · modes m3 m4 · knob SCRIP_GC_RELOC off/on ON ONE BINARY · census SCRIP_GC_DISPLACE=1"
# unmoved_of <errfile> -> "collections unmoved live"; a census line per collection, aggregated here rather than
# in the runtime, because RULES.md line 231 covers function-scope statics and this seat holds no grant.
unmoved_of() { awk '/GC-DISPLACE/{for(i=1;i<=NF;i++){split($i,a,"=");v[a[1]]=a[2]}n++;U+=v["unmoved"];L+=v["live"]}END{printf "%d %d %d",n+0,U+0,L+0}' "$1"; }
run_one() {
  local bin="$1" src="$2" st="$3" reloc="$4" out="$5" err="$6"
  if [ "$bin" = m3 ]; then
    SCRIP_HEAP_MB="$ARENA" SCRIP_GC_DISPLACE=1 SCRIP_GC_RELOC="$reloc" SCRIP_GC_STRESS="$st" timeout 120s "$SCRIP" "$src" > "$out" 2> "$err"
  else
    SCRIP_HEAP_MB="$ARENA" SCRIP_GC_DISPLACE=1 SCRIP_GC_RELOC="$reloc" SCRIP_GC_STRESS="$st" timeout 120s "$bin" > "$out" 2> "$err"
  fi
}
tot_off_un=0; tot_off_fw=0; tot_on_un=0; tot_on_fw=0; arms=0; coll_arms=0; zero_coll=""; refused_lines=0; idle_wits=""
bad_unmoved=""; bad_sem=""; bad_ref=""
for w in $WITS; do
  src="$WITDIR/$w"; ref="${src%.*}.ref"
  [ -f "$src" ] || refuse "declared witness $w is missing from $WITDIR -- the population is a declaration, so a gap in it is a refusal not a pass"
  [ -f "$ref" ] || refuse "witness $w has no committed ref -- a ref is cut from the oracle, never from our own output"
  wit_coll=0
  ext="${w##*.}"
  cp "$src" "$T/w.$ext"
  ( cd "$T" && timeout 90s "$ROOT/scrip" --compile -o "w_$w.s" "w.$ext" < /dev/null > /dev/null 2>&1 \
      && gcc -m64 -no-pie -rdynamic "w_$w.s" -Wl,-rpath,"$LIBDIR" -L"$LIBDIR" -lscrip_rt -lm -lpthread -o "w_$w.x4" 2> "w_$w.ld.log" ) \
      || refuse "mode-4 compile or link failed for $w, so its m4 arms measured nothing ($(head -c 160 "$T/w_$w.ld.log" 2>/dev/null))"
  for mode in m3 m4; do
    bin=m3; [ "$mode" = m4 ] && bin="$T/w_$w.x4"
    for s in $PTS; do
      arms=$((arms+1)); tag="$w.$mode.$s"
      run_one "$bin" "$src" "$s" 0 "$T/$tag.off.out" "$T/$tag.off.err"
      run_one "$bin" "$src" "$s" 1 "$T/$tag.on.out"  "$T/$tag.on.err"
      read c0 u0 l0 <<< "$(unmoved_of "$T/$tag.off.err")"
      read c1 u1 l1 <<< "$(unmoved_of "$T/$tag.on.err")"
      tot_off_un=$((tot_off_un+u0)); tot_off_fw=$((tot_off_fw+l0))
      tot_on_un=$((tot_on_un+u1));   tot_on_fw=$((tot_on_fw+l1))
      if [ "$c1" -gt 0 ]; then coll_arms=$((coll_arms+1)); wit_coll=$((wit_coll+1)); else zero_coll="$zero_coll $tag"; fi
      r=$(grep -c 'GC-RELOC' "$T/$tag.on.err" 2>/dev/null || true); refused_lines=$((refused_lines+r))
      [ "$u1" = 0 ] || bad_unmoved="$bad_unmoved $tag(unmoved=$u1/$l1)"
      cmp -s "$T/$tag.off.out" "$T/$tag.on.out" || bad_sem="$bad_sem $tag"
      cmp -s "$T/$tag.on.out" "$ref" || bad_ref="$bad_ref $tag"
    done
  done
  [ "$wit_coll" -gt 0 ] || idle_wits="$idle_wits $w"
done
[ "${FAIL_ONCE:-0}" = 1 ] && { bad_unmoved="$bad_unmoved PLANTED"; bad_sem="$bad_sem PLANTED"; bad_ref="$bad_ref PLANTED"; refused_lines=$((refused_lines+1)); }
echo "  population: $arms arms = $(printf '%s\n' $WITS | wc -l) witnesses x 2 modes x $(printf '%s\n' $PTS | wc -l) stress points, each run TWICE on one binary"
[ -n "$idle_wits" ] && refuse "these witnesses never collected at ANY band point, so they are decorative in this gate and their arms would pass by producing nothing (a configuration is not an exercise):$idle_wits"
echo "  exercised: $coll_arms of $arms arms actually collected; $((arms-coll_arms)) idle and EXCLUDED from the verdict rather than counted as green:$zero_coll"
if [ "$tot_off_fw" -gt 0 ] && [ "$tot_off_un" -gt 0 ]; then
  ck ok "(b) CONTROL -- knob OFF on this same binary leaves $tot_off_un of $tot_off_fw live-block forwardings ($(awk -v u=$tot_off_un -v l=$tot_off_fw 'BEGIN{printf "%.1f", 100*u/l}')%) sitting on their own address, so the arm below is a comparison that happened"
else
  refuse "knob OFF left NOTHING sitting on its own address over $tot_off_fw forwardings -- on this population the cure is unobservable and a green from arm (c) would mean nothing"
fi
if [ -z "$bad_unmoved" ]; then
  ck ok "(c) VERDICT -- knob ON: 0 of $tot_on_fw live-block forwardings kept their address, across all $arms arms"
else
  ck no "(c) a live block kept its own address under SCRIP_GC_RELOC=1:$bad_unmoved"
fi
if [ "$refused_lines" = 0 ]; then
  ck ok "(d) the plant never silently declined -- no [GC-RELOC] REFUSED line in any arm"
else
  ck no "(d) the plant REFUSED to displace on $refused_lines arm(s): the arena had no room for a 32-byte gap, so those collections are NOT forced-relocation measurements and must not be read as green"
fi
if [ -z "$bad_sem" ]; then
  ck ok "(e) SEMANTIC -- stdout byte-identical with the knob off and on across all $arms arms; relocation moves addresses, never answers"
else
  ck no "(e) forced relocation CHANGED the answer on:$bad_sem"
fi
if [ -z "$bad_ref" ]; then
  ck ok "(f) ORACLE -- every arm under the knob still matches its committed ref"
else
  ck no "(f) the knob-on answer diverged from the committed ref on:$bad_ref"
fi
echo "gc_forced_relocation_moves_every_live_block: checks=$checks fails=$fails"
[ "$fails" = 0 ] || exit 1
exit 0
