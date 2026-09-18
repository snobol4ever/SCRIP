#!/usr/bin/env bash
# test_gate_gc_map_table_present_in_both_media.sh -- rung 2 of the compile-time frame maps
# (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6, F1; row
# gc-the-compile-time-map-table-is-emitted-beside-the-code-in-both-media-...).
#
# WHAT THIS GATE HOLDS. Rung 1 (CTO-65) gave every activation frame a DT_MAP cell naming a 32-byte
# static map, and gave both media an install path: mode 3 registers each chain's map through
# rt_gc_frame_maps_add as it seals it, mode 4 emits a counted __gc_frame_maps .rodata table that
# main's prologue hands to rt_gc_frame_maps_install_counted. NOTHING READ THE TABLE BACK. The only
# reporter in the tree was SCRIP_GC_MAPS_REPORT, which prints what the EMITTER intended at compile
# time -- it cannot see a table that was emitted and never installed, and it cannot see the two media
# disagreeing. This gate reads the installed table back at RUN time through the one runtime symbol
# rt_gc_frame_maps() (SCRIP_GC_MAPS_DUMP=1) in BOTH media and requires the two readings to agree
# entry for entry.
#
# ⛔ WHAT THE READ-BACK FOUND THE FIRST TIME IT RAN (2026-09-18, cto), which is the whole argument for
# grading the artefact instead of the intent -- TWO cross-media identity divergences, both invisible
# to every instrument that existed:
#   (1) THE ENTRY GRAPH HAD TWO NAMES. The driver emits the program's main flat chain as the literal
#       "pat_flat" in mode 3 (scrip.c) and "main" in mode 4, so one graph's map carried two identities.
#       The fact was already KNOWN and PAPERED OVER in two other places rather than cured: util_gc_census.py
#       carried ENTRY_ALIAS = ("pat_flat", "main"), and emit.cpp tested both spellings. Three copies of
#       one fact, held by memory rather than by a check -- which is the state the INSTRUMENT LAWS exist
#       to end. Cured at the source: the map's graph_name is normalised once, in one named function.
#   (2) THE ASSEMBLER MANGLING LEAKED INTO THE MAP. Mode 4 must pass an assembler-legal prefix, so the
#       driver passes asm_sym_name(pname); mode 3 passes the raw name. The map's graph_name is DATA read
#       by the collector and the census, not a symbol, and it was taking whichever spelling the medium
#       needed: a Prolog program named every one of its ~160 graphs app/3, write/1, =../2 in mode 3 and
#       app$2F3, write$2F1, =..$2F2 in mode 4. Geometry agreed; identity did not. Cured by having the map
#       carry the graph's TRUE name (zls_graph_name_get) in both media while labels keep their mangling.
#   Neither divergence corrupts memory today, because the walker reaches a map through the frame's cell
#   rather than by name. Both make the map LIE about which graph a frame belongs to, and a collector
#   that guesses nothing cannot be built on metadata that does.
#
# ⛔ THE NAMED EXCLUSION, stated so this gate cannot read green by never looking (6.2b): a SNOBOL4
# stored-pattern blob frame (emit.cpp R-4(b)) emits NO map cell and therefore NO map, so it appears in
# neither medium's table. That exclusion is held at its measured count by arm 6 of the rung-1 gate, not
# here. This gate grades the graphs that DO carry a map, and PRINTS how many that was in each medium, so
# a table that silently emptied reds arm 1 instead of passing as "no divergence".
#
# ARMS (all blocking): (1) PRESENT -- each witness installs a NON-EMPTY table in BOTH media, read back
# through the one runtime symbol; (2) AGREE -- the two readings are identical entry for entry (graph
# name, frame_bytes, header_bytes, flags), across three frontends; (3) IDENTITY -- the entry graph is
# named "main" in both media and no entry carries the assembler mangling tell ($2F), which is the
# regression guard for the two divergences above; (4) PLANTED -- SCRIP_GC_MAPTAB_PLANT=1 on the mode-4
# compile alone must make the two readings DISAGREE: a guard never seen to fire is not known to hold
# (CEO-554), and if this comes back green the read-back has stopped reading.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
export SNO_LIB="${SNO_LIB:-$S4E/corpus/include}"
echo "  HOLDS: the compile-time frame-map table is emitted beside the code in BOTH media and INSTALLED -- mode 3 registers each sealed chain, mode 4 hands main's prologue a counted __gc_frame_maps .rodata table -- and the table read back at run time through the ONE runtime symbol rt_gc_frame_maps() agrees entry for entry between the two media. A stored-pattern blob frame carries no cell and no map (6.2b) and is excluded from both tables by construction, so this gate prints the population it graded rather than reading zero divergences off an empty table."

cat > "$T/a.icn" <<'EOF'
procedure f(a, b)
   return a + b;
end
procedure g(n)
   local s, i;
   s := 0;
   every i := 1 to n do s := s + f(i, 2);
   return s;
end
procedure main()
   write(g(5));
