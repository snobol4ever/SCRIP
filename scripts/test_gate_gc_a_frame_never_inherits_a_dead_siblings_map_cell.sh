#!/usr/bin/env bash
# test_gate_gc_a_frame_never_inherits_a_dead_siblings_map_cell.sh -- A PROCEDURE OR GENERATOR PROLOGUE CLEARS ITS WHOLE
# VALUE REGION, SO NO DEAD ACTIVATION'S FRAME-MAP CELL CAN SURVIVE INSIDE A LIVE FRAME (cto 2026-09-23, CTO-157; law RULES.md
# FACT RULE THE COLLECTOR GUESSES NOTHING, CEO-812).
#
# MEASURED 2026-09-23: the seven-language census named procedure_record_every_replace_14 (V9GEN: main calls p1..p14 in turn)
# GC-attributable in both modes, and the flip plant located it -- a by-name call of write received image(image([,]))'s
# string pointing into vacated ground at line 62, inside p5.  The walker finds a frame by the first valid DT_MAP cell scanning
# up from the floor.  p4 and p5 are both called from main at the same rsp, so both frames end at the same top; p4's region is
# 2160 bytes and p5's 3744, so p4's dead cell sat sixteen bytes below p5's live cell INSIDE p5's value region, never
# overwritten because p5 had not yet used that slot.  The walker took p4's cell, applied p4's layout to p5's frame and set its
# position past p4's header window -- over p5's live cell -- so p5's value cells were never visited.  gdb: at the collection
# that moved the string, five stack words held its old address and the collector registered none of them.
# A WALKER RULE WAS TRIED AND IS WRONG: rejecting a cell whose header window holds another valid cell fixed this entry and
# CRASHED four others, because the dead sibling cell sits BELOW the live one or ABOVE it depending on the two header sizes,
# so geometry cannot tell them apart.  The cure removes the ambiguity instead: the prologue already cleared the locals
# [locals, region) with rep stosb; it now clears the whole value region [0, region), so a dead cell inside a live frame is
# impossible.  Blob frames and flat zframes already cleared their data.  Cost measured: ~300k calls (fib 25, tak 18 12 6)
# 96 ms against 105 ms without, same output.  SCRIP_GC_PLANT_STALE_FRAME=1 restores the locals-only clear at emission.
#
# ARMS: (a) THE PROPERTY: the witness (the census entry, ref cut from iconx) answers its ref at SCRIP_GC_STRESS=1 and 3, under
# SCRIP_GC_RELOC=1 and under the flip plant, mode 3 and mode 4; (b) THE PLANT: SCRIP_GC_PLANT_STALE_FRAME=1 loses the answer
# at stress 1 and prints the GC-STALEFRAME banner; (c) THE EMISSION: every procedure prologue of the witness that writes a map
# cell at [rsp + R] clears R bytes from rsp.  FAIL_ONCE=1 grades arm (a) under the plant and requires it to red.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
W="$ROOT/scripts/gc_witnesses/hb_sibling_frames_leave_no_dead_map_cell"; [ -s "$W.icn" ] && [ -s "$W.ref" ] || { echo "⛔ GATE REFUSE(2) [$G]: witness or its iconx ref missing"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; plant=""; [ "${FAIL_ONCE:-0}" = 1 ] && plant="SCRIP_GC_PLANT_STALE_FRAME=1"
( cd "$T" && env $plant "$SCRIP" --compile "$W.icn" -o w.s </dev/null >/dev/null 2>&1 && gcc -m64 -no-pie w.s -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o w4 2>/dev/null ) || { echo "  FAIL (a) the mode-4 witness did not build"; RC=1; }
band=""; bad=0
for cfg in "SCRIP_GC_STRESS=1" "SCRIP_GC_STRESS=3" "SCRIP_GC_STRESS=1 SCRIP_GC_RELOC=1" "SCRIP_GC_STRESS=1 SCRIP_GC_PLANT_FLIP=1"; do
  ( cd "$T" && env -u SCRIP_HEAP_MB $plant $cfg timeout 120s "$SCRIP" --run "$W.icn" </dev/null > m3.out 2>m3.err ); r=$?; if [ $r = 0 ] && cmp -s "$T/m3.out" "$W.ref"; then band="$band m3[$cfg]:ok"; else band="$band m3[$cfg]:red(rc=$r)"; bad=$((bad+1)); fi
  case "$cfg" in *PLANT_FLIP=1*) grep -q "^\[GC-FLIP\] plant:" "$T/m3.err" || { band="$band m3[$cfg]:flip-not-applied"; bad=$((bad+1)); } ;; esac
  if [ -x "$T/w4" ]; then ( cd "$T" && env -u SCRIP_HEAP_MB $cfg timeout 120s ./w4 </dev/null > m4.out 2>/dev/null ); r=$?; if [ $r = 0 ] && cmp -s "$T/m4.out" "$W.ref"; then band="$band m4[$cfg]:ok"; else band="$band m4[$cfg]:red(rc=$r)"; bad=$((bad+1)); fi; fi
