#!/usr/bin/env bash
export S4E_ONE_RUNNER_FIXTURE="gate arm ${0##*/}: a runner invoked as an instrument fixture, not a board (CEO-523)"
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_gc_a_collection_over_a_small_live_set_stays_under_its_instruction_budget.sh -- THE FIXED COST OF A COLLECTION, PINNED.
# At the shipped 128 KB arena table_access spends 76% of its instructions in 2,482 collections of a ~712-block live set, and the
# cost of one such collection is mostly walks that do not depend on the live set: the root walks over the name and function
# tables, the defer-cell scan, the vacated-ground ledger, the plant seams read per block, the slot fixup's second lookup, the
# worklist traffic of integer descriptors (ceo 2026-09-25, CEO-1256). The CEO-1256 landing cut that from 851 K to about 650 K Ir
# per collection; this gate keeps it from creeping back. THE INSTRUMENT is callgrind (load-immune: instructions, not time) on the
# table_access iteration twin at 128 KB: gc_collect_ex inclusive Ir divided by the collection count the runtime reports.
# ARMS: (1) the twin computes its value; (2) collections >= 100 (population); (3) Ir per collection <= 760,000.
# FAIL_ONCE (recorded): the pre-cure runtime at 0f560a28e read 851 K per collection; this tree reads about 650 K.
# Cost ~8 s (callgrind over ten repetitions). EXIT 0 all arms; 1 a red; 2 REFUSED (valgrind, the kernel or the build missing).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
K="$S4E/corpus/benchmarks/snobol4/table_access.sno"; [ -f "$K" ] || { echo "REFUSED(2): $K missing"; exit 2; }
[ -x "$ROOT/scrip" ] && [ -f "$ROOT/out/libscrip_rt.so" ] || { echo "REFUSED(2): $ROOT/scrip or out/libscrip_rt.so not built"; exit 2; }
command -v valgrind >/dev/null 2>&1 && command -v callgrind_annotate >/dev/null 2>&1 || { echo "REFUSED(2): valgrind/callgrind_annotate missing -- the budget is an instruction count"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
python3 "$HERE/bench_wrap_snobol4.py" "$K" --mode iter --n 10 -o "$W/ta.sno" > /dev/null 2>&1 || { echo "REFUSED(2): twin generation failed"; exit 2; }
( cd "$W" && "$ROOT/scrip" --compile -o "$W/ta.s" ta.sno < /dev/null > /dev/null 2>&1 && gcc "$W/ta.s" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/ta.bin" ) 2>/dev/null || { echo "REFUSED(2): the twin did not build in mode 4"; exit 2; }
unset SCRIP_HEAP_MB; export SCRIP_HEAP_KB=128
( cd "$W" && SCRIP_GC_EXERCISE=1 timeout 300 valgrind --tool=callgrind --callgrind-out-file="$W/cg.out" ./ta.bin < /dev/null > "$W/ta.out" 2> "$W/ta.err" )
red=0
grep -q 'BENCH mode=iter kernel=TABLE_ACCESS iters=10 .*mismatched=0 value=250500' "$W/ta.err" && echo "ok  (1) the twin computed value=250500 over 10 repetitions" || { echo "RED (1) the twin did not print its value: $(grep -o 'BENCH.*' "$W/ta.err" | head -1)"; red=1; }
coll=$(grep -o 'collections=[0-9]*' "$W/ta.err" | tail -1 | cut -d= -f2); coll=${coll:-0}
[ "$coll" -ge 100 ] || { echo "REFUSED(2): only $coll collections at 128 KB -- too few to grade a per-collection budget"; exit 2; }
echo "ok  (2) collections=$coll (population)"
ir=$(callgrind_annotate --inclusive=yes "$W/cg.out" 2>/dev/null | grep 'gc_heap.c:gc_collect_ex' | head -1 | awk '{gsub(",","",$1); print $1}')
[ -n "$ir" ] || { echo "REFUSED(2): callgrind_annotate reported no gc_collect_ex line"; exit 2; }
per=$((ir / coll))
if [ "$per" -le 760000 ]; then echo "ok  (3) gc_collect_ex $ir Ir over $coll collections = $per Ir per collection <= 760000"
else echo "RED (3) gc_collect_ex $ir Ir over $coll collections = $per Ir per collection > 760000: a fixed cost crept back into the collection"; red=1; fi
[ "$red" -eq 0 ] && { echo "GATE OK: a collection over table_access's live set at 128 KB stays under its instruction budget"; exit 0; }
echo "GATE FAILED: see the RED lines above"; exit 1
