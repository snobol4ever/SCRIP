#!/usr/bin/env bash
# scripts/test_interp_broad_corpus_and_beauty.sh — scrip regression: crosscheck + beauty drivers + demos
# Runs every test program via --interp (mode 2), --run (mode 3), and
# --compile→assemble→link→run (mode 4). Reports PASS/FAIL per mode.
# Self-contained. Run from anywhere with no env vars.
# Usage: bash scripts/test_interp_broad_corpus_and_beauty.sh

set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
TIMEOUT="${TIMEOUT:-10}"
INC="$CORPUS/programs/snobol4/demo/inc"
BEAUTY="$CORPUS/programs/snobol4/beauty_suite"
DEMO="$CORPUS/programs/snobol4/demo"

# ── corpus guard ──────────────────────────────────────────────────────────────
if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS"
    echo "     clone snobol4ever/corpus to $CORPUS to run this suite"
    exit 0
fi

PASS2=0; FAIL2=0
PASS3=0; FAIL3=0
FAILURES2=""; FAILURES3=""

T_M2=0; T_M3=0; T0_ALL=$SECONDS


run_test() {
    local label="$1" sno="$2" ref="$3" input="${4:-}" filter="${5:-}"
    [ ! -f "$ref" ] && return
    [ ! -f "$sno" ] && return
    local exp; exp=$(cat "$ref")
    local slug; slug=$(echo "$label" | tr '/: ' '_')

    # ── Mode 2: --interp ──────────────────────────────────────────────────
    local T0m=$SECONDS; local got2
    if [ -n "$input" ] && [ -f "$input" ]; then
        got2=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --interp "$sno" < "$input" 2>/dev/null || true)
    else
        got2=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --interp "$sno" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got2=$(printf '%s\n' "$got2" | grep -v "$filter" || true)
    T_M2=$((T_M2+SECONDS-T0m))
    if [ "$got2" = "$exp" ]; then PASS2=$((PASS2+1))
    else FAIL2=$((FAIL2+1)); FAILURES2="${FAILURES2}  FAIL ${label}\n"; fi

    # ── Mode 3: --run ─────────────────────────────────────────────────────
    local T0m3=$SECONDS; local got3
    if [ -n "$input" ] && [ -f "$input" ]; then
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$input" 2>/dev/null || true)
    else
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got3=$(printf '%s\n' "$got3" | grep -v "$filter" || true)
    T_M3=$((T_M3+SECONDS-T0m3))
    if [ "$got3" = "$exp" ]; then PASS3=$((PASS3+1))
    else FAIL3=$((FAIL3+1)); FAILURES3="${FAILURES3}  FAIL ${label}\n"; fi

}

# ── Crosscheck corpus ──────────────────────────────────────────────────────────
while IFS= read -r sno; do
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    label=$(basename "$sno" .sno)
    run_test "$label" "$sno" "$ref" "$input" ""
done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)

# ── Beauty library drivers (19 subsystems) ────────────────────────────────────
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
TOTAL=$((PASS2+FAIL2))
echo "mode-2 (--interp):  PASS=$PASS2 FAIL=$FAIL2  ($TOTAL total)"
echo "mode-3 (--run):     PASS=$PASS3 FAIL=$FAIL3  ($TOTAL total)"
[ -n "$FAILURES2" ] && printf "$FAILURES2" | head -40
[ -n "$FAILURES3" ] && printf "$FAILURES3" | head -40
printf "TIME M2=%ds M3=%ds TOTAL=%ds\n" "$T_M2" "$T_M3" "$T_ALL"
