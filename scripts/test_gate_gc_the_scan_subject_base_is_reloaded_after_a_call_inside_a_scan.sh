#!/usr/bin/env bash
# test_gate_gc_the_scan_subject_base_is_reloaded_after_a_call_inside_a_scan.sh -- A CALL MADE INSIDE A SCAN BODY HANDS
# THE SUBJECT BASE BACK FROM THE COLLECTOR'S OWN ROOT, NOT FROM THE REGISTER THE CALLEE LEFT ALONE (cto 2026-09-24,
# CTO-165; row icon-gc-procedure-record-scan-replace-2-goes-crash-to-diff-across-fa1dc84a5-...; law RULES.md FACT RULE
# THE COLLECTOR GUESSES NOTHING, CEO-812; hq_icon's 15-line witness, FINDING-2026-09-24-hq_icon-htprep-stale-scan-subject).
#
# MEASURED 2026-09-24 on SCRIP 8b6cb3607: the Icon master entry procedure_record_scan_replace_2 (htprep) reads its ref at
# stress 0 and prints NUL-filled garbage where scanned text belongs at stress 5 under forced relocation, differently on
# every run.  Under the flip plant the fault is located: the inline cset test of upto in braces (movzx esi, byte ptr
# [r13+rcx]) reads a 64-byte HB_WSB string block (the line trim(read()) returned, the scan subject) 20 bytes in, at the
# address it had BEFORE the collection newtag's tab(many(&letters)) caused MOVED it.  The emitted call successor inside a
# scan body carried the POSITION back through rt_scan_sync_in (scan_pos is runtime state the collector roots) but left
# r13, the subject base, to the callee-saved convention: the callee never touched r13, no frame slot held it during the
# collection, and the collector guesses nothing about a C-saved register -- so the caller resumed on the old ground.
#
# THE CURE (x86_asm.h, x86_scan_sigma_reload, emitted by both scan sync-in helpers when the scan registers are live):
# after the position, r13 is reloaded from rt_scan_live_subj, the runtime's scan_subj, which gen_gc_roots roots and the
# collector relocates -- the same road the position already travelled.  The plant SCRIP_GC_PLANT_STALE_SIGMA=1 leaves the
# reload out at emit time and prints its banner once, so the loss can be seen to return.
#
# ARMS: (a) THE PROPERTY: the witness answers its iconx-cut ref at the shipped 128 KB window in mode 3 at stress 3 and 11
# and in mode 4 at stress 4, 8 and 16 (the levels hq_icon measured as the diverging ones), and under the flip plant at
# stress 3 (mode 3) and 4 (mode 4), stdin fed from its .in; (b) PLANTED: under SCRIP_GC_PLANT_STALE_SIGMA=1 the same
# witness at mode-3 stress 3 fails to answer its ref (DIFF or fault) AND the GC-STALESIGMA banner is on stderr -- an
# instrument never seen to fire is not known to look; (c) the source carries the reload in both sync-in helpers.
# FAIL_ONCE=1 runs arm (a) under the plant and requires it to red.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
WD="$ROOT/scripts/gc_witnesses"; W="$WD/hb_scan_subject_across_a_failing_callee"
[ -s "$W.icn" ] && [ -s "$W.in" ] && [ -s "$W.ref" ] || { echo "⛔ GATE REFUSE(2) [$G]: witness, .in or .ref missing under gc_witnesses"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; plant=""; [ "${FAIL_ONCE:-0}" = 1 ] && plant="SCRIP_GC_PLANT_STALE_SIGMA=1"
band=""; bad=0
flipped() { local err="$1" fl="$2"; [ -z "$fl" ] && return 0; grep -q "^\[GC-FLIP\] plant:" "$err"; }
run3() { local st="$1" fl="$2" tag="$3"; ( cd "$T" && env -u SCRIP_HEAP_MB $plant $fl SCRIP_HEAP_KB=128 SCRIP_GC_STRESS=$st timeout 60s "$SCRIP" --run "$W.icn" < "$W.in" > "$tag.out" 2> "$tag.err" ); local r=$?
  if [ $r = 0 ] && cmp -s "$T/$tag.out" "$W.ref" && flipped "$T/$tag.err" "$fl"; then band="$band $tag:ref"; else band="$band $tag:rc=$r"; bad=1; fi; }
for st in 3 11; do run3 $st "" "m3_s$st"; done
run3 3 "SCRIP_GC_PLANT_FLIP=1" "m3_f3"
if ( cd "$T" && env $plant "$SCRIP" --compile "$W.icn" -o w.s </dev/null >/dev/null 2>w4.err && gcc -m64 -no-pie -rdynamic w.s -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o w4 2>/dev/null ); then
  run4() { local st="$1" fl="$2" tag="$3"; ( cd "$T" && env -u SCRIP_HEAP_MB $fl SCRIP_HEAP_KB=128 SCRIP_GC_STRESS=$st timeout 60s ./w4 < "$W.in" > "$tag.out" 2> "$tag.err" ); local r=$?
    if [ $r = 0 ] && cmp -s "$T/$tag.out" "$W.ref" && flipped "$T/$tag.err" "$fl"; then band="$band $tag:ref"; else band="$band $tag:rc=$r"; bad=1; fi; }
  for st in 4 8 16; do run4 $st "" "m4_s$st"; done
  run4 4 "SCRIP_GC_PLANT_FLIP=1" "m4_f4"
else echo "  FAIL (a) the mode-4 witness did not build"; RC=1; fi
if [ "$bad" = 0 ]; then echo "  ok   (a) THE PROPERTY: the scan witness answers its iconx ref after a failing callee's collection at the shipped window, both modes, and under the flip plant --$band"
else echo "  FAIL (a) the subject base read after a call inside a scan is stale --$band"; RC=1; fi
( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_GC_PLANT_STALE_SIGMA=1 SCRIP_HEAP_KB=128 SCRIP_GC_STRESS=3 timeout 60s "$SCRIP" --run "$W.icn" < "$W.in" > p.out 2> p.err ); pr=$?
if { [ $pr != 0 ] || ! cmp -s "$T/p.out" "$W.ref"; } && grep -q "^\[GC-STALESIGMA\] plant:" "$T/p.err"; then echo "  ok   (b) PLANTED (the GC-STALESIGMA banner proves the plant applied): without the reload the witness diverges at stress 3 (rc=$pr)"
else echo "  FAIL (b) the plant did not reproduce the loss (rc=$pr), or its GC-STALESIGMA banner is missing so it never applied -- the witness no longer collects inside the callee"; RC=1; fi
n=$(grep -c 'x86_scan_sigma_reload()' "$ROOT/src/templates/x86/x86_asm.h")
if [ "$n" -ge 3 ] && grep -q 'rt_scan_live_subj' "$ROOT/src/templates/x86/x86_asm.h"; then echo "  ok   (c) both scan sync-in helpers emit the sigma reload from rt_scan_live_subj (x86_asm.h, $n mentions)"
else echo "  FAIL (c) the sigma reload is missing from a sync-in helper in x86_asm.h ($n mentions)"; RC=1; fi
echo "population: 3 arm(s) graded"
if [ $RC = 0 ]; then echo "✅ GATE PASS(0) [$G]: a call inside a scan body hands the subject base back from the collector's root, and the plant shows the loss it would otherwise be"; else echo "⛔ GATE FAIL(1) [$G]: the scan subject base is stale after a call inside a scan, or the plant no longer shows the loss"; fi
exit $RC
