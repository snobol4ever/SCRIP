#!/usr/bin/env bash
# test_gate_gc_every_allocating_runtime_entry_returns_to_a_safe_point.sh -- THE ALLOCATOR NEVER COLLECTS (frozen design
# .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 7 F4, F6 step 1; Lon 2026-09-17 CEO-812 "the collector guesses
# nothing"; cfo's row gc-safe-points-the-allocator-never-collects-...).
#
# WHAT STEP 1 ESTABLISHES, AND WHAT IT DOES NOT. c_rt_gcheap_alloc collected at three unshielded sites (gc_heap.c :192
# the detax fast path, :206 the pacing line, :207 the end of the arena) with every runtime C frame live and unmapped.
# Those three now set g_gc_pending; the arena is a contiguous virtual reserve (mmap PROT_NONE MAP_NORESERVE, cap
# SCRIP_HEAP_MAX_MB, default 8x SCRIP_HEAP_MB) whose soft end g_hp_end advances 2 MB at a time when the allocator must
# not collect, and the exhaustion abort fires only at the cap. Arms 1-3 measure exactly that and BLOCK.
#
# ⛔ ARM 4 IS A PRINTED CENSUS AND DOES NOT BLOCK, BY THE cfo's MEASUREMENT 2026-09-17 ON THIS CHANGE: an Icon
# allocation loop (4 million dead lists) reaches NO safe point at all -- origin collected 5 times through the
# allocator's own pacing site and read 157 MB RSS; with the allocator silent it collects ONCE, at the end, and reads
# 842 MB. The shielded rt_gc_point_arr sites F4 keeps are SNOBOL4/Prolog dispatch points and Icon's loops contain none,
# so "every allocating runtime entry returns to a safe point" is FALSE today and cannot be made true from the runtime
# side. It becomes true at F6 step 4 (the cto's polls at every allocating return). Arm 4 prints the collections-per-
# program rate that step 4 must raise; WHEN STEP 4 LANDS, ARM 4 BECOMES BLOCKING AND THE BOUND IS A NON-ZERO RATE ON
# THE ICON WITNESS. Until then it is the instrument, not the verdict, and this header is the reason why.
#
# FAIL-ONCE IS MEASURED, NOT ASSERTED: run against origin 162e8a172 (the cure stashed, the runtime rebuilt, the gate
# file left in place) all three blocking arms read red -- census 3 collect calls, reserve 0 soft-end advances, cap
# rc=0 with no cap-named exhaustion -- and all three read green on the cure. FAIL_ONCE=1 additionally plants the
# pre-cure census count for a cheap re-check that arm 1 still discriminates without a rebuild.
# ⛔ ARMS 2 AND 3 CHANGED THEIR WITNESS 2026-09-17 (cto, CEO-821 F6 step 1b), AND THE REASON IS THE POINT OF THIS ROW:
# they grew the arena with FOUR HUNDRED THOUSAND DEAD LISTS, which grew only because an Icon allocation loop reached no
# safe point. With the Icon allocating-box polls landed that loop COLLECTS instead of growing (soft-end advances 0), so
# the arms read red on a tree that is strictly better. The witness is now 400000 LIVE lists held by one list: growth is
# unavoidable there whatever the collector does, which is what these two arms actually grade -- 30 soft-end advances at
# SCRIP_HEAP_MB=8 SCRIP_HEAP_MAX_MB=512, and a cap-named abort (rc=134) when the cap equals the window.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
SRC="$ROOT/src/runtime/rt/gc_heap.c"; [ -f "$SRC" ] || { echo "⛔ REFUSE(2): $SRC absent"; exit 2; }
command -v /usr/bin/time >/dev/null || { echo "⛔ REFUSE(2): /usr/bin/time is the RSS instrument and it is absent"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; examined=0
examined=$((examined+1))
n=$(grep -c 'rt_gc_collect();' "$SRC")
[ -n "${FAIL_ONCE:-}" ] && n=3
if [ "$n" -eq 0 ]; then echo "  census PASS (the allocator contains 0 collect-from-inside-a-runtime-call sites in gc_heap.c)"
else echo "  census FAIL ($n collect call(s) inside gc_heap.c: a collection with live unmapped C frames is back)"; RC=1; fi
examined=$((examined+1))
printf 'procedure main()\n   local i, L;\n   L := [];\n   every i := 1 to 400000 do put(L, [i, i+1, i+2]);\n   write("done ", *L);\nend\n' > "$T/grow.icn"
out=$(cd "$T" && SCRIP_ZETA_TELEM=1 SCRIP_HEAP_MB=8 SCRIP_HEAP_MAX_MB=512 timeout 120 "$SCRIP" grow.icn 2>&1); g=$(printf '%s\n' "$out" | grep -c 'soft end ->')
if [ "$(printf '%s\n' "$out" | grep -c '^done 400000$')" = 1 ] && [ "$g" -gt 0 ]; then echo "  reserve PASS (an 8 MB window inside a 512 MB reserve grew its soft end $g time(s) into the reserve instead of collecting, answer intact)"
else echo "  reserve FAIL (soft-end advances=$g, answer=[$(printf '%s\n' "$out" | grep '^done' | head -1)] -- the arena did not grow when the allocator refused to collect)"; RC=1; fi
examined=$((examined+1))
out=$(cd "$T" && SCRIP_HEAP_MB=8 SCRIP_HEAP_MAX_MB=8 timeout 120 "$SCRIP" grow.icn 2>&1); r=$?
if [ "$r" -ne 0 ] && printf '%s\n' "$out" | grep -q 'heap exhausted at the reserve cap'; then echo "  cap PASS (cap == window: growth is refused at the cap and the abort names it, rc=$r)"
else echo "  cap FAIL (rc=$r, no cap-named exhaustion -- the reserve is unbounded or the abort does not name the cap)"; RC=1; fi
examined=$((examined+1))
printf 'procedure main()\n   local i, L;\n   every i := 1 to 4000000 do L := [i, i+1, i+2];\n   write("done ", *L);\nend\n' > "$T/churn.icn"
( cd "$T" && SCRIP_ZETA_TELEM=1 /usr/bin/time -f 'RSS=%M' timeout 180 "$SCRIP" churn.icn ) >"$T/c.out" 2>"$T/c.err"
col=$(grep -c 'ZGC. regeneration' "$T/c.err"); rss=$(grep -o 'RSS=[0-9]*' "$T/c.err" | tail -1 | cut -d= -f2)
echo "  ⚠ safe-point coverage CENSUS, NOT BLOCKING until F6 step 4 lands the emitted polls:"
echo "      the Icon 4M-dead-list witness took $col collection(s) at ${rss:-?} KB RSS (origin through the allocator: 5 collections at 157144 KB, measured by the cfo 2026-09-17 on 162e8a172)"
echo "      an Icon allocation loop contains no shielded rt_gc_point_arr site, so until the polls exist nothing collects inside it; step 4 must raise this rate and then this arm blocks"
if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$(basename "${BASH_SOURCE[0]}" .sh)]: the allocator never collects, the arena is a virtual reserve that grows and is capped (examined $examined arms; the coverage census is printed, not blocking, until F6 step 4)"
else echo "GATE FAIL(1) [$(basename "${BASH_SOURCE[0]}" .sh)]: the allocator collects again, or the reserve does not grow or is not capped (examined $examined arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