end
EOF
cat > "$T/b.sno" <<'EOF'
        DEFINE('F(X)')                              :(F_END)
F       F = X + 1                                   :(RETURN)
F_END
        S = 'hello world'
        S 'hello' = 'goodbye'                       :F(DONE)
        OUTPUT = S
        OUTPUT = F(41)
DONE
END
EOF
cat > "$T/c.pl" <<'EOF'
app([], L, L).
app([H|T], L, [H|R]) :- app(T, L, R).
:- app([1,2],[3],X), write(X), nl.
EOF

read_m3(){ SCRIP_GC_MAPS_DUMP=1 timeout 120 "$SCRIP" "$1" </dev/null >/dev/null 2>"$2"; }
read_m4(){ local src="$1" out="$2" plant="${3:-0}"; local s="$T/x.s" x="$T/x.bin"; rm -f "$s" "$x"
    if [ "$plant" = 1 ]; then SCRIP_GC_MAPTAB_PLANT=1 timeout 120 "$SCRIP" --compile -o "$s" "$src" </dev/null >/dev/null 2>&1
    else timeout 120 "$SCRIP" --compile -o "$s" "$src" </dev/null >/dev/null 2>&1; fi
    [ -s "$s" ] || return 1
    gcc "$s" -o "$x" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread 2>/dev/null || return 1
    SCRIP_GC_MAPS_DUMP=1 timeout 120 "$x" </dev/null >/dev/null 2>"$out"; }
tab(){ grep '^\[GC-MAPTAB\] graph=' "$1" 2>/dev/null | LC_ALL=C sort; }

pop=""
for w in a.icn b.sno c.pl; do
    src="$T/$w"
    read_m3 "$src" "$T/$w.m3.raw"
    if ! read_m4 "$src" "$T/$w.m4.raw"; then echo "  present FAIL ($w: mode 4 did not compile, link or run -- the table could not be read back)"; RC=1; continue; fi
    tab "$T/$w.m3.raw" > "$T/$w.m3.tab"; tab "$T/$w.m4.raw" > "$T/$w.m4.tab"
    n3=$(wc -l < "$T/$w.m3.tab"); n4=$(wc -l < "$T/$w.m4.tab")
    if [ "$n3" -gt 0 ] && [ "$n4" -gt 0 ]; then pop="$pop $w:m3=$n3/m4=$n4"
    else echo "  present FAIL ($w: the installed table is EMPTY in a medium -- m3=$n3 m4=$n4 entries. A gate that compared two empty tables would report agreement while measuring nothing)"; RC=1; continue; fi
    d=$(diff "$T/$w.m3.tab" "$T/$w.m4.tab" | grep -c '^[<>]')
    if [ "$d" != 0 ]; then echo "  agree FAIL ($w: the two media disagree in $d line(s) -- the same graph carries different geometry or a different identity in mode 3 and mode 4):"; diff "$T/$w.m3.tab" "$T/$w.m4.tab" | grep '^[<>]' | head -6 | sed 's/^/      /'; RC=1; fi
    for m in m3 m4; do
        grep -q 'graph=main ' "$T/$w.$m.tab" || { echo "  identity FAIL ($w $m: no graph is named \"main\" -- the entry graph's two spellings (mode 3 \"pat_flat\", mode 4 \"main\") were a real divergence and this is its regression guard)"; RC=1; }
        if grep -q '\$2F' "$T/$w.$m.tab"; then echo "  identity FAIL ($w $m: an entry carries the assembler mangling tell \$2F -- the map's graph_name is DATA and must be the graph's true name in both media, not whichever spelling the assembler needed): $(grep -m1 '\$2F' "$T/$w.$m.tab")"; RC=1; fi
    done
done
[ "$RC" = 0 ] && echo "  present/agree/identity PASS (population graded --$pop -- read back through rt_gc_frame_maps() in both media, identical entry for entry, entry graph named main, no mangling in any name)"

if read_m4 "$T/a.icn" "$T/plant.raw" 1; then
    tab "$T/plant.raw" > "$T/plant.tab"
    dp=$(diff "$T/a.icn.m3.tab" "$T/plant.tab" 2>/dev/null | grep -c '^[<>]')
    if [ "$dp" -gt 0 ]; then echo "  planted PASS (SCRIP_GC_MAPTAB_PLANT=1 on the mode-4 compile alone moves $dp line(s) of the installed table -- the read-back has been SEEN to catch a cross-media divergence, it is not an inert seam)"
    else echo "  planted FAIL (a mode-4 table emitted with a perturbed frame_bytes still reads identical to mode 3 -- the read-back is not reading the installed table, so arms 1-3 are measuring nothing)"; RC=1; fi
else echo "  planted FAIL (the planted mode-4 witness did not build -- the arm could not measure)"; RC=1; fi

if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: the map table is present and installed in both media and the two readings agree entry for entry (examined 4 arms)"
else echo "GATE FAIL(1) [$G]: the compile-time map table diverges between the media, is empty, or the read-back stopped discriminating (examined 4 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
