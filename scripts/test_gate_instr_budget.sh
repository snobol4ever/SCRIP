#!/usr/bin/env bash
# test_gate_instr_budget.sh -- INSTRUCTION-COUNT REGRESSION GATE, queue row `instr-budget-gate` (rank 3, PERF INFRASTRUCTURE).
#
# WHY THIS GATE EXISTS (HQ s251, ARCH-PERF-TOOLING.md §5).  Wall clock on this Zen 4 mobile part is noisy
# (beauty measured +-8% run to run even with governor=performance) but callgrind's Ir total is EXACT and
# reproducible to the instruction.  Pinning Ir for two workloads with a tolerance band turns a silent 5%
# instruction-count regression into an immediate gate failure instead of something found three weeks later
# under other drift.  s249 named the idiom "arith loop instruction budget" -- this gate generalises it.
#
# WORKLOADS.  (1) roman -- corpus/demo/roman.sno, fully self-contained (no external input
# file) -- DEFINE/recursion/REPLACE over ~345 roman-numeral conversions, chosen because it is RUNTIME-C-
# dominated (pattern REPLACE + string building through libscrip_rt.so), the counterpart of the compiler-
# emitted-dominated arith_loop s249 already budgets.  ⛔ porter.sno was the original choice but CANNOT BE
# BUILT in m4 at all -- duplicate assembler symbol .Lx865_40, a label-minting collision already filed as
# queue row `porter-m4-duplicate-label` (rank 1; not this row's problem, and NOT a reason to hand-encode
# around the emitter).  treebank-match / claws5-match were also tried and rejected: their "matched bytes=N"
# .ref values are stale false-green oracles from the TT_VLIST-not-lowered defect (queue row
# `vlist-expr-alternation`) -- today's CORRECT output legitimately diverges from those pinned .ref files, which
# would make this gate's correctness pre-check permanently red for the wrong reason.  Reconsider the workload
# once either queued row lands (porter is the better fit long-term per ARCH-PERF-TOOLING.md's own "table path
# is 91%" framing). (2) beauty self-host -- the MILESTONE-1 fixed point, beauty.sno < beauty.sno.  Both are
# gated on CORRECTNESS FIRST (roman vs its .ref, beauty vs the fixed-point identity) so a silently truncated or
# early-erroring run can never masquerade as "instructions went down" (ARCH-PERF-TOOLING.md §7: "optimizing
# against a false measurement is worse than not optimizing" applies equally to a false-green instruction count
# here).
#
# TOLERANCE.  A one-sided budget with a +-TOL_PCT% band: FAIL only above watermark*(1+TOL_PCT/100), which is
# the actual regression this gate exists to catch.  Below watermark*(1-TOL_PCT/100) is reported, not failed --
# failing on an instruction-count IMPROVEMENT would fight the entire ARCH-PERF-TOOLING campaign this row serves.
#
# RT_OPT.  Pinned watermarks below are RT_OPT=-O0 (O0-DEV-O2-BENCH: the default for all normal development),
# measured 2026-08-22 on a `make pristine` build (HQ-27; PRISTINE_RC=0, scrip + out/libscrip_rt.so both fresh,
# rebuilt AFTER rebasing onto SCRIP `cd13321e` -- the prior measurement at `2659558e` was re-proven stale by
# that rebase per RULES.md "re-prove your gate after a rebase" and re-measured, not assumed: roman +0.56%
# (concurrent template churn), beauty -4.49% (a real win from the concurrent free-r10/free-r11 campaign).
# This gate measures whichever out/libscrip_rt.so is already built; it does not itself rebuild the runtime.
#
# TABLE_ACCESS / ARRAY_SUM (added 2026-08-24, seat04, row `perf-table-array-runtime` NEXT step -- see
# FINDING-2026-08-24-seat04-post-fix-table-array-callgrind-remeasurement.md).  Both are run STANDALONE
# (no bench_wrap.sh fixed-iter twin, no fixed_n) exactly like roman/beauty above: table_access.sno's own
# MAIN calls TABLE_ACCESS(1) then TABLE_ACCESS(20) and array_sum.sno's calls ARRAY_SUM(1) then (20) --
# 21 total outer builds each, small and fast, each with a real .ref from the s265 standalone revamp.
# ⛔ THIS IS NOT THE SAME MEASUREMENT AS THE FINDING'S SPITBOL-RATIO NUMBERS.  That FINDING used
# bench_wrap.sh --mode=iter at N=2,000/100/8,192 to get a large, statistically stable Ir/iter figure
# comparable against SPITBOL.  This gate has a different job -- catch a REGRESSION against SCRIP's OWN
# prior instruction count, same shape as roman/beauty above -- so it uses the smaller un-wrapped
# standalone run, same as they do, and never compares across engines.  Watermarks below are RT_OPT=-O0,
# `make pristine` at SCRIP `eca52780`, mode-4, plain `valgrind --tool=callgrind` (no --smc-check flag,
# matching this gate's own measure_ir, not profile_callgrind.sh's pattern-blob flag).
#
# PROVING THE FAIL PATH (recorded once, not re-run every invocation -- 2026-08-22, this pristine build).
# ROMAN_IR_WATERMARK=1000000 bash scripts/test_gate_instr_budget.sh negative-tests the budget arithmetic by
# injection (the established idiom for this codebase's gates -- see e.g. test_gate_argnote_sweep.sh's
# off-limits-count injection): roman still matches its .ref (correctness untouched), beauty stays within its
# real budget, and the script correctly reports "FAIL roman: Ir=... > budget 1000000 ... REGRESSION" and exits
# 1 -- proving check_budget's comparison, not faking the exit code.
# ⛔ THREE ORGANIC LEVERS WERE TRIED FIRST AND ALL THREE BROKE SOMETHING ELSE INSTEAD OF CLEANLY INFLATING Ir,
# each a real, separate, PRE-EXISTING instability outside this row's scope (flagged to HQ, not fixed here):
# SCRIP_OPT=0 -- undefined `*_define_β`/`*_goto_deferred_β` symbols at LINK time on BOTH roman and beauty (the
# optimizer is apparently load-bearing for DEFINE-related label resolution, not just a speed lever);
# --zeta-storage=cell-heap -- roman SIGSEGVs at runtime; --zeta-storage=frame-rsp -- beauty aborts
# ("IR_MATCH_CAPTURE_SAVE: no home ... classifier and ZD plan disagree"). Only the compiled default
# (optimizer on, cell-stack) is solid across both workloads today.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
set -u
SCRIP_BIN="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS_ROOT="${CORPUS:-$S4E/corpus}"
DEMO="$CORPUS_ROOT/demo"
BEAUTY_DIR="$DEMO/beauty"
BENCH="$CORPUS_ROOT/benchmarks/snobol4"