done
if [ "$bad" = 0 ] && [ -x "$T/w4" ]; then echo "  ok   (a) THE PROPERTY: sibling procedures leave no dead map cell the walker can take -- stress 1 and 3, RELOC and the flip plant, both modes:$band"
else echo "  FAIL (a) a live frame was walked with a dead sibling's layout --$band"; RC=1; fi
( cd "$T" && env -u SCRIP_HEAP_MB SCRIP_GC_PLANT_STALE_FRAME=1 SCRIP_GC_STRESS=1 timeout 120s "$SCRIP" --run "$W.icn" </dev/null > p.out 2> p.err ); pr=$?
if { [ $pr != 0 ] || ! cmp -s "$T/p.out" "$W.ref"; } && grep -q "^\[GC-STALEFRAME\] plant:" "$T/p.err"; then echo "  ok   (b) PLANTED: SCRIP_GC_PLANT_STALE_FRAME=1 clears only the locals again, the GC-STALEFRAME banner proves it applied, and the witness is lost (rc=$pr) -- the clear is load-bearing"
else echo "  FAIL (b) the plant did not lose the answer (rc=$pr) or its GC-STALEFRAME banner is missing"; RC=1; fi
miss=0; seen=0
if [ -s "$T/w.s" ]; then
  while IFS= read -r fn; do n=$(grep -n "^$fn:" "$T/w.s" | head -1 | cut -d: -f1); [ -n "$n" ] || continue
    blk=$(sed -n "$n,$((n+16))p" "$T/w.s"); R=$(printf '%s\n' "$blk" | grep -oE 'dword ptr \[rsp \+ [0-9]+\], 160' | head -1 | grep -oE '[0-9]+\]' | tr -d ']'); [ -n "$R" ] && [ "$R" -gt 0 ] || continue
    seen=$((seen+1)); printf '%s\n' "$blk" | grep -q "mov  *ecx, $R\$" || { miss=$((miss+1)); echo "      prologue $fn writes its map cell at [rsp + $R] and does not clear $R bytes"; }
  done < <(grep -oE '^FN__[A-Za-z0-9_]+' "$T/w.s" | sort -u)
fi
if [ "$seen" -ge 10 ] && [ "$miss" = 0 ]; then echo "  ok   (c) THE EMISSION: all $seen procedure prologues of the witness with a non-empty value region clear it whole before any value is stored"
else echo "  FAIL (c) $miss of $seen procedure prologues do not clear their whole value region (need at least 10 examined)"; RC=1; fi
echo "population: 3 arm(s) graded"
if [ $RC = 0 ]; then echo "✅ GATE PASS(0) [$G]: a frame never inherits a dead sibling's map cell"; else echo "⛔ GATE FAIL(1) [$G]: a live frame can carry a dead activation's map cell (examined 3 arms)"; fi
exit $RC
