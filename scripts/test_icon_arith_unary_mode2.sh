#!/bin/bash
# test_icon_arith_unary_mode2.sh — Icon mode-2 (--interp) arithmetic/unary oracle gate.
#
# Locks in two foundation correctness fixes (and the surrounding verified-correct
# behavior) so a future change that regresses them is caught:
#   * IR_UNOP exec arm (commit 8615c04): unary - + * \ ~ compute (were silently failing).
#   * BINOP_POW integer result (commit de0ce21): int ^ nonneg-int -> integer, not real.
# Grounded in canonical Icon refs/icon-master/src/runtime/oarith.r (^ / iipow / bigpowii)
# and ocomp.r (relops). Mode-2 (--interp) is the GOAL-ICON-BB oracle (HARD gate).
#
# Each case: a one-statement procedure, run through `scrip --interp`, output compared
# to the expected Icon value. Exits 0 iff every case matches.
set -u
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
SCRIP="${SCRIP:-$SCRIPT_DIR/../scrip}"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
PASS=0
FAIL=0
check() {
    desc="$1"; expr="$2"; want="$3"
    printf 'procedure main()\n   %s\nend\n' "$expr" > "$TMP/p.icn"
    got=$("$SCRIP" --interp "$TMP/p.icn" < /dev/null 2>/dev/null | head -1)
    if [ "$got" = "$want" ]; then
        PASS=$((PASS + 1))
    else
        FAIL=$((FAIL + 1))
        printf '  [FAIL] %-32s want=%-12s got=%s\n' "$desc" "$want" "$got"
    fi
}
echo "=== Icon mode-2 arithmetic / unary oracle gate ==="
# unary (IR_UNOP) — the fix in 8615c04
check "neg literal"        'write(-7)'           '-7'
check "neg of expr"        'write(-(3+4))'       '-7'
check "unary + coerce"     'write(+5)'           '5'
check "size *"             'write(*"hello")'     '5'
# integer power (BINOP_POW) — the fix in de0ce21
check "int pow"            'write(2^10)'         '1024'
check "int pow zero exp"   'write(2^0)'          '1'
check "int pow 3^3"        'write(3^3)'          '27'
check "real base pow"      'write(2.0^10)'       '1024.0'
check "frac exp -> real"   'write(2^0.5)'        '1.4142135623730951'
# surrounding verified-correct arithmetic
check "add"                'write(2+3)'          '5'
check "int div trunc"      'write(-17 / 5)'      '-3'
check "mod sign=dividend"  'write(-17 % 5)'      '-2'
check "real div"           'write(7.0 / 2)'      '3.5'
# relational (returns RHS on success; fails -> no output)
check "lt returns rhs"     'write(3 < 5)'        '5'
check "chain lt"           'write(1 < 2 < 3)'    '3'
# string relops + concat coercion
check "string << "         'write("abc" << "abd")' 'abd'
check "string ~=="         'write("abc" ~== "abd")' 'abd'
check "num concat"         'write(1 || 2)'       '12'
echo "PASS=$PASS FAIL=$FAIL"
[ "$FAIL" -eq 0 ]
