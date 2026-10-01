#!/usr/bin/env bash
# test_gate_gc_one_stack_the_walker_sweeps_tagged_cells.sh -- THE DONE-WHEN of the cto's rank-0 row
# gc-one-stack-all-descriptors-no-marker-no-map-no-ledger-every-raw-word-on-the-emitted-stack-becomes-a-tagged-cell-lon-2026-09-30
# (Lon 2026-09-30, in-chat to the cto: no frame markers, no scanning to markers, no side-car stack; ARCH-GC-COMPILE-TIME-FRAME-MAPS.md
# section 12 ONE STACK, ALL DESCRIPTORS). The law it grades (section 7 F1, CLAUDE.md THE COLLECTOR GUESSES NOTHING): everything on the
# emitted stack is a DESCR and its type field is the only tag, so a collector can sweep the stack 16 bytes at a time from a 16-aligned
# floor and read every unit as a cell, with no map, no marker and no second stack.
# THE INSTRUMENT is gc_s16_census in gc_heap.c (RT_DIAG), switched by SCRIP_GC_SWEEP16: 1 prints one [GC-S16-SUM] line per segment per
# collection, 2..8 also print every raw unit of the first 8, 64, 512 ... collections with the frame it sits in (BLOB rbp-relative, FRAME
# and ROOT base-relative, SPINE as the distance below the next frame's map cell, TOP above the last), 9 every collection. A unit is RAW
# when its first word is a heap, code or stack address, or carries no known tag (other); a bare tag-known test is not enough because
# every tag is a multiple of 8 and so is most of a raw stack address's low byte.
# ARMS, per witness and per mode (env -i, setarch -R, 64 KB window, SCRIP_GC_STRESS=1; the main stack only -- a parked co-expression's
# stack above park_sp holds C frames, which section 12 (c) gives to the C2BB rows):
#   (a) THE INSTRUMENT RAN: every run printed main-stack census lines over a non-empty stack -- else REFUSE(2), a zero is not a reading
#   (b) IT CAN SAY YES: the same classifier over the GVA island (an array of DESCR cells the collector already visits) reads 0 raw
#   (c) THE CRITERION: the main stack reads 0 raw units at every collection of every witness in both modes -- RED until the batch lands
#   (d) NO MAP IS READ: the census finds 0 DT_MAP cells at every dumped collection -- RED until L3 deletes the maps and the scan
# REPORTED in make test (the leading -) until the row's last layer: a red here is the row's distance to done, printed by class and by
# the most frequent (frame, offset, kind) of the dumped units, so each layer's landing reads its own effect.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; SCRIP="$ROOT/scrip"; RT="$ROOT/out"; CORPUS="$(cd "$ROOT/../corpus" 2>/dev/null && pwd)"
NAME=test_gate_gc_one_stack_the_walker_sweeps_tagged_cells
refuse() { echo "⛔ GATE REFUSED(2) [$NAME]: $*"; exit 2; }
[ -x "$SCRIP" ] && [ -f "$RT/libscrip_rt.so" ] || refuse "no ./scrip or out/libscrip_rt.so -- build first"
[ -n "$CORPUS" ] || refuse "no corpus checkout beside SCRIP/"
grep -q 'gc_s16_census' "$ROOT/src/runtime/rt/gc_heap.c" || refuse "gc_heap.c carries no gc_s16_census -- the instrument this gate reads is gone"
SETARCH=""; command -v setarch >/dev/null 2>&1 && SETARCH="setarch -R"
WIT=("$HERE/gc_witnesses/hb_blob_span_defer.sno" "$HERE/gc_witnesses/hb_eval_names.sno"
     "$HERE/gc_witnesses/hb_sno_every_deferred_expression_shape_under_the_flip_plant.sno"
     "$CORPUS/benchmarks/prolog/bench/crypt.pl" "$HERE/gc_witnesses/hb_coexpr_create.icn"
     "$CORPUS/packages/icon/ipl/gprocs/vscroll_driver.icn")
