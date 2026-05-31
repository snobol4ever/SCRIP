#!/usr/bin/env bash
# test_bench_snobol4_modes.sh — SBL-BENCH: run every SNOBOL4 benchmark under
# Mode 2 (--interp, SM dispatch), Mode 3 (--run, SB-LINEAR linear x86), and
# Mode 4 (--compile --target=x86 -> assemble -> link -> standalone ELF).
#
# Two outputs:
#   (1) CORRECTNESS GATE — non-timing stdout must be identical across the modes
#       that successfully run. Timing lines ("ms: ...") and the --bench stderr
#       line are stripped before comparison. A mode that errors/segv/times-out
#       is recorded as a per-mode FAIL but does not abort the run.
#   (2) TIMING TABLE — per-mode exec wall-clock (median feel: single run with
#       timeout; benches are deterministic-size loops).
#
# Self-contained per RULES.md: paths from $0, timeout on every scrip call,
# corpus path /home/claude/corpus (SKIP if missing).
# AUTHORS: Lon Jones Cherryholmes · Claude Opus   DATE: 2026-05-26
set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
RT_DIR="${RT_DIR:-$ROOT/out}"
CORPUS="${CORPUS:-/home/claude/corpus}"
BENCH_DIR="$CORPUS/benchmarks"
TIMEOUT="${TIMEOUT:-30}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$BENCH_DIR" ]; then echo "SKIP bench corpus not found at $BENCH_DIR"; exit 0; fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# Strip non-deterministic lines (timing + bench diagnostics) for equivalence.
norm() { grep -vE '^ms:|^BENCH |iterations:|^elapsed' 2>/dev/null || true; }

# Mode 4 build: --compile --target=x86 -> gcc -c -> link libscrip_rt.
compile_mode4() {
    local sno="$1" out="$2"
    SNO_LIB="$BENCH_DIR" timeout "$TIMEOUT" "$SCRIP" --compile --target=x86 "$sno" \
        > "$WORK/m4.s" 2>/dev/null || return 1
    (cd "$ROOT" && gcc -c "$WORK/m4.s" -o "$WORK/m4.o" 2>/dev/null) || return 1
    [ -f "$RT_DIR/libscrip_rt.so" ] || return 1
    gcc "$WORK/m4.o" -L"$RT_DIR" -lscrip_rt -lgc -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || return 1
    return 0
}

# Returns exec-ms on stdout (or "-" on failure); writes normalized output to $3.
run_mode2() {
    local sno="$1" outfile="$2"
    local t0 t1
    t0=$(date +%s.%N)
    SNO_LIB="$BENCH_DIR" timeout "$TIMEOUT" "$SCRIP" --interp "$sno" </dev/null 2>/dev/null \
        | norm > "$outfile"
    local rc=${PIPESTATUS[0]}
    t1=$(date +%s.%N)
    [ "$rc" -ne 0 ] && { echo "-"; return 1; }
    awk "BEGIN{printf \"%.0f\", ($t1-$t0)*1000}"
}
run_mode3() {
    local sno="$1" outfile="$2"
    local t0 t1
    t0=$(date +%s.%N)
    SNO_LIB="$BENCH_DIR" timeout "$TIMEOUT" "$SCRIP" --run "$sno" </dev/null 2>/dev/null \
        | norm > "$outfile"
    local rc=${PIPESTATUS[0]}
    t1=$(date +%s.%N)
    [ "$rc" -ne 0 ] && { echo "-"; return 1; }
    awk "BEGIN{printf \"%.0f\", ($t1-$t0)*1000}"
}
run_mode4() {
    local sno="$1" outfile="$2"
    local bin="$WORK/m4.bin" t0 t1
    compile_mode4 "$sno" "$bin" || { echo "-"; return 1; }
    t0=$(date +%s.%N)
    SNO_LIB="$BENCH_DIR" timeout "$TIMEOUT" "$bin" </dev/null 2>/dev/null \
        | norm > "$outfile"
    local rc=${PIPESTATUS[0]}
    t1=$(date +%s.%N)
    [ "$rc" -ne 0 ] && { echo "-"; return 1; }
    awk "BEGIN{printf \"%.0f\", ($t1-$t0)*1000}"
}

echo "=== SNOBOL4 benchmark — 3-mode parity + timing ==="
printf "%-26s %8s %8s %8s   %s\n" "bench" "m2(ms)" "m3(ms)" "m4(ms)" "equiv"
echo "-------------------------------------------------------------------------"

EQ_PASS=0; EQ_FAIL=0
M2_OK=0; M3_OK=0; M4_OK=0
TOTAL=0

for sno in "$BENCH_DIR"/*.sno; do
    [ -f "$sno" ] || continue
    base="$(basename "$sno" .sno)"
    TOTAL=$((TOTAL+1))

    o2="$WORK/$base.m2"; o3="$WORK/$base.m3"; o4="$WORK/$base.m4"
    : > "$o2"; : > "$o3"; : > "$o4"

    ms2=$(run_mode2 "$sno" "$o2"); r2=$?
    ms3=$(run_mode3 "$sno" "$o3"); r3=$?
    ms4=$(run_mode4 "$sno" "$o4"); r4=$?

    [ "$r2" -eq 0 ] && M2_OK=$((M2_OK+1))
    [ "$r3" -eq 0 ] && M3_OK=$((M3_OK+1))
    [ "$r4" -eq 0 ] && M4_OK=$((M4_OK+1))

    # Equivalence: among modes that ran, all normalized outputs must match.
    eq="n/a"
    ran=()
    [ "$r2" -eq 0 ] && ran+=("$o2")
    [ "$r3" -eq 0 ] && ran+=("$o3")
    [ "$r4" -eq 0 ] && ran+=("$o4")
    if [ "${#ran[@]}" -ge 2 ]; then
        ref="${ran[0]}"; same=1
        for f in "${ran[@]:1}"; do
            if ! diff -q "$ref" "$f" >/dev/null 2>&1; then same=0; break; fi
        done
        if [ "$same" -eq 1 ]; then eq="OK(${#ran[@]})"; EQ_PASS=$((EQ_PASS+1))
        else eq="DIFF";   EQ_FAIL=$((EQ_FAIL+1)); fi
    elif [ "${#ran[@]}" -eq 1 ]; then
        eq="1mode"
    else
        eq="none"
    fi

    printf "%-26s %8s %8s %8s   %s\n" "$base" "$ms2" "$ms3" "$ms4" "$eq"
done

echo "-------------------------------------------------------------------------"
echo "ran:   m2=$M2_OK/$TOTAL  m3=$M3_OK/$TOTAL  m4=$M4_OK/$TOTAL"
echo "equiv: PASS(>=2 modes agree)=$EQ_PASS  DIFF=$EQ_FAIL"
echo ""
echo "Goal: all benches run + agree in all 3 modes (m2=m3=m4=$TOTAL, DIFF=0)."
# Correctness gate: no DIFF among modes that ran. Mode-availability is tracked
# separately (the SBL-PAT-PRIM / Mode-4 work brings ran-counts up over time).
[ "$EQ_FAIL" -eq 0 ]