TOL_PCT="${TOL_PCT:-2}"
# Watermarks: RT_OPT=-O0, measured on a `make pristine` build, 2026-08-22, SCRIP `cd13321e`.  Re-pin with the FINDING that changed them.
ROMAN_IR_WATERMARK="${ROMAN_IR_WATERMARK:-22522863}"
BEAUTY_IR_WATERMARK="${BEAUTY_IR_WATERMARK:-2215545392}"
# Watermarks: RT_OPT=-O0, `make pristine`, SCRIP `eca52780`, 2026-08-24 (seat04).  Re-pin with the FINDING that changed them.
TABLE_ACCESS_IR_WATERMARK="${TABLE_ACCESS_IR_WATERMARK:-15267937}"
ARRAY_SUM_IR_WATERMARK="${ARRAY_SUM_IR_WATERMARK:-10912565}"

[ -x "$SCRIP_BIN" ] || { echo "GATE FAIL(2): scrip not built at $SCRIP_BIN"; exit 2; }
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "GATE FAIL(2): libscrip_rt.so not built at $RT_DIR"; exit 2; }
command -v valgrind >/dev/null 2>&1 || { echo "GATE FAIL(2): valgrind not installed"; exit 2; }
[ "$ROMAN_IR_WATERMARK" -gt 0 ] && [ "$BEAUTY_IR_WATERMARK" -gt 0 ] && [ "$TABLE_ACCESS_IR_WATERMARK" -gt 0 ] && [ "$ARRAY_SUM_IR_WATERMARK" -gt 0 ] || { echo "GATE FAIL(2): watermark(s) not pinned yet"; exit 2; }

WORK=$(mktemp -d); trap 'rm -rf "$WORK"' EXIT
FAIL=0

