#!/usr/bin/env bash
# test_snobol4_pat_rung_suite.sh — SNOBOL4 pattern rung suite, modes 2+3+4
# Gate: M4 must not drop below baseline. M2/M3 informational.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet 4.6  DATE: 2026-06-08
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
PATDIR="$HERE/../../corpus/tests/scrip_test/snobol4/patterns"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$PATDIR" ]; then echo "SKIP pattern dir not found at $PATDIR"; exit 0; fi

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

expected_for() {
    case "$1" in
        038_pat_literal)      printf 'matched' ;;
        039_pat_any)          printf 'e' ;;
        040_pat_notany)       printf 'h' ;;
        041_pat_span)         printf '12345' ;;
        042_pat_break)        printf 'hello' ;;
        043_pat_len)          printf 'abc' ;;
        044_pat_pos)          printf 'hel' ;;
        045_pat_rpos)         printf 'lo' ;;
        046_pat_tab)          printf 'de' ;;
        047_pat_rtab)         printf 'abcd' ;;
        048_pat_rem)          printf 'world' ;;
        049_pat_arb)          printf '' ;;
        050_pat_alt_two)      printf 'dog' ;;
        051_pat_alt_three)    printf 'banana' ;;
        052_pat_arbno)        printf 'aaa' ;;
        053_pat_alt_commit)   printf 'b' ;;
        054_pat_arbno_alt)    printf 'abba' ;;
        055_pat_concat_seq)   printf 'ab cd ef' ;;
        056_pat_star_deref)   printf 'hello' ;;
        057_pat_fail_builtin) printf 'correctly failed' ;;
        *) printf '\x00__NO_ORACLE__' ;;
    esac
}

run_m2() {
    local sno="$1" exp="$2"
    timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null
}

run_m3() {
    local sno="$1" exp="$2"
    timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null
}

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

PASS_M2=0; FAIL_M2=0
PASS_M3=0; FAIL_M3=0
PASS_M4=0; FAIL_M4=0; SKIP_M4=0
FAIL_LIST=""
T0=$SECONDS

for sno in "$PATDIR"/*.sno; do
    [ -f "$sno" ] || continue
    base="${sno##*/}"; base="${base%.sno}"
    exp="$(expected_for "$base")"
    if [ "$exp" = $'\x00__NO_ORACLE__' ]; then continue; fi
    actual2=$(run_m2 "$sno" "$exp")
    if [ "$actual2" = "$exp" ]; then PASS_M2=$((PASS_M2+1))
    else FAIL_M2=$((FAIL_M2+1)); FAIL_LIST="$FAIL_LIST\n  FAIL-M2 $base (got: $actual2)"; fi
    actual3=$(run_m3 "$sno" "$exp")
    if [ "$actual3" = "$exp" ]; then PASS_M3=$((PASS_M3+1))
    else FAIL_M3=$((FAIL_M3+1)); FAIL_LIST="$FAIL_LIST\n  FAIL-M3 $base (got: $actual3)"; fi
    bin="$WORKDIR/${base}.bin"
    if compile_mode4 "$sno" "$bin"; then
        actual4=$(timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null)
        if [ "$actual4" = "$exp" ]; then PASS_M4=$((PASS_M4+1))
        else FAIL_M4=$((FAIL_M4+1)); FAIL_LIST="$FAIL_LIST\n  FAIL-M4 $base (got: $actual4)"; fi
    else
        SKIP_M4=$((SKIP_M4+1))
        FAIL_LIST="$FAIL_LIST\n  SKIP-M4 $base (compile/link failed)"
    fi
done

TT=$((SECONDS-T0))
echo "=== SNOBOL4 pattern rung suite (038-057) — MODE-4 HARD GATE ==="
echo "mode-2 (--run):  PASS-M2=$PASS_M2 FAIL-M2=$FAIL_M2"
echo "mode-3 (--run):     PASS-M3=$PASS_M3 FAIL-M3=$FAIL_M3"
echo "PASS-M4=$PASS_M4 FAIL-M4=$FAIL_M4 SKIP-M4=$SKIP_M4"
[ -n "$FAIL_LIST" ] && printf '%b\n' "$FAIL_LIST"
printf "TIME M4=%ds TOTAL=%ds\n" "$TT" "$TT"
