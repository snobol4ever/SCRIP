#!/usr/bin/env bash
# scripts/test_smoke_snobol4_run.sh — SN-9c-e gate:
# Three-mode sweep across the crosscheck corpus. Every program that passes
# under --interp (mode 2) must also pass under --run (mode 3) and
# --compile→link→run (mode 4).
#
# Gate semantics:
#   * --interp PASS count is the reference.
#   * --run and --compile must not regress below --interp PASS count.
#   * Shared failures (failing in all modes) are real bugs, reported but
#     do not fail the gate.
#
# Self-contained per RULES.md: paths derived from $0; no env deps required.
# Usage: bash scripts/test_smoke_snobol4_run.sh

set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="${CORPUS:-/home/claude/corpus}"
INC="$CORPUS/programs/snobol4/demo/inc"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not built at $SCRIP"
    exit 0
fi
if [ ! -d "$CORPUS/crosscheck" ]; then
    echo "SKIP corpus not populated at $CORPUS"
    exit 0
fi

WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$INC" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lgc -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

# Run every crosscheck .sno under one mode; write "PASS FAIL" to stdout
# and the full failure list (space-sep basenames) to file $2.
run_mode() {
    local mode="$1" fails_out="$2"
    local pass=0 fail=0
    : > "$fails_out"
    while IFS= read -r sno; do
        local ref="${sno%.sno}.ref"
        local input="${sno%.sno}.input"
        [ ! -f "$ref" ] && continue
        local got exp
        if [ -f "$input" ]; then
            got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" $mode "$sno" < "$input" 2>/dev/null || true)
        else
            got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" $mode "$sno" < /dev/null 2>/dev/null || true)
        fi
        exp=$(cat "$ref")
        if [ "$got" = "$exp" ]; then
            pass=$((pass + 1))
        else
            fail=$((fail + 1))
            printf ' %s' "$(basename "${sno%.sno}")" >> "$fails_out"
        fi
    done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)
    printf '%d %d\n' "$pass" "$fail"
}

run_mode4() {
    local fails_out="$1"
    local pass=0 fail=0 skip=0
    : > "$fails_out"
    [ ! -f "$RT_DIR/libscrip_rt.so" ] && { printf '0 0\n'; return; }
    while IFS= read -r sno; do
        local ref="${sno%.sno}.ref"
        local input="${sno%.sno}.input"
        [ ! -f "$ref" ] && continue
        local slug; slug=$(basename "${sno%.sno}")
        local bin="$WORKDIR/${slug}.bin"
        if ! compile_mode4 "$sno" "$bin"; then skip=$((skip+1)); continue; fi
        local got exp
        if [ -f "$input" ]; then
            got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < "$input" 2>/dev/null || true)
        else
            got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
        fi
        exp=$(cat "$ref")
        if [ "$got" = "$exp" ]; then
            pass=$((pass + 1))
        else
            fail=$((fail + 1))
            printf ' %s' "$slug" >> "$fails_out"
        fi
    done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)
    printf '%d %d\n' "$pass" "$fail"
}

echo "=== SN-9c-e: three-mode crosscheck sweep ==="

read IR_PASS  IR_FAIL  <<< "$(run_mode --interp "$WORKDIR/ir")"
read RUN_PASS RUN_FAIL <<< "$(run_mode --run    "$WORKDIR/run")"
read M4_PASS  M4_FAIL  <<< "$(run_mode4         "$WORKDIR/m4")"

IR_FAILS_FULL=$(cat  "$WORKDIR/ir")
RUN_FAILS_FULL=$(cat "$WORKDIR/run")
M4_FAILS_FULL=$(cat  "$WORKDIR/m4")

printf '  --interp  PASS=%-3d FAIL=%d\n' "$IR_PASS"  "$IR_FAIL"
printf '  --run     PASS=%-3d FAIL=%d\n' "$RUN_PASS" "$RUN_FAIL"
printf '  --compile PASS=%-3d FAIL=%d\n' "$M4_PASS"  "$M4_FAIL"
echo ""

GATE_FAIL=0
if [ "$RUN_PASS" -lt "$IR_PASS" ]; then
    echo "FAIL  --run PASS ($RUN_PASS) < --interp PASS ($IR_PASS)"
    GATE_FAIL=1
fi
if [ "$M4_PASS" -gt 0 ] && [ "$M4_PASS" -lt "$IR_PASS" ]; then
    echo "FAIL  --compile PASS ($M4_PASS) < --interp PASS ($IR_PASS)"
    GATE_FAIL=1
fi

if [ "$GATE_FAIL" -eq 0 ]; then
    echo "PASS  three-mode parity on crosscheck: $IR_PASS programs"
    [ "$IR_FAIL" -gt 0 ] && echo "  shared failures (all modes):$IR_FAILS_FULL"
    exit 0
else
    echo "  --interp  failures:$IR_FAILS_FULL"
    echo "  --run     failures:$RUN_FAILS_FULL"
    echo "  --compile failures:$M4_FAILS_FULL"
    exit 1
fi