for f in "${WIT[@]}"; do [ -f "$f" ] || refuse "witness missing: $f"; done
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT; trap 'rm -rf "$W"; exit 143' TERM; trap 'rm -rf "$W"; exit 130' INT
run() { env -i PATH=/usr/bin:/bin HOME="$W" SCRIP_HEAP_KB=64 SCRIP_GC_STRESS=1 SCRIP_GC_SWEEP16=2 $SETARCH timeout 120 "$@" </dev/null 2>"$W/err" >"$W/out"; }
nref=0; nyes=0; ncrit=0; nmap=0; refused=""; crit=""; maps=""; yes=""; : > "$W/units"
for f in "${WIT[@]}"; do b="$(basename "$f")"
  for m in m3 m4; do
    if [ "$m" = m3 ]; then (cd "$W" && run "$SCRIP" "$f")
    else (cd "$W" && rm -f x x.s && timeout 120 "$SCRIP" --compile -o x.s "$f" </dev/null >/dev/null 2>&1 && gcc -o x x.s -L"$RT" -lscrip_rt -lm -lstdc++ -Wl,-rpath,"$RT" >/dev/null 2>&1) \
           || { refused="$refused $b[$m:compile]"; continue; }; (cd "$W" && run ./x); fi
    r="$(awk '/^\[GC-S16-SUM\]/ { p=$2; for (i=3;i<=NF;i++) { split($i,a,"="); t[p" "a[1]]+=a[2] } if (p=="pop=cstack" && $NF!="frames=0") fm++ }
             END { printf "%d %d %d %d %d %d %d %d %d", t["pop=cstack units"], t["pop=cstack raw"], t["pop=cstack code"], t["pop=cstack stack"], t["pop=cstack heap"], t["pop=cstack other"], t["pop=gva units"], t["pop=gva raw"], fm }' "$W/err")"
    set -- $r; u=$1 raw=$2 code=$3 stk=$4 hp=$5 oth=$6 gu=$7 graw=$8 fmn=$9
    grep '^\[GC-S16\] pop=cstack' "$W/err" | awk '{print $3, $5, $6}' >> "$W/units"
    if [ "$u" -le 0 ]; then refused="$refused $b[$m:no-census]"; continue; fi
    nref=$((nref+1))
    if [ "$gu" -gt 0 ]; then nyes=$((nyes+1)); [ "$graw" -eq 0 ] || yes="$yes $b[$m]=$graw"; fi
    [ "$raw" -eq 0 ] || { ncrit=$((ncrit+1)); crit="$crit
      $b $m: $raw raw of $u units (code $code, stack $stk, heap $hp, other $oth)"; }
    [ "$fmn" -eq 0 ] || { nmap=$((nmap+1)); maps="$maps $b[$m]"; }
  done
done
fail=0
[ -z "$refused" ] || refuse "the census could not be read for:$refused"
[ "$nyes" -gt 0 ] || refuse "no witness put cells in the GVA island, so arm (b) has no population"
if [ -z "$yes" ]; then echo "  ok   (a) the census read the main stack of all $nref runs (${#WIT[@]} witnesses x m3 m4)"
                       echo "  ok   (b) IT CAN SAY YES: over the GVA island of $nyes run(s) the same classifier reads 0 raw units"
else echo "  FAIL (b) the classifier reads raw units in the GVA island, an array of DESCR cells:$yes -- fix the instrument before reading (c)"; fail=1; fi
if [ "$ncrit" -eq 0 ]; then echo "  ok   (c) THE CRITERION: 0 raw units on the main stack at every collection of every run"
else echo "  FAIL (c) THE CRITERION: $ncrit of $nref run(s) carry raw units on the main stack:$crit"
     echo "       the most frequent dumped units (where, offset, kind) over the first 8 collections of every run:"
     sort "$W/units" | uniq -c | sort -rn | head -8 | sed 's/^/        /'; fail=1; fi
if [ "$nmap" -eq 0 ]; then echo "  ok   (d) NO MAP IS READ: 0 DT_MAP cells found at every dumped collection"
else echo "  FAIL (d) NO MAP IS READ: $nmap of $nref run(s) still carry DT_MAP cells the walk scans for:$maps"; fail=1; fi
[ "$fail" = 0 ] && { echo "✅ GATE PASS(0) [$NAME]: every unit of the main stack is a tagged cell at every collection, and no map is read"; exit 0; }
echo "⛔ GATE RED(1) [$NAME]: the emitted stack still carries raw words or map cells (REPORTED until the row's last layer)"; exit 1
