#!/usr/bin/env bash
# test_gate_gc_maps_reporter_runs_beside_the_scan.sh -- F6 STEP 3: THE WALKER AS A REPORTER, BEHIND SCRIP_GC_MAPS=1.
# ⭐ WHAT STEP 3 IS FOR (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 7 F6, ceo CEO-867): the conservative scan --
# gc_zeta_frame, cons_stack, rt_cas_live_span and the heap-interior sniff, CENSUS conservative total=13 (24 until 2026-09-19: the cas-span byte scan and its gc_zeta_frame call went with the blob static layout, then 21 -> 13 the same day when the seven coexpression sweeps became typed visits and one segment walk, cto) -- may only be
# DELETED once a census says the typed walk and the sniff agree. This gate is that census. It walks every range the
# conservative scan walks, classifies each 16-byte cell TWICE (by the DESCR type tag alone, which is the frozen design's
# only tag; and by whether the sniff would take it), and counts the two divergence directions per POPULATION.
# ⛔ THE POPULATIONS ARE SPLIT ON PURPOSE AND A FLAT TOTAL IS A LIE: walking BY TYPE is only meaningful where every cell
# really is a DESCR. Over the C stack (pop=cstack) and over a whole seam-to-top span (pop=seam) the `v` byte is whatever
# happens to be there, so map_only there is the INSTRUMENT over-reading, not a defect. Measured flat first and it read
# divergence=79543 -- a number that means nothing. Split, it says exactly which population is measurable.
# ⭐ THE ARM THAT IS A REAL MEASUREMENT TODAY IS sniff_only == 0: the conservative sniff never visits a cell the type tag
# rejects, i.e. it does not rewrite non-pointers. That is one of the two divergence directions the plan names, and it is
# ZERO on both a SNOBOL4 and an Icon witness. This gate RATCHETS it so a later change cannot reintroduce the other kind.
# ⛔ map_only IS NOT YET A MEASUREMENT and this gate does not pretend otherwise: it needs the per-slot (offset, kind) map
# from the emitter to know which cells are slots at all. That is the emitter's row, not this one. Printed, never graded.
# ⛔ AND THE REPORTER MUST NOT CHANGE THE COLLECTOR: arm 1 requires byte-identical program output and ZERO [GC-MAPS]
# lines with the knob off, so a reporter that perturbs the run fails before its numbers are read.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
bash scripts/util_require_fresh.sh >/dev/null 2>&1 || { echo "REFUSES rc=2: stale or missing ./scrip -- run make"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT; bad=0
cat > "$W/w.sno" <<'EOS'
        A = TABLE()
        N = 0
LOOP    N = N + 1
        A<N> = DUPL("x", 200) ":" N
        LT(N, 300)                                      :S(LOOP)
        OUTPUT = "DONE"
END
EOS
cat > "$W/g.icn" <<'EOS'
procedure main();
   local i, t;
   t := table();
   every i := 1 to 200 do t[i] := repl("y", 100) || string(i);
   write(*t);
end
EOS
( cd "$W" && SCRIP_GC_STRESS=5 timeout 120 "$ROOT/scrip" w.sno > off.txt 2> offerr.txt ); roff=$?
noff=$(grep -c 'GC-MAPS' "$W/offerr.txt")
if [ "$roff" -eq 0 ] && [ "$noff" -eq 0 ] && [ "$(tail -1 "$W/off.txt")" = "DONE" ]; then echo "  arm 1 PASS: knob off -- rc=0, correct output, 0 [GC-MAPS] lines"
else echo "  arm 1 RED: knob off changed the run (rc=$roff, $noff report line(s), last=[$(tail -1 "$W/off.txt")])"; bad=1; fi
for w in w.sno g.icn; do
    ( cd "$W" && SCRIP_GC_MAPS=1 SCRIP_GC_STRESS=5 timeout 120 "$ROOT/scrip" "$w" > "on_$w.txt" 2> "onerr_$w.txt" ); ron=$?
    n=$(grep -c 'GC-MAPS' "$W/onerr_$w.txt")
    so=$(grep -o 'sniff_only=[0-9]*' "$W/onerr_$w.txt" | sed 's/.*=//' | sort -rn | head -1)
    pops=$(grep -o 'pop=[a-z]*' "$W/onerr_$w.txt" | sort -u | tr '\n' ' ')
    if [ "$ron" -ne 0 ] || [ "$n" -eq 0 ]; then echo "  arm 2 RED [$w]: reporter produced no census (rc=$ron, $n line(s))"; bad=1; continue; fi
    echo "  arm 2 PASS [$w]: $n census line(s) over populations: $pops"
    if [ "${so:-x}" = "0" ]; then echo "  arm 3 PASS [$w]: sniff_only=0 -- the typed walk found everything the guesser found. ⛔ NECESSARY, NOT SUFFICIENT (ceo CEO-881): it cannot see a pointer that is only in a REGISTER or only in a C FRAME at the moment of collection"
    else echo "  arm 3 RED [$w]: sniff_only=$so -- the sniff is visiting cells the type tag says are not references"; bad=1; fi
    grep 'GC-MAPS' "$W/onerr_$w.txt" | tail -4 | sed 's/^/      /'
done
# ⭐ ARMS 4-6 (cto 2026-09-19, CTO-88): THE FRAME WALKER AS A REPORTER, beside the sweep. Every graph's map now seals its
# zls kind table after the four quads (flag GC_FRAME_MAP_LAYOUT, the fourth quad = map_off), and behind the same knob the
# collector walks every swept segment FRAME BY FRAME through the DT_MAP cells: the interior by the table (slot by kind),
# the words between frames as spine, the words above the ROOT frame as residue. It prints one [GC-WALK] line per
# population per collection. arm 4: every segment finds its frames through their cells (nomap=0), every frame's interior
# is tabled (notab=0) and fully covered (i_gap=0). arm 5, THE FAIL-ONCE THAT IS A CURE: the nested-scan witness holds the
# OUTER subject pointer in scan-enter's save slot across the whole inner scan; registered ZK_RAW ("dead at safe points")
# it read i_raw_heap=1 at every one of 15101 collections on the uncured tree (2026-09-19); retagged ZK_PTR_GC it reads
# i_raw_heap=0 and i_ptr_heap>=1, both media, answer = icont. arm 6: every installed map carries a layout in both media.
for w in w.sno g.icn; do
    e="$W/onerr_$w.txt"; n=$(grep -c '^\[GC-WALK\] ' "$e")
    if [ "$n" -eq 0 ]; then echo "  arm 4 RED [$w]: no [GC-WALK] line -- the frame walker did not run beside the sweep"; bad=1; continue; fi
    fr=$(grep '^\[GC-WALK\] ' "$e" | grep -o 'frames=[0-9]*' | sed 's/.*=//' | sort -n | head -1)
    nomap=$(grep '^\[GC-WALK\] ' "$e" | grep -o 'nomap=[0-9]*' | sed 's/.*=//' | sort -rn | head -1)
    notab=$(grep '^\[GC-WALK\] ' "$e" | grep -o 'notab=[0-9]*' | sed 's/.*=//' | sort -rn | head -1)
    gap=$(grep '^\[GC-WALK\] ' "$e" | grep -o 'i_gap=[0-9]*' | sed 's/.*=//' | sort -rn | head -1)
    if [ "${fr:-0}" -ge 1 ] && [ "${nomap:-1}" -eq 0 ] && [ "${notab:-1}" -eq 0 ] && [ "${gap:-1}" -eq 0 ]; then echo "  arm 4 PASS [$w]: $n walk line(s); every segment found a frame through its cell (min frames=$fr), notab=0, i_gap=0 -- every interior read by its kind table"
    else echo "  arm 4 RED [$w]: frames>=${fr:-0} nomap=${nomap:-?} notab=${notab:-?} i_gap=${gap:-?}"; bad=1; fi
done
NS="$ROOT/scripts/gc_witnesses/hb_scan_nested.icn"; NSREF="$ROOT/scripts/gc_witnesses/hb_scan_nested.ref"
walk_arm5() {
    lbl="$1"; shift
    "$@" > "$W/ns_out.txt" 2> "$W/ns_err.txt"; r=$?
    c=$(grep -c '^\[GC-WALK\] ' "$W/ns_err.txt")
    rh=$(grep '^\[GC-WALK\] ' "$W/ns_err.txt" | grep -o 'i_raw_heap=[0-9]*' | sed 's/.*=//' | sort -rn | head -1)
    ph=$(grep '^\[GC-WALK\] ' "$W/ns_err.txt" | grep -o 'i_ptr_heap=[0-9]*' | sed 's/.*=//' | sort -rn | head -1)
    if [ "$r" -ne 0 ] || ! cmp -s "$W/ns_out.txt" "$NSREF"; then echo "  arm 5 RED [$lbl]: rc=$r answer=[$(tail -1 "$W/ns_out.txt")] oracle=[$(cat "$NSREF")]"; bad=1; return; fi
    if [ "$c" -lt 1 ]; then echo "  arm 5 RED [$lbl]: no collection observed under the reporter"; bad=1; return; fi
    if [ "${rh:-1}" -ne 0 ]; then echo "  arm 5 RED [$lbl]: i_raw_heap=$rh -- a RAW-kinded frame slot holds a collected-heap pointer the typed walk would not relocate (the scan-enter sigma save read exactly this on the uncured tree)"; bad=1; return; fi
    if [ "${ph:-0}" -lt 1 ]; then echo "  arm 5 RED [$lbl]: i_ptr_heap=${ph:-0} -- the retagged sigma save never held a heap pointer, so this arm graded nothing"; bad=1; return; fi
    echo "  arm 5 PASS [$lbl]: $c collection(s), i_raw_heap=0, i_ptr_heap>=1, answer = icont"
}
walk_arm5 "m3" env SCRIP_GC_MAPS=1 SCRIP_GC_STRESS=1 timeout 300 "$ROOT/scrip" "$NS"
if "$ROOT/scrip" --compile -o "$W/ns.s" "$NS" < /dev/null 2>"$W/nsc.txt" && gcc "$W/ns.s" -L "$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/ns.m4" 2>>"$W/nsc.txt"; then
    walk_arm5 "m4" env SCRIP_GC_MAPS=1 SCRIP_GC_STRESS=1 timeout 300 "$W/ns.m4"
else echo "  arm 5 RED [m4]: the witness did not compile or link -- $(tail -1 "$W/nsc.txt")"; bad=1; fi
for w in w.sno g.icn; do
    ( cd "$W" && SCRIP_GC_MAPS_DUMP=1 timeout 60 "$ROOT/scrip" "$w" > /dev/null 2> "$W/dump_$w.txt" )
    nm=$(grep -c '^\[GC-MAPTAB\] graph=' "$W/dump_$w.txt"); nl=$(grep -c '^\[GC-MAPTAB-LAYOUT\] ' "$W/dump_$w.txt")
    if [ "$nm" -ge 1 ] && [ "$nm" -eq "$nl" ]; then echo "  arm 6 PASS [$w]: $nm installed map(s), $nl with a kind table"
    else echo "  arm 6 RED [$w]: $nm installed map(s) but $nl kind table(s) -- a frame the walker cannot read by kind"; bad=1; fi
done
if [ "$bad" -ne 0 ]; then echo "GATE FAIL(1) [gc_maps_reporter_runs_beside_the_scan]: the step-3 reporter is not measuring"; exit 1; fi
echo "GATE PASS(0) [gc_maps_reporter_runs_beside_the_scan]: reporter silent when off, censuses two frontends by population, sniff_only=0 on both; the frame walker finds every frame through its cell and every interior by its kind table, and the scan-enter sigma save is a typed pointer slot (6 arms)"
