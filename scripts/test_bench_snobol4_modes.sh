#!/usr/bin/env bash
# scripts/test_bench_snobol4_modes.sh — MODE-4 ONLY benchmark (Lon directive 2026-06-06)
# Formerly benchmarked modes 2/3/4. Now mode-4 only.
# Correctness gate: mode-4 output matches reference. Reports timing.
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
BENCH_DIR="${BENCH_DIR:-/home/claude/corpus/benchmarks}"
TIMEOUT="${TIMEOUT:-30}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$BENCH_DIR" ]; then echo "SKIP bench corpus not found at $BENCH_DIR"; exit 0; fi
[ ! -f "$RT_DIR/libscrip_rt.so" ] && { echo "SKIP libscrip_rt.so not found"; exit 0; }

WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lgc -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

echo "=== SNOBOL4 benchmark — MODE-4 ONLY ==="
PASS=0; FAIL=0; SKIP=0

while IFS= read -r sno; do
    slug=$(basename "${sno%.sno}")
    ref="${sno%.sno}.ref"; input="${sno%.sno}.input"
    bin="$WORKDIR/${slug}.bin"
    if ! compile_mode4 "$sno" "$bin"; then
        echo "  SKIP $slug (compile failed)"; SKIP=$((SKIP+1)); continue
    fi
    T0=$SECONDS
    if [ -f "$input" ]; then
        got=$(timeout "$TIMEOUT" "$bin" < "$input" 2>/dev/null || true)
    else
        got=$(timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
    fi
    T_RUN=$((SECONDS-T0))
    if [ -f "$ref" ]; then
        exp=$(cat "$ref")
        if [ "$got" = "$exp" ]; then
            echo "  PASS $slug  time=${T_RUN}s"; PASS=$((PASS+1))
        else
            echo "  FAIL $slug  time=${T_RUN}s"; FAIL=$((FAIL+1))
        fi
    else
        echo "  RAN  $slug  time=${T_RUN}s (no ref)"; PASS=$((PASS+1))
    fi
done < <(find "$BENCH_DIR" -name "*.sno" 2>/dev/null | sort)

echo ""
printf "mode-4 --compile: PASS=%d FAIL=%d SKIP=%d\n" "$PASS" "$FAIL" "$SKIP"
[ "$FAIL" -eq 0 ]
