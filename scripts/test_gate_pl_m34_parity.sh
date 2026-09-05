#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_pl_m34_parity.sh — M34-3: verify m3 (--run) and m4 (--compile x86) take identical paths.
# PASS   : both produce matching output (correct or wrong — they agree).
# REFUSED: both abort (m3 SIGABRT rc=134, m4 link/asm/emit fails rc!=0). Not a failure.
# FAIL   : one side aborts and the other produces output; OR both produce output but they differ.
# Gate: zero FAIL before PL-GZ-FENCE. Initial run establishes the honest baseline.
# Authors: LCherryholmes · Claude Sonnet 4.6   DATE: 2026-06-07
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/prolog}"
VERBOSE=0
[ "${1:-}" = "--verbose" ] && VERBOSE=1
PASS=0; FAIL=0; REFUSED=0; SKIP=0
TMP3=$(mktemp /tmp/plm3_XXXXXX); TMP4=$(mktemp /tmp/plm4_XXXXXX)
# ⭐ V2-5 GATE HONESTY: examining nothing must exit UNPROVEN(2), never read as a pass.
. "$(dirname "$0")/lib_gate.sh"
gate_require_exec "${SCRIP:-${SCRIP_BIN:-$(dirname "$0")/../scrip}}" "the scrip compiler"
gate_require "${RT_DIR:-$(dirname "$0")/../out}/libscrip_rt.so" "the runtime shared object out/libscrip_rt.so"
gate_floor "$(find "${CORPUS:-$(dirname "$0")/../../corpus}" -name '*.pl' 2>/dev/null | wc -l)" 1 "corpus .pl programs"   # ⛔ WAS '*.sno' (seat15 2026-09-01): a copy-paste from a SNOBOL4 gate floored this PROLOG gate on an extension its corpus does not contain -- corpus/tests/prolog holds 0 .sno and 45 .pl, so gate_floor saw 0 < 1 and exited UNPROVEN(2) BEFORE the first comparison. The gate never compared m3 against m4 once. Honest-dead, not false-green (V2-5 refused rather than passing), but zero coverage either way.
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
        [ $VERBOSE -eq 1 ] && echo "REFUSED $name (both abort: m3_rc=$m3_rc m4_rc=$m4_rc)"
        REFUSED=$((REFUSED+1))
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
echo "--- PL-M34-PARITY: PASS=$PASS FAIL=$FAIL REFUSED=$REFUSED SKIP=$SKIP ---"
[ "$FAIL" -eq 0 ]
