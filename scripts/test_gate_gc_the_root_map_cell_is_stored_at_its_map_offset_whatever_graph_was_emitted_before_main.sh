#!/usr/bin/env bash
# test_gate_gc_the_root_map_cell_is_stored_at_its_map_offset_whatever_graph_was_emitted_before_main.sh
#
# WHAT THIS GATE HOLDS (cto 2026-10-04, found by the differential check of the chain over the maps,
# ARCH-GC section 13): main's root frame-map cell is stored at main's map_off from main_alpha's rsp in
# every program. The SNOBOL4 root map cell is emitted at main's prologue through a frame-relative
# operand (emit_gc_map_cell(..., frame_rel 1) -> FRQ -> x86_frame_off, which adds g_emit.op_zdepth).
# op_zdepth is NODE state, set at each node's start; at main's prologue no node has started, and it
# held whatever the PREVIOUS graph's last node left. hb_protected_pattern_value_read.sno emits its
# stored pattern PAT$0 before main, PAT$0's last node carved 16, so main's cell landed at rsp+592 with
# map_off 576 -- and the marker scan, which computes the frame base as the cell's address minus
# map_off, applied main's whole layout 16 bytes too high: every layout entry read the slot above it,
# so a live DESCR under a RAW or gap entry went unvisited. The cure resets the node-scoped depth at
# every graph's start (codegen_flat_chain_body).
#
# ARMS: (1) the row's witness: the DT_MAP tag store in main_alpha's mode-4 text is at [rsp + map_off]
# for map_off read off the emitter's own [GC-MAP] line; (2) the same over every SNOBOL4 gc_witness;
# (3) FAIL_ONCE=1 compares against map_off + 16 to prove arm 1 can say no.
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
"$ROOT/scripts/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
SCRIP="$ROOT/scrip"
WIT="$ROOT/scripts/gc_witnesses/hb_protected_pattern_value_read.sno"
G="test_gate_gc_the_root_map_cell_is_stored_at_its_map_offset_whatever_graph_was_emitted_before_main"
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -f "$WIT" ] || refuse "the row's witness $WIT is missing"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
bump=0; [ "${FAIL_ONCE:-0}" = 1 ] && bump=16
reading() {
  local prog="$1" b; b="$(basename "$prog")"
  ( cd "$W" && SCRIP_GC_MAPS_REPORT=1 timeout 60 "$SCRIP" --compile "$prog" > "$W/$b.s" 2> "$W/$b.rep" < /dev/null ) || { echo "COMPILE-FAILED"; return; }
  local mo cell
  mo="$(sed -n 's/^\[GC-MAP\] graph=main frame_bytes=[0-9]* header_bytes=[0-9]* map_off=\([0-9]*\) flags=[0-9]*$/\1/p' "$W/$b.rep" | head -1)"
  [ -n "$mo" ] || { echo "NO-ROOT-MAP"; return; }
  cell="$(awk '/^main_α:/{on=1} on && /lea +rax, \[rip \+ \.Lgcmap_main\]/{lea=1; next} lea && /mov +dword ptr \[rsp \+ [0-9]+\], 160/{match($0, /\[rsp \+ [0-9]+\]/); s=substr($0, RSTART+7, RLENGTH-8); print s; exit} /^main_α_body:/{exit}' "$W/$b.s")"
  [ -n "$cell" ] || { echo "NO-CELL-STORE map_off=$mo"; return; }
  echo "map_off=$mo cell=$cell"
}
r="$(reading "$WIT")"
mo="$(printf '%s\n' "$r" | sed -n 's/^map_off=\([0-9]*\) cell=.*/\1/p')"; cell="$(printf '%s\n' "$r" | sed -n 's/.* cell=\([0-9]*\)$/\1/p')"
if [ -z "$mo" ] || [ -z "$cell" ]; then refuse "the witness could not be read: $r"; fi
if [ "$cell" = "$((mo + bump))" ]; then
  ck ok "(1) the row's witness stores main's DT_MAP cell at [rsp + $cell], its map_off $mo -- the stored pattern PAT\$0 emitted before main leaves no depth behind"
else
  ck no "(1) the row's witness stores main's DT_MAP cell at [rsp + $cell] with map_off $mo$( [ "$bump" = 16 ] && echo ' (FAIL_ONCE compares against map_off + 16)') -- a node depth from the graph emitted before main leaked into the root cell, and the marker scan reads main's layout $((cell - mo)) bytes high"
fi
n=0; bad=0; badl=""
for f in "$ROOT"/scripts/gc_witnesses/*.sno; do
  r="$(reading "$f")"; case "$r" in map_off=*) ;; *) continue;; esac
  n=$((n+1)); m="$(printf '%s\n' "$r" | sed -n 's/^map_off=\([0-9]*\) cell=.*/\1/p')"; c="$(printf '%s\n' "$r" | sed -n 's/.* cell=\([0-9]*\)$/\1/p')"
  [ "$c" = "$m" ] || { bad=$((bad+1)); badl="$badl $(basename "$f"):$c/$m"; }
done
[ "$n" -gt 0 ] || refuse "no SNOBOL4 gc_witness yielded a root map reading -- a zero over an unmeasured population"
if [ "$bad" = 0 ]; then ck ok "(2) every one of $n SNOBOL4 gc_witnesses stores main's root cell at its map_off"; else ck no "(2) $bad of $n SNOBOL4 gc_witnesses store main's root cell away from map_off (cell/map_off):$badl"; fi
echo "population: $checks arm(s) graded, $fails FAIL"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$G]"; exit 0; fi
echo "⛔ GATE RED [$G]: $fails of $checks arms FAIL"; exit 1
