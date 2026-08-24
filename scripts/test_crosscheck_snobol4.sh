#!/usr/bin/env bash
# scripts/test_crosscheck_snobol4.sh
# Runs the snobol4 crosscheck corpus through BOTH backends against each program's .ref:
#   mode-3 (--run, in-process binary)         -> P3/F3/S3
#   mode-4 (--compile -> gcc -c -> gcc -> run) -> P4/F4/S4
# Every program is exercised in BOTH modes (Lon directive 2026-06-25: no mode-only suites).
# DIVERGE = programs where mode-3 and mode-4 disagree vs the ref (the 1:1-parity signal that was
# formerly split into test_mode34_parity.sh). Gate: BOTH modes' FAIL == 0 AND zero DIVERGE.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="${CORPUS:-$S4E/corpus}"
INC="$CORPUS/snobol4/demo/inc"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built"; exit 0; fi

WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT

# mode-4: compile .sno -> .s -> .o -> linked binary. echoes the binary path on success, returns 1 on failure.
compile_mode4() {
    local sno="$1" out="$2" lib="$3"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$lib" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

T0=$SECONDS
echo "=== SNOBOL4 crosscheck — MODES 3+4 (both backends vs .ref) ==="
P3=0; F3=0; S3=0; P4=0; F4=0; S4=0; DIV=0
F3_LIST=""; F4_LIST=""; DIV_LIST=""

while IFS= read -r sno; do
    sno_lib=""
    [ -d "$INC" ] && sno_lib="$INC"
    slug=$(basename "${sno%.sno}")
    ref="${sno%.sno}.ref"; input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    exp=$(cat "$ref")
    [ -f "$input" ] && IN="$input" || IN=/dev/null

    # ---- mode-3 (--run) ----
    m3=$(SNO_LIB="$sno_lib" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$IN" 2>/dev/null || true)
    if [ "$m3" = "$exp" ]; then ok3=1; P3=$((P3+1)); else ok3=0; F3=$((F3+1)); F3_LIST="$F3_LIST $slug"; fi

    # ---- mode-4 (--compile -> run) ----
    bin="$WORKDIR/${slug}.bin"
    if SNO_LIB="$sno_lib" compile_mode4 "$sno" "$bin" "$sno_lib"; then
        m4=$(SNO_LIB="$sno_lib" timeout "$TIMEOUT" "$bin" < "$IN" 2>/dev/null || true)
        if [ "$m4" = "$exp" ]; then ok4=1; P4=$((P4+1)); else ok4=0; F4=$((F4+1)); F4_LIST="$F4_LIST $slug"; fi
    else
        ok4=-1; S4=$((S4+1))
    fi

    # ---- divergence: the two backends disagree vs the ref ----
    if [ "$ok4" != "-1" ] && [ "$ok3" != "$ok4" ]; then DIV=$((DIV+1)); DIV_LIST="$DIV_LIST $slug"; fi
done < <(find "$CORPUS/crosscheck" -name "*.sno" 2>/dev/null | sort)

T=$((SECONDS-T0))
printf "  --run      PASS=%-3d FAIL=%d SKIP=%d\n" "$P3" "$F3" "$S3"
printf "  --compile  PASS=%-3d FAIL=%d SKIP=%d\n" "$P4" "$F4" "$S4"
printf "  DIVERGE (mode-3 != mode-4 vs ref): %d\n" "$DIV"
[ -n "$F3_LIST" ]  && printf "  FAIL(m3):%s\n" "$F3_LIST"
[ -n "$F4_LIST" ]  && printf "  FAIL(m4):%s\n" "$F4_LIST"
[ -n "$DIV_LIST" ] && printf "  DIVERGE:%s\n" "$DIV_LIST"
printf "TIME TOTAL=%ds\n" "$T"
[ "$F3" -eq 0 ] && [ "$F4" -eq 0 ] && [ "$DIV" -eq 0 ]