measure_ir() {  # name sno input cwd_or_empty  -> sets IR_TOTAL, RUN_OUT on success (rc 0)
    local name="$1" sno="$2" input="$3" cwd="$4"
    local s="$WORK/$name.s" bin="$WORK/$name.bin" cg="$WORK/$name.cg.out"
    if [ -n "$cwd" ]; then ( cd "$cwd" && timeout 120 "$SCRIP_BIN" --compile -o "$s" "$sno" ) >"$WORK/$name.cc.err" 2>&1
    else timeout 120 "$SCRIP_BIN" --compile -o "$s" "$sno" >"$WORK/$name.cc.err" 2>&1; fi
    if [ ! -s "$s" ]; then echo "GATE FAIL(2): $name mode-4 compile produced nothing"; sed 's/^/    /' "$WORK/$name.cc.err"; return 1; fi
    gcc -no-pie "$s" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" -lm -o "$bin" 2>"$WORK/$name.ld.err"
    if [ ! -x "$bin" ]; then echo "GATE FAIL(2): $name mode-4 link failed"; sed 's/^/    /' "$WORK/$name.ld.err"; return 1; fi
    valgrind --tool=callgrind --callgrind-out-file="$cg" "$bin" < "$input" > "$WORK/$name.out" 2>"$WORK/$name.vg.err"
    IR_TOTAL=$(callgrind_annotate "$cg" 2>/dev/null | awk '/PROGRAM TOTALS/{gsub(/,/,"",$1); print $1}')
    RUN_OUT="$WORK/$name.out"
    [ -n "$IR_TOTAL" ] && [ "$IR_TOTAL" -gt 0 ] 2>/dev/null
}

check_budget() {  # label ir watermark
    local label="$1" ir="$2" wm="$3" hi lo
    hi=$(( wm * (100 + TOL_PCT) / 100 )); lo=$(( wm * (100 - TOL_PCT) / 100 ))
    if [ "$ir" -gt "$hi" ]; then
        echo "FAIL $label: Ir=$ir > budget $wm (+${TOL_PCT}% = $hi) -- REGRESSION"; FAIL=1
    elif [ "$ir" -lt "$lo" ]; then
        echo "NOTE $label: Ir=$ir < budget $wm (-${TOL_PCT}% = $lo) -- improved; consider re-pinning down"
    else
        echo "OK   $label: Ir=$ir within budget $wm +-${TOL_PCT}% [$lo,$hi]"
    fi
}

echo "=== instr-budget-gate: RT_OPT=-O0 pinned watermarks, TOL_PCT=${TOL_PCT}% ==="

if measure_ir roman "$DEMO/roman.sno" /dev/null ""; then
    roman_ir="$IR_TOTAL"
    if diff -q "$RUN_OUT" "$DEMO/roman.ref" >/dev/null 2>&1; then
        echo "OK   roman: output matches roman.ref"
    else
        echo "FAIL roman: output DIFFERS from roman.ref -- Ir count below is not trustworthy"; FAIL=1
    fi
    check_budget roman "$roman_ir" "$ROMAN_IR_WATERMARK"
else
    echo "GATE FAIL(2): roman measurement did not produce an Ir count"; exit 2
fi

if measure_ir beauty "$BEAUTY_DIR/beauty.sno" "$BEAUTY_DIR/beauty.sno" "$BEAUTY_DIR"; then
    beauty_ir="$IR_TOTAL"
    if cmp -s "$RUN_OUT" "$BEAUTY_DIR/beauty.sno"; then
        echo "OK   beauty: output is the FIXED POINT (byte-identical to beauty.sno)"
    else
        echo "FAIL beauty: output is NOT the fixed point -- Ir count below is not trustworthy"; FAIL=1
    fi
    check_budget beauty "$beauty_ir" "$BEAUTY_IR_WATERMARK"
else
    echo "GATE FAIL(2): beauty measurement did not produce an Ir count"; exit 2
fi

if measure_ir table_access "$BENCH/table_access.sno" /dev/null ""; then
    table_access_ir="$IR_TOTAL"
    if diff -q "$RUN_OUT" "$BENCH/table_access.ref" >/dev/null 2>&1; then
        echo "OK   table_access: output matches table_access.ref"
    else
        echo "FAIL table_access: output DIFFERS from table_access.ref -- Ir count below is not trustworthy"; FAIL=1
    fi
    check_budget table_access "$table_access_ir" "$TABLE_ACCESS_IR_WATERMARK"
else
    echo "GATE FAIL(2): table_access measurement did not produce an Ir count"; exit 2
fi

if measure_ir array_sum "$BENCH/array_sum.sno" /dev/null ""; then
    array_sum_ir="$IR_TOTAL"
    if diff -q "$RUN_OUT" "$BENCH/array_sum.ref" >/dev/null 2>&1; then
        echo "OK   array_sum: output matches array_sum.ref"
    else
        echo "FAIL array_sum: output DIFFERS from array_sum.ref -- Ir count below is not trustworthy"; FAIL=1
    fi
    check_budget array_sum "$array_sum_ir" "$ARRAY_SUM_IR_WATERMARK"
else
    echo "GATE FAIL(2): array_sum measurement did not produce an Ir count"; exit 2
fi

if [ "$FAIL" -ne 0 ]; then
    echo "GATE FAIL: instruction budget exceeded or a correctness check failed."
    exit 1
fi
echo "GATE OK: all four workloads within their pinned instruction budget."
exit 0
