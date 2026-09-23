#!/usr/bin/env bash
# test_gate_gc_the_visited_set_key_spaces_do_not_collide.sh -- THE COLLECTOR'S VISITED SET HOLDS THREE KINDS OF KEY IN ONE
# TABLE, AND A KEY OF ONE KIND MUST NEVER ANSWER A QUESTION OF ANOTHER (cto 2026-09-23, CTO-153; law RULES.md FACT RULE THE
# COLLECTOR GUESSES NOTHING, CEO-812).
#
# MEASURED 2026-09-23 on procedure_every_scan_replace_9 (the Icon master), the ONE entry the holder-(A) landing bda7691cc
# turned non-green (CRASH at SCRIP_GC_STRESS=1/3/5 under SCRIP_GC_RELOC=1 in both modes, [ZGC-STALE] "THE FAULT HAPPENED
# INSIDE A COLLECTION"): the crash was gc_visit_tbblk reading a table whose buckets pointer had not been repaired for
# three collections while the table itself moved every time.  The gdb probes (slot2..slot8.py: a watchpoint on the frame
# slot, then on the table's buckets word, then every gc_hins call for the table's address with its caller line and return
# value) read the mechanism: a DT_N slen=1 cell reference whose ptr was the table's first word inserted the TABLE'S OWN
# ADDRESS into the visited set from gc_visit_one's name-cell case (gc_heap.c:823, key = the cell address), every later
# DT_T visit of the table then read gc_hins(t)=0 "already visited" and skipped gc_visit_tbblk, the buckets array was never
# marked, was reclaimed, and the collection after next walked an integer cell as a bucket.  Ordinary compaction had left
# the table and its buckets in place, so the assignment seam's removal only changed WHEN the collision started to matter.
#
# THE CURE: three key spaces by construction -- bit 0 = a registered slot (gc_slot_reg, unchanged), bit 1 = a name-
# referenced cell (the DT_N slen=1 case), an even key = an aggregate whose contents were visited.  The plant
# SCRIP_GC_PLANT_KEY_COLLISION=1 restores the shared key so the crash can be seen to return.
#
# ARMS: (a) THE PROPERTY: the witness (the master entry, ref cut from iconx) answers its ref at SCRIP_GC_STRESS=1,3,5 under
# SCRIP_GC_RELOC=1 at the compiled default arena in mode 3 and mode 4; (b) the plant reproduces the loss (rc!=0 or DIFF) at
# stress 1 -- an instrument never seen to fire is not known to look; (c) the source carries the distinct key.
# FAIL_ONCE=1 runs arm (a) under the plant and requires it to red.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
WD="$ROOT/scripts/gc_witnesses"; W="$WD/hb_table_dump_every_key_ref"; [ -s "$W.icn" ] && [ -s "$W.ref" ] || { echo "⛔ GATE REFUSE(2) [$G]: witness or ref missing under gc_witnesses"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; plant=""; [ "${FAIL_ONCE:-0}" = 1 ] && plant="SCRIP_GC_PLANT_KEY_COLLISION=1"
band=""; bad=0
for st in 1 3 5; do ( cd "$T" && env -u SCRIP_HEAP_MB $plant SCRIP_GC_STRESS=$st SCRIP_GC_RELOC=1 timeout 60s "$SCRIP" --run "$W.icn" </dev/null > m3_$st.out 2> m3_$st.err ); r=$?; if [ $r = 0 ] && cmp -s "$T/m3_$st.out" "$W.ref"; then band="$band m3@$st:ok"; else band="$band m3@$st:red(rc=$r)"; bad=$((bad+1)); fi; done
if ( cd "$T" && "$SCRIP" --compile "$W.icn" -o w.s </dev/null >/dev/null 2>&1 && gcc -m64 -no-pie -rdynamic w.s -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o w4 2>/dev/null ); then
  for st in 1 3 5; do ( cd "$T" && env -u SCRIP_HEAP_MB $plant SCRIP_GC_STRESS=$st SCRIP_GC_RELOC=1 timeout 60s ./w4 </dev/null > m4_$st.out 2> m4_$st.err ); r=$?; if [ $r = 0 ] && cmp -s "$T/m4_$st.out" "$W.ref"; then band="$band m4@$st:ok"; else band="$band m4@$st:red(rc=$r)"; bad=$((bad+1)); fi; done
else echo "  FAIL (a) the mode-4 witness did not build"; RC=1; fi
if [ "$bad" = 0 ]; then echo "  ok   (a) THE PROPERTY: the table witness answers its iconx ref at stress 1/3/5 under forced relocation in both modes --$band"
else echo "  FAIL (a) a table lost its buckets across collections --$band"; RC=1; fi
( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_GC_PLANT_KEY_COLLISION=1 SCRIP_GC_STRESS=1 SCRIP_GC_RELOC=1 timeout 60s "$SCRIP" --run "$W.icn" </dev/null > p.out 2> p.err ); pr=$?
if [ $pr != 0 ] || ! cmp -s "$T/p.out" "$W.ref"; then echo "  ok   (b) PLANTED: SCRIP_GC_PLANT_KEY_COLLISION=1 shares the cell key with the aggregate key again and the witness is lost (rc=$pr) -- the cure is seen to be load-bearing"
else echo "  FAIL (b) the plant did not reproduce the loss -- the witness no longer exercises a name-referenced cell on a table's first word"; RC=1; fi
if grep -q 'gc_hins((void \*)((uintptr_t)tc | (gc_plant_key_collision() ? 0u : 2u)))' "$ROOT/src/runtime/rt/gc_heap.c"; then echo "  ok   (c) the name-cell key carries bit 1 in gc_visit_one (gc_heap.c)"; else echo "  FAIL (c) the name-cell key no longer carries bit 1 -- the key spaces collide again"; RC=1; fi
echo "population: 3 arm(s) graded"
if [ $RC = 0 ]; then echo "✅ GATE PASS(0) [$G]: the visited set's three key spaces are distinct and the table witness survives forced relocation"; else echo "⛔ GATE FAIL(1) [$G]: a visited-set key collision (examined 3 arms)"; fi
exit $RC
