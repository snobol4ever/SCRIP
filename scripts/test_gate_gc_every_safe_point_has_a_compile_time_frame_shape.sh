#!/usr/bin/env bash
# test_gate_gc_every_safe_point_has_a_compile_time_frame_shape.sh -- THE MAP CELL: EVERY ACTIVATION FRAME NAMES ITS
# COMPILE-TIME MAP THROUGH A DT_MAP DESCR AT THE TOP OF ITS VALUE REGION, IN BOTH MEDIA (cto 2026-09-17, CTO-65; the
# frozen design .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6 and section 7 F1/F6 step 2; law RULES.md FACT RULE
# THE COLLECTOR GUESSES NOTHING, CEO-812).
#
# WHAT IT HOLDS. (1) TAGS: descr.h and descr_tags.inc both carry DT_RAW and DT_MAP and the tag-layout verifier agrees.
# (2) STATIC, MODE 4: for every benchmark program that compiles, the .s carries one .Lgcmap_<graph> map per frame graph,
# one DT_MAP tag store (160) per map, and a counted __gc_frame_maps table whose count equals the number of maps.
# (3) LIVE, BOTH MEDIA: compiled with SCRIP_GC_MAPS_CHECK=1 (a COMPILE-TIME knob: every prologue then calls
# rt_gc_frame_map_check on its own cell), seven witnesses -- one per frontend -- run rc-identical and stdout-identical
# to their plain runs and report frames_checked >= 1 with maps >= 1. (4) PLANTED: SCRIP_GC_MAPS_PLANT=1 makes the
# emitter write a wrong slen into one cell; the check MUST abort (rc 134, BAD CELL on stderr) in both media -- the arm
# that proves the check is not decoration. (5) COST: util_gc_descr_cell_census.py runs and its frame growth reads under
# the 25 percent ceiling of section 7 F2.
# ⛔ ARM 6 EXISTS BECAUSE ARMS 1-5 PASSED OVER A REAL HOLE (coo's maps census, SCRIP f57bfb05c, 2026-09-17): the SNOBOL4
# stored-pattern graph PAT$N gets an emitted RBP activation frame (push rbp; mov rbp,rsp; sub rsp,N -- emit.cpp's R-4(b)
# blob frame) and NO map cell, in both media, so by section 6.2's rule the walker never meets a DT_MAP and runs off that
# frame into the caller's segment. Arms 2 and 3 could not see it: arm 2 compares maps to cell-stores to table-entries,
# which are self-consistent when a whole graph is missing from all three, and the seven witnesses of arm 3 carry no
# stored pattern. MEASURED at the cure's start: 3 such frames across the 74 compiling benchmark programs and 1 at the
# coo's fixture = 4. The cure is rung 2 (the frame needs a cell the walk can terminate on without moving the registry
# slots the templates address from rbp -- the GC_FRAME_MAP_BASE_CELL shape is drafted in the row's NEXT); until then this
# arm holds the count so the class cannot GROW silently, which is the one thing a named hole must not do.
# FAIL-ONCE: on origin ad0f85fae arm 1 read 0 codes, arm 2 read 0 maps, arm 3 printed no GC-MAP line. PASS-ONCE: the
# landing tree, 7 of 7 witnesses both media, planted arm rc=134 both media, growth 0.12 percent.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
CORPUS="$ROOT/../corpus/benchmarks"; [ -d "$CORPUS" ] || { echo "REFUSE(2): corpus/benchmarks not beside SCRIP"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
bad=0
n1=$(grep -c 'DT_RAW *= *0x98\|DT_MAP *= *0xA0' src/ir/descr.h); n2=$(grep -c '^#define DT_RAW *0x98\|^#define DT_MAP *0xA0' src/ir/descr_tags.inc)
if [ "$n1" -eq 2 ] && [ "$n2" -eq 2 ] && python3 scripts/util_tag_layout_verify.py >/dev/null 2>&1; then echo "  arm 1 PASS: DT_RAW/DT_MAP in both tag sources, layout verifier green"; else echo "  arm 1 RED: tag codes missing ($n1/$n2) or the layout verifier is red"; bad=1; fi
maps=0; cells=0; tabs=0; progs=0; skipped=0
for f in "$CORPUS"/*/*.sno "$CORPUS"/*/*.sc "$CORPUS"/*/*.icn "$CORPUS"/*/*.pl "$CORPUS"/*/*.reb "$CORPUS"/*/*.raku "$CORPUS"/*/*.pas; do
    [ -f "$f" ] || continue
    if ! timeout 120 ./scrip --compile "$f" > "$W/p.s" 2>/dev/null; then skipped=$((skipped+1)); continue; fi
    progs=$((progs+1))
    m=$(grep -c '^\.Lgcmap_[^ ]*:$' "$W/p.s"); c=$(grep -c 'lea *rax, \[rip + \.Lgcmap_' "$W/p.s"); t=$(awk '/^__gc_frame_maps:/{print $NF; exit}' "$W/p.s")
    maps=$((maps+m)); cells=$((cells+c)); tabs=$((tabs+${t:-0}))
    if { [ "$m" -lt 1 ] && grep -q '^main_α:\|^main_\xce\xb1:' "$W/p.s"; } || [ "$m" -ne "$c" ] || [ "${t:-0}" -ne "$m" ]; then echo "  arm 2 RED: $(basename "$f") maps=$m cell_stores=$c table_count=${t:-0}"; bad=1; fi
done
if [ "$progs" -ge 60 ]; then echo "  arm 2 PASS: $progs programs compiled ($skipped refused by the frontend), maps=$maps cell_stores=$cells table_entries=$tabs, equal per program"; else echo "  arm 2 RED: only $progs programs compiled"; bad=1; fi
run_witness() {
    local lang="$1"; local src="$2"; local base="$W/$lang"
    ./scrip "$src" > "$base.plain" 2>/dev/null; local r0=$?
    SCRIP_GC_MAPS_CHECK=1 ./scrip "$src" > "$base.m3" 2> "$base.m3err"; local r3=$?
    local f3; f3=$(grep -o 'frames_checked=[0-9]*' "$base.m3err" | tail -1 | cut -d= -f2)
    if [ "$r3" -ne "$r0" ] || ! cmp -s "$base.plain" "$base.m3" || [ "${f3:-0}" -lt 1 ]; then echo "  arm 3 RED [$lang m3]: plain rc=$r0 check rc=$r3 frames=${f3:-0} identical=$(cmp -s "$base.plain" "$base.m3" && echo yes || echo no)"; bad=1; fi
    if ( SCRIP_GC_MAPS_CHECK=1 ./scrip --compile "$src" > "$base.s" 2>/dev/null && gcc "$base.s" -o "$base.bin" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" 2>/dev/null ); then
        ( cd "$W" && "./$lang.bin" > "$base.m4" 2> "$base.m4err" ); local r4=$?
        local f4; f4=$(grep -o 'frames_checked=[0-9]*' "$base.m4err" | tail -1 | cut -d= -f2)
        if [ "$r4" -ne "$r0" ] || ! cmp -s "$base.plain" "$base.m4" || [ "${f4:-0}" -lt 1 ]; then echo "  arm 3 RED [$lang m4]: plain rc=$r0 check rc=$r4 frames=${f4:-0} identical=$(cmp -s "$base.plain" "$base.m4" && echo yes || echo no)"; bad=1; fi
        echo "  arm 3 [$lang]: m3 frames=${f3:-0} m4 frames=${f4:-0} rc=$r0"
    else echo "  arm 3 RED [$lang m4]: the witness did not build"; bad=1; fi
}
printf 'procedure q(n)\n  local i\n  every i := 1 to n do write(i * i)\nend\nprocedure main()\n  q(3)\nend\n' > "$W/w.icn"; run_witness icon "$W/w.icn"
printf 'p(1).\np(2).\nq(X, Y) :- p(X), Y is X * 2.\n:- q(X, Y), write(Y), nl, fail.\n:- true.\n' > "$W/w.pl"; run_witness prolog "$W/w.pl"
printf '        DEFINE("F(X)")             :(START)\nF       F = X * 2                   :(RETURN)\nSTART   OUTPUT = F(21)\n        S = "abc" ; S "b" = "B"\n        OUTPUT = S\nEND\n' > "$W/w.sno"; run_witness snobol4 "$W/w.sno"
cp "$CORPUS/snocone/$(ls "$CORPUS/snocone" | grep '\.sc$' | head -1)" "$W/w.sc" 2>/dev/null && run_witness snocone "$W/w.sc"
cp "$CORPUS/rebus/$(ls "$CORPUS/rebus" | grep '\.reb$' | head -1)" "$W/w.reb" 2>/dev/null && run_witness rebus "$W/w.reb"
printf 'sub sq($n) { return $n * $n }\nsay sq(7);\n' > "$W/w.raku"; run_witness raku "$W/w.raku"
printf 'program w;\nfunction sq(n: integer): integer;\nbegin sq := n * n end;\nbegin writeln(sq(9)) end.\n' > "$W/w.pas"; run_witness pascal "$W/w.pas"
SCRIP_GC_MAPS_CHECK=1 SCRIP_GC_MAPS_PLANT=1 ./scrip "$W/w.icn" > /dev/null 2> "$W/plant3.err"; p3=$?
( SCRIP_GC_MAPS_CHECK=1 SCRIP_GC_MAPS_PLANT=1 ./scrip --compile "$W/w.icn" > "$W/plant.s" 2>/dev/null && gcc "$W/plant.s" -o "$W/plant.bin" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" 2>/dev/null && ( cd "$W" && ./plant.bin > /dev/null 2> plant4.err ) ); p4=$?
if [ "$p3" -ne 0 ] && grep -q 'BAD CELL' "$W/plant3.err" && [ "$p4" -ne 0 ] && grep -q 'BAD CELL' "$W/plant4.err"; then echo "  arm 4 PASS: the planted wrong slen aborts the check in both media (rc $p3 / $p4)"; else echo "  arm 4 RED: planted cell not caught (rc $p3 / $p4)"; bad=1; fi
if python3 scripts/util_gc_descr_cell_census.py > "$W/census.txt" 2>/dev/null; then g=$(tail -1 "$W/census.txt" | grep -o 'growth_pct=[0-9.]*' | cut -d= -f2); if awk -v g="${g:-100}" 'BEGIN{exit !(g < 25)}'; then echo "  arm 5 PASS: $(tail -1 "$W/census.txt")"; else echo "  arm 5 RED: frame growth ${g:-?} percent is at or above the 25 percent ceiling"; bad=1; fi; else echo "  arm 5 RED: the cell census did not run"; bad=1; fi
BLOB_CEILING=4
blobcnt() { awk '{ if (want) { if ($0 !~ /\.Lgcmap_/) c++; want = 0 } if (p2 && $1=="sub" && $2=="rsp,") want = 1; p2 = (p1 && $1=="mov" && $2=="rbp," && $3=="rsp"); p1 = ($1=="push" && $2=="rbp"); } END{print c+0}' "$1"; }
blobs=0
for f in "$CORPUS"/*/*.sno "$CORPUS"/*/*.sc "$CORPUS"/*/*.icn "$CORPUS"/*/*.pl "$CORPUS"/*/*.reb "$CORPUS"/*/*.raku "$CORPUS"/*/*.pas "$ROOT/scripts/fixtures/gc_roots_witness.sno"; do
    [ -f "$f" ] || continue
    timeout 120 ./scrip --compile "$f" > "$W/b.s" 2>/dev/null || continue
    blobs=$((blobs + $(blobcnt "$W/b.s")))
done
if [ "$blobs" -le "$BLOB_CEILING" ]; then
    echo "  arm 6 PASS: $blobs stored-pattern BLOB activation frame(s) carry NO map cell, at or under the declared ceiling $BLOB_CEILING (a NAMED exclusion with its own walk rule, ARCH-GC section 6.2b)"
    [ "$blobs" -lt "$BLOB_CEILING" ] && echo "         ⭐ IT FELL: lower BLOB_CEILING to $blobs in this file, in the landing that paid for it"
else
    echo "  arm 6 RED: $blobs blob activation frames without a map cell, ABOVE the ceiling $BLOB_CEILING -- a NEW frame regime is emitting a frame the walker cannot terminate on"; bad=1
fi
if [ "$bad" -ne 0 ]; then echo "GATE FAIL(1) [gc_every_safe_point_has_a_compile_time_frame_shape]: an activation frame does not name its map, or the check does not see a wrong cell"; exit 1; fi
echo "GATE PASS(0) [gc_every_safe_point_has_a_compile_time_frame_shape]: every graph that reaches one of the FOUR CELL-CARRYING PROLOGUES (lcl_proc, generator, zframe, main) names its static map in both media, verified live in seven frontends, the planted cell trips, and the stored-pattern BLOB frames that carry no cell yet are held at their measured count (6 arms, 0 red)"
