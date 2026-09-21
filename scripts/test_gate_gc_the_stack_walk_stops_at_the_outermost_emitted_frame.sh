#!/usr/bin/env bash
# test_gate_gc_the_stack_walk_stops_at_the_outermost_emitted_frame.sh -- the emitted-stack walk is bounded ABOVE
# by the outermost emitted frame, not by the [stack] VMA end. Row
# gc-the-emitted-stack-walk-takes-a-ceiling-at-the-rt-outer-call-entry-rsp (ceo, 2026-09-21);
# ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 10.
#
# WHAT WAS WRONG. gc_seg_next stage 2 took its upper bound from gc_stack_top() -- the [stack] VMA end read out of
# /proc/self/maps -- and mode 3 enters emitted code through rt_outer_call, which carves sub $4194304,%rsp
# (src/driver/scrip.c:122). So every collection walked a 4 MB hole of dead stack. Measured on SCRIP acf601bdd at
# SCRIP_HEAP_MB=1, first collection, one segment: mode 3 read 4218328 bytes and the SAME program in mode 4 read
# 67992 -- 62.0x, of which the carve is 99.4 percent. Cost: bench_icnstr_concat_table.icn read 1614 collections /
# 5.72 s at 1 MB against 6 / 0.13 s at the default, i.e. 3.47 ms per collection to trace 88448 live bytes.
#
# WHAT HOLDS NOW. gc_walk_range already knows where the emitted stack ends: `above` goes to 1 at the top of the
# ROOT frame when no further DT_MAP cell lies above it. At that point the walker LOWERS the cached g_gc_stktop to
# that address -- no new state, the existing cache corrected -- so every later collection's segment is bounded
# there, and in the shipped path (SCRIP_GC_MAPS off) the above-region scan is skipped in the first collection too.
# ⛔ THE SAFETY ARGUMENT IS MEASURED, NOT ASSERTED: the above region was ALREADY report-only -- gc_walk_words takes
# cls=2 there and never calls gc_cell_visit -- so no word in it was ever a root. The a_heap counter measures exactly
# that population and read 1 (Icon) and 0 (SNOBOL4) before the change; those words were counted and never visited.
# Driver roots are reached through the registered root ranges and the *_gc_roots() walks, never off the driver stack.
# ⭐ AND IT MAKES THE DIVERGENCE CENSUS HONEST RATHER THAN NARROWER: sniff_only=0 used to be computed over a region
# gc_walk_range never visits, which inflated the denominator with words the sniff could not have decided; bounded,
# the census measures exactly the region where gc_cell_visit still decides. SCRIP_GC_CEILING=0 restores the old
# unbounded walk (an inert cached-getenv seam, CEO-554) and is this gate's plant.
#
# ARMS. (1) m3 and m4 agree on the STEADY-STATE bytes per collection (the reporter's cumulative bytes/ranges delta
# after the ceiling is learned) within 2x and under 64 KB each. (2) PLANTED: SCRIP_GC_CEILING=0 takes m3's
# steady-state per-collection range OVER 1 MB -- the arm that proves this gate can still see the old defect.
# (3) the witness's output is byte-identical across ceiling on/off and across both arenas. (4) the saving is
# REPORTED and not dark: [GC-COV] carries ceiling_bytes_skipped and a ceiling address.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
export SNO_LIB="${SNO_LIB:-$S4E/corpus/include}"
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
W="$ROOT/scripts/gc_witnesses/hb_arr.sno"
[ -f "$W" ] || { echo "⛔ GATE REFUSE(2) [$G]: missing $W"; exit 2; }
echo "  HOLDS: the emitted-stack walk stops at the outermost emitted frame, so mode 3's rt_outer_call carve is never scanned and the two modes read the same bytes per collection (ARCH-GC section 10)"
"$SCRIP" --compile "$W" > "$T/w.s" 2>/dev/null && gcc "$T/w.s" -o "$T/w" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: the witness will not build in mode 4"; exit 2; }
# steady-state bytes per collection = (last cumulative bytes - first) / (last ranges - 1)
steady() {
    "$@" 2>&1 | awk '/^\[GC-MAPS\] pop=cstack/ { for (i=1;i<=NF;i++) { if ($i ~ /^ranges=/) { split($i,a,"="); r=a[2] } if ($i ~ /^bytes=/) { split($i,b,"="); by=b[2] } }
        if (n++ == 0) { fb=by } lb=by; lr=r }
        END { if (n < 3 || lr < 2) { print "REFUSE" } else { printf "%d\n", (lb-fb)/(lr-1) } }'
}
M3=$(steady env SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 SCRIP_GC_MAPS=1 "$SCRIP" "$W" < /dev/null)
M4=$(steady env SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 SCRIP_GC_MAPS=1 "$T/w" < /dev/null)
OFF=$(steady env SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 SCRIP_GC_MAPS=1 SCRIP_GC_CEILING=0 "$SCRIP" "$W" < /dev/null)
for v in "$M3" "$M4" "$OFF"; do [ "$v" = "REFUSE" ] && { echo "⛔ GATE REFUSE(2) [$G]: fewer than 3 collections seen -- the witness did not exercise the walker"; exit 2; }; done
echo "  steady-state stack bytes per collection: m3=$M3 m4=$M4 plant(SCRIP_GC_CEILING=0,m3)=$OFF"
if [ "$M3" -lt 65536 ] && [ "$M4" -lt 65536 ] && [ "$M3" -lt $((M4 * 2 + 64)) ] && [ "$M4" -lt $((M3 * 2 + 64)) ]; then echo "  arm 1 PASS (the two modes read the same bounded region)"; else echo "  arm 1 FAIL (m3=$M3 m4=$M4; want both under 65536 and within 2x)"; RC=1; fi
if [ "$OFF" -gt 1048576 ]; then echo "  arm 2 PASS (planted: the unbounded walk still reads $OFF bytes per collection, so this gate can see the defect it was built for)"; else echo "  arm 2 FAIL (SCRIP_GC_CEILING=0 read $OFF bytes per collection, want over 1048576 -- the plant no longer reproduces the old walk and arm 1 proves nothing)"; RC=1; fi
h1=$(SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 "$SCRIP" "$W" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
h2=$(SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 SCRIP_GC_CEILING=0 "$SCRIP" "$W" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
h3=$("$SCRIP" "$W" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
h4=$(SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 "$T/w" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
if [ "$h1" = "$h2" ] && [ "$h1" = "$h3" ] && [ "$h1" = "$h4" ]; then echo "  arm 3 PASS (output byte-identical across ceiling on/off, both arenas, both modes)"; else echo "  arm 3 FAIL (on=$h1 off=$h2 default-arena=$h3 m4=$h4)"; RC=1; fi
cov=$(SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=3 SCRIP_GC_COVERAGE=1 "$SCRIP" "$W" < /dev/null 2>&1 | grep -m1 '^\[GC-COV\]' | sed -n 's/.*ceiling_bytes_skipped=\([0-9]*\).*/\1/p')
if [ -n "$cov" ] && [ "$cov" -gt 0 ]; then echo "  arm 4 PASS (the saving is reported: ceiling_bytes_skipped=$cov)"; else echo "  arm 4 FAIL (no ceiling_bytes_skipped on the coverage line -- a saving nobody can read is dark)"; RC=1; fi
[ $RC -eq 0 ] && echo "GATE PASS [$G]" || echo "GATE FAIL [$G]"
exit $RC
