#!/bin/bash
# test_gate_nv_set_fn_ir_ratchet.sh -- the DONE-WHEN of row perf-nv-set-fn-o0-overhead (item 3).
#
# ⭐ WHAT ITEM 3 IS: scope + land a cure for NV_SET_fn's fast-path-hit cost, or rule it explicitly not worth doing. Items 1 and 2 are CLOSED (item 1 by seat07, 100% of 50,492,413 Ir attributed;
# item 2 by seat01, the memo cache is a decisive -O0 win). This gate ratchets NV_SET_fn's Ir on roman so "a cure landed" becomes a measurement instead of a claim.
#
# ⛔⛔ READ THIS BEFORE TRUSTING THE NUMBER -- THE RECIPE GAP IS THIS ROW'S FIRST TASK, NOT A DETAIL (hq_P 2026-09-02, measured). The row's own figures (roman kernel 353,472,366 Ir; item 3's
# 13,403,067 Ir fast-path-hit line) come from a SCALED kernel, and THAT RECIPE IS NOT RECORDED ON THE BATON. The committed corpus/benchmarks/snobol4/roman.sno runs K = 1001..1010 -- TEN iterations
# -- so a direct callgrind run of it totals ~35.6M Ir and is COMPILE-DOMINATED (__strcmp_avx2 12.65%, bb_emit_x86 8.80%, zd_plan 4.28%): it profiles the COMPILER, not the roman kernel. NV_SET_fn
# reads ~122K Ir there, three orders below the 13.4M item 3 targets.
# ⭐ SO THE PIN BELOW IS AN HONEST BASELINE OF A DIFFERENT QUANTITY THAN THE ROW'S HEADLINE FIGURE, AND IT SAYS SO. It is a real, reproducible ratchet -- a cure to NV_SET_fn's fast path must still
# move it -- but it is NOT the 13.4M line. ⛔ THE TAKER'S FIRST STEP IS TO RECOVER THE SCALED RECIPE (the *BENCH directive at roman.sno:24 -- kernel=ROMAN_RUN check=200 bud=500 flr=20 -- is the
# thread to pull), RE-PIN AGAINST IT, and record the recipe ON THE BATON so the next seat does not re-derive it. Then this gate grades the quantity the row actually names.
# ⛔ Pinning a number whose recipe is unrecorded is how a row becomes uncloseable by a later seat -- which is exactly the defect class the live-batons row was minted for. Do not repeat it here.
set -u
R="${S4E_HOME:-/home/claude_P}"
PIN=122092          # src/runtime/core/core.c:NV_SET_fn, measured hq_P 2026-09-02, SCRIP fa12d7cb, -O0, direct callgrind of the COMMITTED roman.sno (10 iterations). See the recipe-gap note above.
cd "$R/SCRIP" 2>/dev/null || { echo "⛔ REFUSED (rc=2): no $R/SCRIP"; exit 2; }
[ -x ./scrip ] || { echo "⛔ REFUSED (rc=2): ./scrip is not built -- cannot profile what does not exist"; exit 2; }
SNO="$R/corpus/benchmarks/snobol4/roman.sno"
[ -f "$SNO" ] || { echo "⛔ REFUSED (rc=2): $SNO missing -- the benchmark moved; re-point this gate rather than lowering the pin"; exit 2; }
command -v valgrind >/dev/null && command -v callgrind_annotate >/dev/null || { echo "⛔ REFUSED (rc=2): valgrind/callgrind_annotate not installed -- this gate measures Ir and must not pass on a box that cannot"; exit 2; }
O="$(mktemp -d)"; trap 'rm -rf "$O"' EXIT
timeout 600s valgrind --tool=callgrind --callgrind-out-file="$O/cg.out" ./scrip "$SNO" < /dev/null > "$O/run.log" 2>&1 || { echo "⛔ REFUSED (rc=2): the callgrind run itself failed -- see that before reading any number"; exit 2; }
IR="$(callgrind_annotate "$O/cg.out" 2>/dev/null | grep -F 'core.c:NV_SET_fn' | head -1 | tr -d ' ,' | grep -oE '^[0-9]+')"
case "$IR" in ''|*[!0-9]*) echo "⛔ REFUSED (rc=2): could not read a core.c:NV_SET_fn Ir figure out of callgrind_annotate -- the symbol was inlined away, renamed, or the annotation shape changed. That is NOT a pass; find out which."; exit 2 ;; esac
echo "NV_SET_fn (src/runtime/core/core.c) Ir on roman: $IR   pinned baseline: $PIN"
if [ "$IR" -lt "$PIN" ]; then
    echo "PASS: NV_SET_fn Ir dropped $((PIN - IR)) below the pin -- a cure moved it. Re-pin PIN=$IR in this script IN THE SAME COMMIT as the cure, and cite the FINDING."
    exit 0
fi
echo "GATE FAIL(1) [nv_set_fn_ir_ratchet]: NV_SET_fn Ir is $IR, not below the pinned $PIN -- item 3's cure is not landed (or is not measurable at this scale; see the recipe-gap note in this file's header)"
exit 1
