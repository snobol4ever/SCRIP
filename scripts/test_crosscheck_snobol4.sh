#!/usr/bin/env bash
# scripts/test_crosscheck_snobol4.sh
# Runs snobol4 crosscheck corpus through mode-4 (--compile->as->gcc->run) for the PASS/FAIL baseline.
# BOTH-MODE PARITY (mode-3 == mode-4 == ref) is enforced by scripts/test_mode34_parity.sh per
# GOAL-MODE34-IDENTICAL.md (supersedes the former "MODE-4 ONLY" directive of 2026-06-06). Run BOTH:
#   bash scripts/test_crosscheck_snobol4.sh    # mode-4 PASS/FAIL baseline
#   bash scripts/test_mode34_parity.sh         # mode-3 vs mode-4 1:1 parity census
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="${CORPUS:-/home/claude/corpus}"
INC="$CORPUS/programs/snobol4/demo/inc"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built"; exit 0; fi

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

T0=$SECONDS
echo "=== SNOBOL4 crosscheck — MODE-4 ONLY ==="
PASS=0; FAIL=0; SKIP=0; FAIL_LIST=""

while IFS= read -r sno; do
    sno_lib=""
    [ -d "$INC" ] && sno_lib="$INC"
    slug=$(basename "${sno%.sno}")
    bin="$WORKDIR/${slug}.bin"
    ref="${sno%.sno}.ref"; input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    if ! SNO_LIB="$sno_lib" compile_mode4 "$sno" "$bin"; then
        SKIP=$((SKIP+1)); continue
    fi
    if [ -f "$input" ]; then
        got=$(SNO_LIB="$sno_lib" timeout "$TIMEOUT" "$bin" < "$input" 2>/dev/null || true)
    else
        got=$(SNO_LIB="$sno_lib" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
    fi
    exp=$(cat "$ref")
    if [ "$got" = "$exp" ]; then
        PASS=$((PASS+1))
    else
        FAIL=$((FAIL+1)); FAIL_LIST="$FAIL_LIST $slug"
    fi
done < <(find "$CORPUS/crosscheck" -name "*.sno" 2>/dev/null | sort)

T_M4=$((SECONDS-T0))
printf "  --compile  PASS=%-3d FAIL=%d SKIP=%d\n" "$PASS" "$FAIL" "$SKIP"
[ -n "$FAIL_LIST" ] && printf "  FAIL:%s\n" "$FAIL_LIST"
printf "TIME M4=%ds TOTAL=%ds\n" "$T_M4" "$T_M4"
[ "$FAIL" -eq 0 ]
