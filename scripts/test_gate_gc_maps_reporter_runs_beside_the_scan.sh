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
if [ "$bad" -ne 0 ]; then echo "GATE FAIL(1) [gc_maps_reporter_runs_beside_the_scan]: the step-3 reporter is not measuring"; exit 1; fi
echo "GATE PASS(0) [gc_maps_reporter_runs_beside_the_scan]: reporter silent when off, censuses two frontends by population, sniff_only=0 on both"
