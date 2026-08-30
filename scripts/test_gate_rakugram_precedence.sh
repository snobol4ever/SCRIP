#!/usr/bin/env bash
# test_gate_rakugram_precedence.sh — RUNG 4 gate for the Grammar.nqp -> recursive-descent port.
#
# Regenerates Raku's operator-precedence table from Rakudo's Grammar.nqp, compiles the emitted Pratt
# parser, and proves three things about it. The third is the one that matters:
#
#   1. PRECEDENCE   2 + 3 * 4  folds as  (2 + (3 * 4))
#   2. ASSOCIATIVITY  1 - 2 - 3 -> ((1-2)-3) left, 2 ** 3 ** 2 -> (2**(3**2)) right
#      ⛔ Associativity is INVISIBLE to any syntax-only test: a parser that folds 1-2-3 as 1-(2-3)
#         accepts exactly the same language. Only a shape assertion catches it.
#   3. PARSE-TIME EXTENSION — install a new operator at a precedence BETWEEN two existing levels
#      while parsing, and see it bind correctly. This is the property that makes Raku non-LALR and
#      the reason the bison grammar was retired (Lon 2026-08-30); a gate that did not exercise it
#      would pass just as happily against a frozen table, which is the exact defect being avoided.
#
# REFUSES rc=2 when it cannot measure (no Grammar.nqp, no compiler) rather than reporting green.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT=$(pwd)
GRAM="${RAKUDO_GRAMMAR:-$ROOT/refs/rakudo-main/src/Perl6/Grammar.nqp}"
[ -f "$GRAM" ] || GRAM=/home/resources/rakudo-main/src/Perl6/Grammar.nqp
[ -f "$GRAM" ] || { echo "⛔ REFUSES rc=2: no Grammar.nqp (wire refs/rakudo-main)"; exit 2; }
command -v gcc >/dev/null 2>&1 || { echo "⛔ REFUSES rc=2: no gcc"; exit 2; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
python3 "$ROOT/tools/rakugram/nqp_prec.py" "$GRAM" "$T/rk_prec_gen.c" --emit > "$T/gen.log" 2>&1 || {
    echo "⛔ REFUSES rc=2: generator failed"; sed -n '1,15p' "$T/gen.log"; exit 2; }
grep -q 'rk_ops_init_builtin' "$T/rk_prec_gen.c" || { echo "⛔ REFUSES rc=2: generator emitted no table"; exit 2; }
nops=$(grep -c 'rk_op_install(' "$T/rk_prec_gen.c")
[ "$nops" -ge 150 ] || { echo "⛔ REFUSES rc=2: only $nops operators extracted (expected >=150) -- the"; \
    echo "   extractor silently matched nothing rather than failing; grading this would be vacuous."; exit 2; }
cp "$ROOT/tools/rakugram/prec_test.c" "$T/" || exit 2
gcc -O0 -Wall -o "$T/prec_test" "$T/prec_test.c" -I"$T" 2> "$T/cc.log" || {
    echo "⛔ GATE FAIL: emitted Pratt parser does not compile"; sed -n '1,15p' "$T/cc.log"; exit 1; }
"$T/prec_test"; rc=$?
if [ "$rc" -eq 0 ]; then echo "✅ GATE OK: $nops operators, precedence + associativity + parse-time extension all verified."; else echo "⛔ GATE FAIL: see the FAIL rows above."; fi
exit $rc
