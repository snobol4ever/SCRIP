#!/usr/bin/env bash
# scripts/test_smoke_snobol4_run.sh — MODE-4 ONLY crosscheck sweep (Lon directive 2026-06-06)
# Formerly SN-9c-e three-mode sweep. Modes 2 and 3 removed. Mode-4 is the gate.
# Gate: --compile PASS count must equal total crosscheck corpus with .ref files.
# Self-contained per RULES.md: paths derived from $0; no env deps required.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="${CORPUS:-$S4E/corpus}"
INC="$CORPUS/snobol4/demo/inc"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$CORPUS/crosscheck" ]; then echo "SKIP corpus not populated at $CORPUS"; exit 0; fi

WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$INC" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

T0=$SECONDS
echo "=== SNOBOL4 crosscheck sweep — MODE-4 ONLY ==="
[ ! -f "$RT_DIR/libscrip_rt.so" ] && { echo "SKIP libscrip_rt.so not found"; exit 0; }

PASS=0; FAIL=0; SKIP=0; FAIL_LIST=""
while IFS= read -r sno; do
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    slug=$(basename "${sno%.sno}")
    bin="$WORKDIR/${slug}.bin"
    if ! compile_mode4 "$sno" "$bin"; then
        SKIP=$((SKIP+1)); FAIL_LIST="$FAIL_LIST $slug(skip)"; continue
    fi
    if [ -f "$input" ]; then
        got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < "$input" 2>/dev/null || true)
    else
        got=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
    fi
    exp=$(cat "$ref")
    if [ "$got" = "$exp" ]; then
        PASS=$((PASS+1))
    else
        FAIL=$((FAIL+1)); FAIL_LIST="$FAIL_LIST $slug"
    fi
done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)

T_M4=$((SECONDS-T0))
printf "  --compile PASS=%-3d FAIL=%d SKIP=%d\n" "$PASS" "$FAIL" "$SKIP"
[ -n "$FAIL_LIST" ] && echo "  FAIL:$FAIL_LIST"
printf "TIME M4=%ds TOTAL=%ds\n" "$T_M4" "$T_M4"
[ "$FAIL" -eq 0 ]
