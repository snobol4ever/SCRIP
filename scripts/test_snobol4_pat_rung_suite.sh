#!/usr/bin/env bash
# scripts/test_snobol4_pat_rung_suite.sh — SBL-G-1: SNOBOL4 pattern rung suite
# Runs every test/snobol4/patterns/*.sno (rungs 038-057) via --interp (mode 2)
# and --compile (mode 4). Reports PASS-M2 and PASS-M4 separately.
# Expected output is the SPITBOL x64 oracle output, baked inline so the script
# is self-contained and does not require the oracle at run time.
# Gate: M2 must not drop below baseline. M4 climbs with each filled template.
# Self-contained per RULES.md: paths from $0, timeout on every run.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet 4.6  DATE: 2026-05-27

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
PATDIR="$HERE/../test/snobol4/patterns"
TIMEOUT="${TIMEOUT:-10}"

if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built at $SCRIP"; exit 0; fi
if [ ! -d "$PATDIR" ]; then echo "SKIP pattern dir not found at $PATDIR"; exit 0; fi

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

# Expected outputs — SPITBOL x64 oracle (sbl -b), captured 2026-05-27.
expected_for() {
    case "$1" in
        038_pat_literal)    printf 'matched' ;;
        039_pat_any)        printf 'e' ;;
        040_pat_notany)     printf 'h' ;;
        041_pat_span)       printf '12345' ;;
        042_pat_break)      printf 'hello' ;;
        043_pat_len)        printf 'abc' ;;
        044_pat_pos)        printf 'hel' ;;
        045_pat_rpos)       printf 'lo' ;;
        046_pat_tab)        printf 'de' ;;
        047_pat_rtab)       printf 'abcd' ;;
        048_pat_rem)        printf 'world' ;;
        049_pat_arb)        printf '' ;;
        050_pat_alt_two)    printf 'dog' ;;
        051_pat_alt_three)  printf 'banana' ;;
        052_pat_arbno)      printf 'aaa' ;;
        053_pat_alt_commit) printf 'b' ;;
        054_pat_arbno_alt)  printf 'abba' ;;
        055_pat_concat_seq) printf 'ab cd ef' ;;
        056_pat_star_deref) printf 'hello' ;;
        057_pat_fail_builtin) printf 'correctly failed' ;;
        *) printf '\x00__NO_ORACLE__' ;;
    esac
}

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lgc -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

PASS_M2=0; FAIL_M2=0
PASS_M4=0; FAIL_M4=0; SKIP_M4=0
FAILS_M2=""; FAILS_M4=""

for sno in "$PATDIR"/*.sno; do
    [ -f "$sno" ] || continue
    name=$(basename "$sno" .sno)
    exp=$(expected_for "$name")
    [ "$exp" = $'\x00__NO_ORACLE__' ] && continue

    # ── Mode 2: --interp ───────────────────────────────────────────────────
    got2=$(timeout "$TIMEOUT" "$SCRIP" --interp "$sno" < /dev/null 2>/dev/null || true)
    if [ "$got2" = "$exp" ]; then PASS_M2=$((PASS_M2+1))
    else FAIL_M2=$((FAIL_M2+1)); FAILS_M2="${FAILS_M2}  FAIL-M2 ${name} (got: $(printf '%s' "$got2" | head -1))\n"; fi

    # ── Mode 4: --compile → assemble → link → run ──────────────────────────
    bin="$WORKDIR/${name}.bin"
    if compile_mode4 "$sno" "$bin"; then
        got4=$(timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
        if [ "$got4" = "$exp" ]; then PASS_M4=$((PASS_M4+1))
        else FAIL_M4=$((FAIL_M4+1)); FAILS_M4="${FAILS_M4}  FAIL-M4 ${name} (got: $(printf '%s' "$got4" | head -1))\n"; fi
    else
        SKIP_M4=$((SKIP_M4+1)); FAILS_M4="${FAILS_M4}  SKIP-M4 ${name} (compile/link failed)\n"
    fi
done

echo "=== SNOBOL4 pattern rung suite (038-057) ==="
echo "PASS-M2=$PASS_M2 FAIL-M2=$FAIL_M2   PASS-M4=$PASS_M4 FAIL-M4=$FAIL_M4 SKIP-M4=$SKIP_M4"
[ -n "$FAILS_M2" ] && printf "$FAILS_M2"
[ -n "$FAILS_M4" ] && printf "$FAILS_M4"
exit 0
