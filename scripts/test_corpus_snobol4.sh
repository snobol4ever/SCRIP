#!/usr/bin/env bash
# scripts/test_corpus_snobol4.sh — SNOBOL4 broad corpus, modes 2+3+4
# Mode-4 gate (hard). Modes 2+3 informational. Reinstated 2026-06-08.
# Compares output against .ref files. Reports PASS/FAIL/SKIP per mode.
#
# Self-contained per RULES.md: paths from $0, timeout on every run.
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6   DATE: 2026-05-14
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="$S4E/corpus"
TIMEOUT="${TIMEOUT:-10}"
INC="$CORPUS/programs/snobol4/demo/inc"
BEAUTY="$CORPUS/programs/snobol4/beauty_suite"
DEMO="$CORPUS/programs/snobol4/demo"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$CORPUS" ]; then echo "SKIP corpus not found at $CORPUS"; exit 0; fi

PASS3=0; FAIL3=0; FAILURES3=""
PASS4=0; FAIL4=0; SKIP4=0; FAILURES4=""

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT
T_M3=0; T_M4=0; T0_ALL=$SECONDS

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$INC" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

run_test() {
    local label="$1" sno="$2" ref="$3" input="${4:-}" filter="${5:-}"
    [ ! -f "$ref" ] && return
    [ ! -f "$sno" ] && return
    local exp; exp=$(cat "$ref")
    local slug; slug=$(echo "$label" | tr '/: ' '_')
    local inp_arg; [ -n "$input" ] && [ -f "$input" ] && inp_arg="$input" || inp_arg=""


    # ── Mode 3: --run ──────────────────────────────────────────────────────
    local T0m3=$SECONDS
    local got3
    if [ -n "$inp_arg" ]; then
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$inp_arg" 2>/dev/null || true)
    else
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got3=$(printf '%s\n' "$got3" | grep -v "$filter" || true)
    T_M3=$((T_M3+SECONDS-T0m3))
    if [ "$got3" = "$exp" ]; then PASS3=$((PASS3+1))
    else FAIL3=$((FAIL3+1)); FAILURES3="${FAILURES3}  FAIL-M3 ${label}\n"; fi

    # ── Mode 4: --compile → assemble → link → run ─────────────────────────
    local T0m4=$SECONDS
    if [ ! -f "$RT_DIR/libscrip_rt.so" ]; then SKIP4=$((SKIP4+1)); return; fi
    local bin="$WORKDIR/${slug}.bin"
    if ! compile_mode4 "$sno" "$bin"; then SKIP4=$((SKIP4+1)); FAILURES4="${FAILURES4}  SKIP(compile/link) ${label}\n"; return; fi
    local got4
    if [ -n "$inp_arg" ]; then
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < "$inp_arg" 2>/dev/null || true)
    else
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got4=$(printf '%s\n' "$got4" | grep -v "$filter" || true)
    T_M4=$((T_M4+SECONDS-T0m4))
    if [ "$got4" = "$exp" ]; then PASS4=$((PASS4+1))
    else FAIL4=$((FAIL4+1)); FAILURES4="${FAILURES4}  FAIL ${label}\n"; fi
}

# ── Crosscheck corpus ──────────────────────────────────────────────────────────
while IFS= read -r sno; do
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    label=$(basename "$sno" .sno)
    run_test "$label" "$sno" "$ref" "$input" ""
done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)

# ── Beauty library drivers ─────────────────────────────────────────────────────
for sno in "$BEAUTY"/*_driver.sno; do
    [ ! -f "$sno" ] && continue
    name=$(basename "$sno" .sno)
    ref="$BEAUTY/${name}.ref"
    run_test "$name" "$sno" "$ref" "" ""
done

# ── Demo programs ─────────────────────────────────────────────────────────────
run_test "demo_wordcount" "$DEMO/wordcount.sno" "$DEMO/wordcount.ref" "$DEMO/wordcount.input" ""
run_test "demo_treebank"  "$DEMO/treebank.sno"  "$DEMO/treebank.ref"  "$DEMO/treebank.input"  ""
run_test "demo_claws5"    "$DEMO/claws5.sno"    "$DEMO/claws5.ref"    "$DEMO/claws5.input"    ""
TIMEOUT=30 \
run_test "demo_roman"     "$DEMO/roman.sno"     "$DEMO/roman.ref"     ""                      "^ms:"

T_ALL=$((SECONDS-T0_ALL))
TOTAL=$((PASS4+FAIL4+SKIP4))
echo "mode-3 (--run):     PASS=$PASS3 FAIL=$FAIL3"
echo "mode-4 (--compile): PASS=$PASS4 FAIL=$FAIL4 SKIP=$SKIP4  ($TOTAL total)"
[ -n "$FAILURES3" ] && printf "$FAILURES3" | head -20
[ -n "$FAILURES4" ] && printf "$FAILURES4" | head -40

printf "TIME M3=%ds M4=%ds TOTAL=%ds\n" "$T_M3" "$T_M4" "$T_ALL"
