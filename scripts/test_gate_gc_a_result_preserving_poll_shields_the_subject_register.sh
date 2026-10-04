#!/usr/bin/env bash
# test_gate_gc_a_result_preserving_poll_shields_the_subject_register.sh -- EVERY RESULT-PRESERVING POLL SHIELDS r13 THE WAY THE
# PLAIN POLL DOES, CONTEXT-FREE (cfo ruling 2026-10-04, on hq_templates' finding at c869d57c1).
#
# THE LOSS THIS HOLDS: c869d57c1 gave the baked by-name call arms x86_rt_gc_poll_res, which spilled rax:rdx and called rt_gc_poll; the plain
# poll's slow road (rt_gc_poll_slow) also pushes r13, the match or scan subject Sigma, and hands it to gc_point_arr_body as the saved subject,
# which rewrites it when it aliases Sigma or scan_subj.  rt_gc_poll did not, so a subject held in r13 across the new poll went stale
# (ZGC-STALE, holder never visited): corpus/benchmarks/icon/tgrlink.icn rc 139 in both modes and the Icon pre-step on two programs.
# x86_rt_gc_poll_res now spills the r13 word BELOW the sweep floor and the result cell AT it, and calls rt_gc_point_arr_probe_c with the
# r13 word as the saved subject.
#
# (a) THE SHAPE in the emitted mode-4 text of a witness whose poll_res runs inside a match: the r13 store at [rsp+0], the result stores at
#     [rsp+16] and [rsp+24], rdx = rsp, rcx = rsp+16, the probe call, the three reloads.
# (b) THE PROPERTY, both modes: hb_nested_match_outer_subject.sno answers its ref at stress points 1..6 with the displacement plant on
#     (SCRIP_GC_PLANT_SHIFT=1024 moves every collected block, so a stale r13 is a wrong answer and not a lucky one).
# (c) THE PLANT: the same witness's mode-4 text with the saved-subject argument taken away (mov rdx, rsp -> xor edx, edx) is an unshielded
#     poll_res, and it LOSES at 4 or more of the 6 points -- so (b) is a measurement and not a pass by construction.
# FAIL_ONCE=1 runs (b) against the planted text.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
WD="$ROOT/scripts/gc_witnesses"; W="$WD/hb_nested_match_outer_subject"
[ -s "$W.sno" ] && [ -s "$W.ref" ] || { echo "⛔ GATE REFUSE(2) [$G]: witness or .ref missing under gc_witnesses"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
( cd "$T" && "$SCRIP" --compile -o w.s "$W.sno" < /dev/null > /dev/null 2> c.err ) || { echo "⛔ GATE REFUSE(2) [$G]: the witness did not compile in mode 4: $(head -1 "$T/c.err")"; exit 2; }
sed 's/^\( *\)mov  *rdx, rsp$/\1xor edx, edx/' "$T/w.s" > "$T/wp.s"
nplant=$(diff "$T/w.s" "$T/wp.s" | grep -c '^<')
shape=$(awk '
  /mov +qword ptr \[rsp \+ 0\], r13/ { a=NR }
  /mov +qword ptr \[rsp \+ 16\], rax/ && a && NR-a<=2 { b=NR }
  /mov +qword ptr \[rsp \+ 24\], rdx/ && b && NR-b<=2 { c=NR }
  /mov +rdx, rsp/ && c && NR-c<=8 { d=NR }
  /lea +rcx, \[rsp \+ 16\]/ && d && NR-d<=2 { e=NR }
  /call +rt_gc_point_arr_probe_c/ && e && NR-e<=8 { f=NR }
  /mov +r13, qword ptr \[rsp \+ 0\]/ && f && NR-f<=8 { g=NR }
  /mov +rdx, qword ptr \[rsp \+ 24\]/ && g && NR-g<=4 { n++; a=b=c=d=e=f=g=0 }
  END { print n+0 }' "$T/w.s")
if [ "$shape" -gt 0 ] && [ "$nplant" = "$shape" ]; then echo "  ok   (a) the poll_res shape reads whole $shape time(s) in the witness's mode-4 text: r13 below the floor, the result cell at it, rdx = the r13 word, rcx = the floor, the probe call, three reloads"
else echo "  FAIL (a) the poll_res shape reads $shape time(s) and the plant edits $nplant line(s) in the witness's mode-4 text -- x86_rt_gc_poll_res no longer hands r13 to rt_gc_point_arr_probe_c"; RC=1; fi
SRC="w"; [ "${FAIL_ONCE:-0}" = 1 ] && SRC="wp"
gcc -m64 -no-pie -rdynamic "$T/$SRC.s" -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o "$T/$SRC.bin" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: the mode-4 text did not link"; exit 2; }
gcc -m64 -no-pie -rdynamic "$T/wp.s" -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o "$T/wp.bin" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: the planted text did not link"; exit 2; }
band=""; bad=0; pband=""; phit=0
for st in 1 2 3 4 5 6; do
  ( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_HEAP_KB=128 SCRIP_GC_PLANT_SHIFT=1024 SCRIP_GC_STRESS=$st timeout 60s "$SCRIP" --run "$W.sno" < /dev/null > m3_$st.out 2> m3_$st.err ); r3=$?
  if [ $r3 = 0 ] && cmp -s "$T/m3_$st.out" "$W.ref"; then band="$band m3_s$st:ref"; else band="$band m3_s$st:LOSS"; bad=1; fi
  ( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_HEAP_KB=128 SCRIP_GC_PLANT_SHIFT=1024 SCRIP_GC_STRESS=$st timeout 60s "./$SRC.bin" < /dev/null > m4_$st.out 2> m4_$st.err ); r4=$?
  if [ $r4 = 0 ] && cmp -s "$T/m4_$st.out" "$W.ref"; then band="$band m4_s$st:ref"; else band="$band m4_s$st:LOSS"; bad=1; fi
  ( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_HEAP_KB=128 SCRIP_GC_PLANT_SHIFT=1024 SCRIP_GC_STRESS=$st timeout 60s ./wp.bin < /dev/null > p_$st.out 2> p_$st.err ); rp=$?
  if [ $rp != 0 ] || ! cmp -s "$T/p_$st.out" "$W.ref"; then pband="$pband s$st:LOSS"; phit=$((phit+1)); else pband="$pband s$st:ref"; fi
done
if [ "$bad" = 0 ]; then echo "  ok   (b) THE PROPERTY: the witness answers its ref at stress 1..6 under the displacement plant, both modes --$band"
else echo "  FAIL (b) a poll_res inside a match lost the subject --$band"; RC=1; fi
if [ "$phit" -ge 4 ]; then echo "  ok   (c) PLANTED: with the saved-subject argument taken away the same text loses at $phit of 6 points --$pband -- so (b) measures the shield"
else echo "  FAIL (c) the unshielded plant lost at only $phit of 6 points ($pband) -- the witness no longer holds a subject in r13 across a poll_res, and (b) proves nothing"; RC=1; fi
echo "population: 3 arm(s) graded"
if [ $RC = 0 ]; then echo "✅ GATE PASS(0) [$G]: x86_rt_gc_poll_res hands the subject register to the collector as the plain poll does, and the plant shows the loss it would otherwise be"
else echo "⛔ GATE FAIL(1) [$G]: a result-preserving poll does not shield r13"; fi
exit $RC
