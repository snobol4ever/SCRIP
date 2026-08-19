#!/usr/bin/env bash
# test_gate_pl_m34_parity.sh — M34-3: verify m3 (--run) and m4 (--compile x86) take identical paths.
# PASS   : both produce matching output (correct or wrong — they agree).
# DECLINED: both abort (m3 SIGABRT rc=134, m4 link/asm/emit fails rc!=0). Not a failure.
# FAIL   : one side aborts and the other produces output; OR both produce output but they differ.
# Gate: zero FAIL before PL-GZ-FENCE. Initial run establishes the honest baseline.
# Authors: LCherryholmes · Claude Sonnet 4.6   DATE: 2026-06-07
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/programs/prolog}"
VERBOSE=0
[ "${1:-}" = "--verbose" ] && VERBOSE=1
PASS=0; FAIL=0; DECLINED=0; SKIP=0
TMP3=$(mktemp /tmp/plm3_XXXXXX); TMP4=$(mktemp /tmp/plm4_XXXXXX)
trap 'rm -f "$TMP3" "$TMP4"' EXIT
for pl in "$CORPUS"/rung0[1-9]_*.pl \
          "$CORPUS"/rung1[0-9]_*.pl \
          "$CORPUS"/rung2[0-9]_*.pl \
          "$CORPUS"/rung3[0-9]_*.pl \
          "$CORPUS"/rung4[0-9]_*.pl; do
    [ -f "$pl" ] || continue
    [ -f "${pl%.pl}.expected" ] || { SKIP=$((SKIP+1)); continue; }
    name=$(basename "$pl" .pl)
    timeout 8 "$SCRIP" --run "$pl" < /dev/null > "$TMP3" 2>/dev/null; m3_rc=$?
    timeout 8 bash "$HERE/run_prolog_via_x86_backend.sh" "$pl" < /dev/null > "$TMP4" 2>/dev/null; m4_rc=$?
    m3_abort=0; [ $m3_rc -eq 134 ] && m3_abort=1
    m4_abort=0; [ $m4_rc -ne 0  ] && m4_abort=1
    m3_out=$(cat "$TMP3"); m4_out=$(cat "$TMP4")
    if [ $m3_abort -eq 1 ] && [ $m4_abort -eq 1 ]; then
        [ $VERBOSE -eq 1 ] && echo "DECLINED $name (both abort: m3_rc=$m3_rc m4_rc=$m4_rc)"
        DECLINED=$((DECLINED+1))
    elif [ $m3_abort -eq 1 ] && [ $m4_abort -eq 0 ]; then
        echo "FAIL $name (m3 aborts rc=$m3_rc, m4 outputs: $(printf '%s' "$m4_out" | head -1))"
        FAIL=$((FAIL+1))
    elif [ $m3_abort -eq 0 ] && [ $m4_abort -eq 1 ]; then
        echo "FAIL $name (m4 aborts rc=$m4_rc, m3 outputs: $(printf '%s' "$m3_out" | head -1))"
        FAIL=$((FAIL+1))
    elif [ "$m3_out" = "$m4_out" ]; then
        [ $VERBOSE -eq 1 ] && echo "PASS $name"
        PASS=$((PASS+1))
    else
        echo "FAIL $name (outputs differ)"
        [ $VERBOSE -eq 1 ] && printf "  m3: %s\n  m4: %s\n" \
            "$(printf '%s' "$m3_out" | tr '\n' '|')" \
            "$(printf '%s' "$m4_out" | tr '\n' '|')"
        FAIL=$((FAIL+1))
    fi
done
echo "--- PL-M34-PARITY: PASS=$PASS FAIL=$FAIL DECLINED=$DECLINED SKIP=$SKIP ---"
[ "$FAIL" -eq 0 ]
