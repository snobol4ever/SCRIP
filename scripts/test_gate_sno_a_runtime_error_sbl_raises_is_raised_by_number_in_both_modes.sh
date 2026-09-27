#!/usr/bin/env bash
# test_gate_sno_a_runtime_error_sbl_raises_is_raised_by_number_in_both_modes.sh -- row snobol4-runtime-errors-sbl-raises-are-
# silent-statement-failures-in-scrip-binary-arithmetic-array-and-replace-and-negation-carries-the-wrong-error-number-ceo-1304
# (found by the ceo's audit, CEO-1304; routed to the cfo, CEO-1306; cured by the cfo 2026-09-27).
#
# THE DEFECT: under &ERRLIMIT 0 (the default of both) sbl -bf raises a FATAL runtime error and terminates on x + 1, x - 1,
# x * 2, x / 2 and x ** 2 with x = 'abc', on ARRAY(0 - 1) and on REPLACE with unequally long 2nd and 3rd arguments; SCRIP
# failed the statement silently, took :F, ran on and exited 0, in both modes. -x was raised but numbered 1 where sbl says 010.
# NO SUITE SAW IT: corpus/tests/snobol4/ALL.ref carries no ERROR line, so these programs sit outside every master's
# denominator -- which is why a cure proven on them owes this gate (the ceo's rule of 2026-09-27: a cure proven on a program
# that leaves the denominator owes a gate that keeps it under test).
# THE CURE: the SNOBOL4 lowerer marks its arithmetic nodes strict=2 and bb_binop_arith calls a SNOBOL4-voiced entry family
# (rt_add_sno and its siblings: the same asm fast paths, a slow path that raises SPITBOL's number for the operand that is not
# numeric); Icon's strict=1 and every other frontend's strict=0 are unchanged. ARRAY raises 067 in both dispatch copies and in
# the prototype parser, REPLACE raises 171, negation 010, affirmation 004, and integer division by zero 014 (it said 2).
#
# EACH ARM is a one-statement program run by sbl -bf (the EXPECTED number is read live from the oracle, never typed) and by
# scrip in mode 3 and mode 4: the arm passes when scrip prints 'before', never reaches 'success' or 'failure', and names the
# same error number. A witness sbl does not raise on is a wrong witness and REFUSES rc=2.
# CONTROL ARMS: a numeric string operand ('12' + 1) raises nothing and prints 13 in both; ATAN('abc') raises 301 in both.
# ERRLIMIT ARM: with &ERRLIMIT = 5 the same x + 1 is counted, the statement FAILS, and &ERRTYPE reads 1, as sbl gives it.
# FAIL-ONCE, MEASURED: SCRIP_BIN at a scrip built on origin bc96cf1b2 (before the cure) reads 0 of 38 raising arms and the
# &ERRLIMIT arm RED in both modes (&ERRTYPE 0 where sbl gives 1), the two controls green -- the gate measures the cure.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"
RT_DIR="$(cd "$(dirname "$SCRIP")" && pwd)/out"
NAME=sno_a_runtime_error_sbl_raises_is_raised_by_number_in_both_modes
refuse() { echo "GATE REFUSE(2) [$NAME]: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- cannot measure"
[ -f "$RT_DIR/libscrip_rt.so" ] || refuse "no $RT_DIR/libscrip_rt.so -- cannot measure mode 4"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || true
SBL="$(sbl_correctness_bin 2>/dev/null || true)"; [ -n "${SBL:-}" ] && [ -x "$SBL" ] || SBL=/home/resources/x64/bin/sbl
[ -x "$SBL" ] || refuse "no sbl oracle -- cannot measure"
T="$(mktemp -d)" || refuse "no tmpdir"
trap 'rm -rf "$T"' EXIT
prog() {
    printf "        OUTPUT = 'before'\n        x = 'abc'\n        n = '12'\n        a = 'ab'\n        b = 'X'\n%s        OUTPUT = %s   :S(ok)F(bad)\nok      OUTPUT = 'success' :(END)\nbad     OUTPUT = 'failure'\nEND\n" "${2:-}" "$1" > "$T/w.sno"
}
runm() {
    if [ "$1" = m3 ]; then timeout 20 "$SCRIP" "$T/w.sno" < /dev/null 2>&1
    else rm -f "$T/w" "$T/w.s"
        "$SCRIP" --compile -o "$T/w.s" "$T/w.sno" < /dev/null > /dev/null 2>&1 && gcc -o "$T/w" "$T/w.s" -L "$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" -lm 2> /dev/null || { echo "M4-BUILD-FAILED"; return; }
        (cd "$T" && timeout 20 ./w < /dev/null 2>&1); fi
}
errno_of() { grep -io 'error [0-9]*' | head -1 | awk '{print $2+0}'; }
RC=0; n=0; ok=0
for ex in "x + 1" "1 + x" "x - 1" "2 - x" "x * 2" "2 * x" "x / 2" "2 / x" "x ** 2" "2 ** x" "-x" "+x" "1 / 0" \
          "ARRAY(0 - 1)" "ARRAY(0)" "ARRAY('')" "ARRAY('3:1')" "REPLACE('abcabc', a, b)" "REPLACE('abcabc', '', '')"; do
    prog "$ex"
    want=$("$SBL" -bf "$T/w.sno" < /dev/null 2>/dev/null | errno_of)
    [ -n "$want" ] || refuse "sbl -bf raised no error for [$ex] -- the witness is wrong"
    for m in m3 m4; do n=$((n+1))
        out=$(runm $m); got=$(printf '%s\n' "$out" | errno_of)
        if printf '%s\n' "$out" | grep -qx 'before' && ! printf '%s\n' "$out" | grep -qx 'failure\|success' && [ "$got" = "$want" ]; then ok=$((ok+1))
        else RC=1; echo "  RED $m [$ex]: sbl -bf raises ERROR $want and terminates; scrip: $(printf '%s' "$out" | tr '\n' ' ' | cut -c1-120)"; fi
    done
done
echo "  raising arms: $ok of $n raise the number sbl -bf raises and terminate, both modes"
for m in m3 m4; do
    prog "n + 1"; out=$(runm $m)
    if printf '%s\n' "$out" | grep -qx '13' && printf '%s\n' "$out" | grep -qx 'success' && ! printf '%s\n' "$out" | grep -qi 'error'; then :; else RC=1; echo "  RED $m control ['12' + 1]: expected 13 and success, got: $(printf '%s' "$out" | tr '\n' ' ' | cut -c1-120)"; fi
    prog "ATAN(x)"; want=$("$SBL" -bf "$T/w.sno" < /dev/null 2>/dev/null | errno_of); out=$(runm $m); got=$(printf '%s\n' "$out" | errno_of)
    [ -n "$want" ] && [ "$got" = "$want" ] || { RC=1; echo "  RED $m control [ATAN(x)]: sbl raises ${want:-nothing}, scrip ${got:-nothing}"; }
    prog "x + 1" $'        &ERRLIMIT = 5\n'
    sed -i "s/^bad     OUTPUT = 'failure'$/bad     OUTPUT = 'failure ' \&ERRTYPE/" "$T/w.sno"
    wantl=$("$SBL" -bf "$T/w.sno" < /dev/null 2>/dev/null | grep -x 'failure [0-9]*'); out=$(runm $m)
    if [ -n "$wantl" ] && printf '%s\n' "$out" | grep -qx "$wantl"; then :; else RC=1; echo "  RED $m &ERRLIMIT arm: sbl prints [${wantl:-nothing}], scrip: $(printf '%s' "$out" | tr '\n' ' ' | cut -c1-120)"; fi
done
[ $RC = 0 ] && echo "GATE PASS(0) [$NAME]: every runtime error of the witness set is raised by the number sbl -bf raises and terminates, in both modes ($ok of $n); the numeric control succeeds, ATAN's 301 holds, and &ERRLIMIT counts and fails the statement with sbl's &ERRTYPE" \
             || echo "GATE FAIL(1) [$NAME]: a runtime error sbl -bf raises is silent or misnumbered in scrip"
exit $RC
