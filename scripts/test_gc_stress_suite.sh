#!/usr/bin/env bash
# scripts/test_gc_stress_suite.sh — the 3-stage GC (MARK/ADJUST/SLIDE) torture matrix.
# Runs corpus/crosscheck/gc/ (one test per collector root family + the exhaustion tortures)
# through BOTH modes at a stress ladder: plain, SCRIP_GC_STRESS=25, =7, =1 (collect every alloc).
# Every cell is oracle-diffed against the SPITBOL-generated .ref.
#
# WHAT plain DOES NOT COVER at the default 512MB heap: natural EXHAUSTION-triggered collection
# (gc_collect_ex(1), the conservative-stack-scan path at the allocation site). To exercise it,
# rebuild with a tiny heap and re-run this script:
#     cd SCRIP && rm -f scrip out/libscrip_rt.so src/runtime/rt/gc_heap.o
#     make -j4 scrip ZCFLAGS="-DZC_HEAP_MB=2" && make libscrip_rt ZCFLAGS="-DZC_HEAP_MB=2"
#     bash scripts/test_gc_stress_suite.sh          # 213/214 then force ~30 regenerations each
# (Remember to rebuild default afterward — ZCFLAGS is not a make prerequisite.)
#
# Gate: every cell PASS. Exit 1 on any FAIL.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="${CORPUS:-$S4E/corpus}"
GCDIR="$CORPUS/crosscheck/gc"
TIMEOUT="${TIMEOUT:-60}"
if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built"; exit 0; fi
if [ ! -d "$GCDIR" ]; then echo "SKIP no gc corpus at $GCDIR"; exit 0; fi
WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT
echo "=== GC stress suite — corpus/crosscheck/gc x {plain,S25,S7,S1} x {m3,m4} ==="
TOTAL_FAIL=0
for sno in "$GCDIR"/*.sno; do
    b=$(basename "${sno%.sno}")
    "$SCRIP" --compile "$sno" > "$WORKDIR/$b.s" 2>/dev/null \
      && gcc -c "$WORKDIR/$b.s" -o "$WORKDIR/$b.o" 2>/dev/null \
      && gcc "$WORKDIR/$b.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$WORKDIR/$b" 2>/dev/null \
      || echo "COMPILE-FAIL $b (mode-4 cells will FAIL)"
done
for S in 0 25 7 1; do
    [ "$S" = 0 ] && lbl="plain   " || lbl="STRESS=$S"
    for MODE in m3 m4; do
        P=0; F=0; FL=""
        for sno in "$GCDIR"/*.sno; do
            b=$(basename "${sno%.sno}")
            ref="${sno%.sno}.ref"
            [ -f "$ref" ] || continue
            if [ "$MODE" = m3 ]; then
                if [ "$S" = 0 ]; then out=$(timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null); else out=$(SCRIP_GC_STRESS=$S timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null); fi
            else
                [ -x "$WORKDIR/$b" ] || { F=$((F+1)); FL="$FL $b(nc)"; continue; }
                if [ "$S" = 0 ]; then out=$(timeout "$TIMEOUT" "$WORKDIR/$b" < /dev/null 2>/dev/null); else out=$(SCRIP_GC_STRESS=$S timeout "$TIMEOUT" "$WORKDIR/$b" < /dev/null 2>/dev/null); fi
            fi
            if [ "$out" = "$(cat "$ref")" ]; then P=$((P+1)); else F=$((F+1)); FL="$FL $b"; fi
        done
        echo "  $lbl $MODE: PASS=$P FAIL=$F$FL"
        TOTAL_FAIL=$((TOTAL_FAIL + F))
    done
done
if [ "$TOTAL_FAIL" = 0 ]; then echo "GC STRESS SUITE: ALL GREEN"; exit 0; else echo "GC STRESS SUITE: $TOTAL_FAIL FAILING CELLS"; exit 1; fi
