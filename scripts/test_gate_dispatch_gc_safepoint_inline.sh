#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_dispatch_gc_safepoint_inline.sh -- perf-dispatch-gc-safepoint-necessity DONE-WHEN.
#
# CLAIM UNDER TEST: rt_call_arr_impl (src/runtime/by_name_dispatch.c) inlines a check of g_gc_pending
# BEFORE paying for the rt_gc_point_arr veneer's unconditional 6-register push/pop + @plt call, calling
# the real (unchanged) veneer only when a collection is actually due -- same precedent-shape already
# landed at rtx_plunify.s's rt_pl_dop_unify ("absorbed rt_gc_point_arr" comment). Killswitch
# SCRIP_DISPATCH_GC_INLINE=0 restores the old unconditional call on the same binary.
#
# This gate proves the claim two ways, both COMPUTED, neither asserted:
#   1. CORRECTNESS: cure-on and killswitch-off produce byte-identical program output (check value) on
#      a builtin-heavy kernel -- the cure changes nothing observable when no collection occurs, and the
#      cold (collecting) path is textually unchanged from before, so there is nothing else to diverge.
#   2. PERFORMANCE: cure-on executes STRICTLY FEWER instructions (callgrind Ir, fixed-work N=20000,
#      RT_OPT=-O0) than killswitch-off on the same kernel -- the win the row exists to prove is real.
#
# Full-corpus correctness (362/362 SNOBOL4 both modes, 14/14 Icon, Prolog at its pre-existing 3/5
# baseline) and a 1MB-heap GC-stress A/B (cure-on's failure set is a STRICT SUBSET of killswitch-off's,
# i.e. no new failure under artificial heap pressure) were run once and recorded in this row's LEDGER +
# the accompanying FINDING -- not re-run here on every invocation, to keep this gate fast and repeatable.
#
# Usage: bash scripts/test_gate_dispatch_gc_safepoint_inline.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

gate_require_exec "$ROOT/scrip" "the scrip binary"
gate_require_exec "$ROOT/out/libscrip_rt.so" "libscrip_rt.so"
gate_require "$ROOT/src/runtime/by_name_dispatch.c" "by_name_dispatch.c"

VIOL=0

# --- source marker: the cure must still be present, not silently reverted ---
if ! grep -q "SCRIP_DISPATCH_GC_INLINE" "$ROOT/src/runtime/by_name_dispatch.c"; then
    echo "GATE FAIL: SCRIP_DISPATCH_GC_INLINE killswitch/cure marker not found in by_name_dispatch.c -- reverted?"
    VIOL=$((VIOL+1))
fi

# --- build the fixed-work harness (N=20000, matches this row's LEDGER methodology) ---
CORPUS="$ROOT/../corpus"
gate_require "$CORPUS/benchmarks/snobol4/string_manip.sno" "string_manip.sno"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
bash "$HERE/bench_wrap.sh" "$CORPUS/benchmarks/snobol4/string_manip.sno" -o "$WORK/sm.sno" --mode=iter --n=20000 >/dev/null 2>&1
"$ROOT/scrip" --compile -o "$WORK/sm.s" "$WORK/sm.sno" < /dev/null >/dev/null 2>&1
gcc -no-pie "$WORK/sm.s" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -o "$WORK/sm.bin" 2>/dev/null
gate_require_exec "$WORK/sm.bin" "the compiled string_manip harness"

command -v valgrind >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: valgrind not available, cannot measure Ir"; exit 2; }

valgrind --tool=callgrind --callgrind-out-file="$WORK/on.cg" "$WORK/sm.bin" < /dev/null > "$WORK/on.out" 2>"$WORK/on.vgerr"
SCRIP_DISPATCH_GC_INLINE=0 valgrind --tool=callgrind --callgrind-out-file="$WORK/off.cg" "$WORK/sm.bin" < /dev/null > "$WORK/off.out" 2>"$WORK/off.vgerr"

ON_IR="$(grep -m1 '^==.*Collected' "$WORK/on.vgerr" | grep -oE '[0-9]+$')"
OFF_IR="$(grep -m1 '^==.*Collected' "$WORK/off.vgerr" | grep -oE '[0-9]+$')"

if [ -z "${ON_IR:-}" ] || [ -z "${OFF_IR:-}" ]; then
    echo "GATE UNPROVEN(2) [$GATE_NAME]: could not parse Ir counts from callgrind output"
    exit 2
fi

# --- correctness: identical observable output (ignore nothing -- this harness prints no timing noise on stdout other than ns/ms, which we exclude deliberately since wall-clock is expected to vary run to run and is not part of the correctness claim) ---
if ! diff -q <(grep -v '^ns:\|^ms:' "$WORK/on.out") <(grep -v '^ns:\|^ms:' "$WORK/off.out") >/dev/null 2>&1; then
    echo "GATE FAIL: cure-on and killswitch-off produced DIFFERENT correctness-relevant output -- the cure changed behavior beyond timing"
    diff <(grep -v '^ns:\|^ms:' "$WORK/on.out") <(grep -v '^ns:\|^ms:' "$WORK/off.out")
    VIOL=$((VIOL+1))
fi

# --- performance: cure must execute strictly fewer instructions ---
if [ "$ON_IR" -ge "$OFF_IR" ]; then
    echo "GATE FAIL: cure-on Ir ($ON_IR) is NOT fewer than killswitch-off Ir ($OFF_IR) -- claimed win not reproduced"
    VIOL=$((VIOL+1))
else
    SAVED=$((OFF_IR - ON_IR))
    PCT=$(awk "BEGIN{printf \"%.2f\", $SAVED/$OFF_IR*100}")
    echo "measured: cure-on=$ON_IR Ir, killswitch-off=$OFF_IR Ir, saved=$SAVED Ir (${PCT}% of baseline)"
fi

gate_floor 1 1 "kernel measured"
gate_verdict "$VIOL" "violation(s)"
