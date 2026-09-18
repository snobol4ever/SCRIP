#!/usr/bin/env bash
# test_gate_gc_record_type_table_is_rooted_not_blanket_marked.sh -- THE ARIZONA gc2 WITNESS, BOTH MODES, BYTE-IDENTICAL TO ITS .std.
# ⛔ THE MEASURED DEFECT (cfo 2026-09-16, row icon-gc2-arizona-gc-witness-regressed-...): dat_alloc_fill (src/driver/driver_data.c)
# lazily allocates each record's DATBLK_t with rt_pinned_alloc and keeps the only durable pointer in the STATIC dat_types[] table,
# which no root walk visited. Under the pre-rung-1 blanket policy every HB_WS block was force-marked, so the table needed no root;
# after GC-5 rung 1 (24f1ec353) the block is marked only from an instance reached AS A DESCRIPTOR, and an instance reached as a raw
# frame pointer is interior-scanned as descriptors, so its type pointer marks nothing. The block was reclaimed and slid over
# (hardware watchpoint: two memcpy writes from gc_collect_ex line 700), and type() then dereferenced garbage: m3 SIGSEGV after 31
# of 256 lines, m4 rc=0 with 100 empty lines. SCRIP_GC_PIN_AGGREGATES=1 (rung 1's own control) restored both modes, which is the
# attribution. CURE: dat_gc_roots() roots every DatType's blk, its name, its field-name array and each field name, plus any heap
# default value, from the collector's root list. This gate ran RED on the pre-cure build (m3 rc=139, m4 DIFF) and GREEN on the cure.
# A missing witness or a stale binary REFUSES rc=2; nothing here passes vacuously.
# ⭐ BLOCKING AGAIN 2026-09-17 (cto, CEO-807/821): this gate rode the REPORTED ramp because m4 read bad 12 of 20 on
# origin. On the Icon-allocating-box-polls landing it reads byte-identical 20 of 20 in BOTH modes. The cure is the
# aligned start of the conservative sniff (gc_zeta_frame began at the address of a `char` local, 3 bytes off a word
# boundary, and so found zero arena-pointing words on the main stack: 0 unaligned vs 3, 7, 11, 12 aligned on the same
# four collections). If this gate goes intermittent again, the number to re-measure is that one, not the run count.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
bash scripts/util_require_fresh.sh >/dev/null 2>&1 || { echo "REFUSES rc=2: stale or missing ./scrip -- run make"; exit 2; }
P="$ROOT/../corpus/packages/icon/arizona_tests/general"
[ -f "$P/gc2.icn" ] && [ -f "$P/gc2.std" ] || { echo "REFUSES rc=2: $P/gc2.icn or gc2.std missing -- the witness is vendored, not synthesised"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
bad=0
N="${GC2_RUNS:-5}"
n3=0; r3=0
for i in $(seq 1 "$N"); do
    ( cd "$W" && timeout 120 "$ROOT/scrip" "$P/gc2.icn" < /dev/null > m3.out 2>&1 ); rr=$?
    if [ "$rr" -ne 0 ] || ! cmp -s "$W/m3.out" "$P/gc2.std"; then n3=$((n3+1)); r3=$rr; fi
done
if [ "$n3" -eq 0 ]; then echo "  m3 PASS gc2 byte-identical in $N of $N runs"; else echo "  m3 RED $n3 of $N runs (last rc=$r3) -- a record's type block was reclaimed or a holder was not relocated; THIS DEFECT IS INTERMITTENT, so a count is the only honest verdict"; bad=1; fi
( cd "$W" && timeout 120 "$ROOT/scrip" --compile -o gc2.s "$P/gc2.icn" < /dev/null 2>c.err && gcc gc2.s -L "$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o gc2 2>l.err ) || { echo "GATE REFUSE(2): gc2 does not build in mode 4 -- cannot measure"; exit 2; }
n4=0; r4=0
for i in $(seq 1 "$N"); do
    ( cd "$W" && timeout 120 ./gc2 < /dev/null > m4.out 2>&1 ); rr=$?
    if [ "$rr" -ne 0 ] || ! cmp -s "$W/m4.out" "$P/gc2.std"; then n4=$((n4+1)); r4=$rr; fi
done
if [ "$n4" -eq 0 ]; then echo "  m4 PASS gc2 byte-identical in $N of $N runs"; else echo "  m4 RED $n4 of $N runs (last rc=$r4) -- empty record fields, or a SIGSEGV, as the type or instance block slides and a holder is not relocated"; bad=1; fi
if [ "$bad" -ne 0 ]; then echo "GATE FAIL(1) [gc_record_type_table_is_rooted_not_blanket_marked]: gc2 is not byte-identical in both modes -- dat_gc_roots is missing from the collector's root list or does not cover every DatType"; exit 1; fi
echo "GATE PASS(0) [gc_record_type_table_is_rooted_not_blanket_marked]: gc2 byte-identical to its .std in EVERY one of $N runs per mode, both modes (2 arms x $N runs, 0 red)"
