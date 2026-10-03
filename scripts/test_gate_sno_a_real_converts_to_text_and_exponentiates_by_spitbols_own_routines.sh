#!/usr/bin/env bash
# test_gate_sno_a_real_converts_to_text_and_exponentiates_by_spitbols_own_routines.sh
#
# SPITBOL'S RULES (sbl.min, read by the cfo 2026-10-02 for ceo CEO-1419's ticket snobol4-real-to-string-differs-in-the-last-digit-
# and-a-real-in-a-match-loses-its-trailing-point; x64 sbl builds without .cncr, so its own code runs, never syscr):
#   REAL TO STRING, gts10-gts28: scale the magnitude into [0.1, 1.0) by steps of 10**10 and a powers-of-ten table, ADD the rounding
#   bias 0.5*10**-15, rescale once if that reached 1.0, multiply by 10**15 and TRUNCATE (cvttsd2si). That is not C's correctly
#   rounded "%.14e": 1.1 / 1.5 is 0.733333333333334 in sbl, and was 0.733333333333333 in SCRIP. The form rules were already SCRIP's
#   (an exponent below 0 or above 15 gives 0.ddde-N / 0.ddde+N, trailing fractional zeros go, a whole value keeps its point).
#   EXPONENTIATION, o_exp: a real exponent is e ** (exponent * ln(base)) through libm's log and exp (osint math.c f_lnf, f_etx),
#   not pow(), with a negative base allowed only at an integral exponent (else 311) and the sign set by its parity; a real base to
#   a non-negative integer power is the base multiplied in sequence (oex14), not squaring; a negative integer exponent becomes a
#   real one. 0 ** 0 and 0.0 ** 0.0 are 018, an overflow is 266.
#   A REAL IN A MATCH OR AS A STRING: every conversion is gtstg's, so a real pattern is '0.' and '2.', SIZE(2.5) is 3, and a TRACE
#   line spells the value the same way.
# WHAT SCRIP DID BEFORE: real_str rounded by "%.14e" (6125 of this gate's 174,600-line sweep differed, measured on the parent);
# rt_sno_pow used pow() and squaring (6050 of 19,200 differed); a deferred real pattern converted by "%g" ('0' ? R matched, sbl
# fails); bn_size returned 0 for every real; the trace voice spelled a real by "%g" (F(3) where sbl prints F(3.)).
# NOT COVERED: a subnormal result, which sbl zeroes through its MXCSR underflow test on every real operation (0. both sides here
# only by the conversion's own scaling), and Pascal's real writer, which has its own formatter and its own row.
#
# Expectations are cut from sbl -bf AT RUN TIME, both modes; every arm grades its whole stdout. The sweeps compute every value at
# run time, because sbl folds a constant expression at compile time. rc=0 clean · rc=1 a divergence · rc=2 REFUSAL.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="$HERE/.."
refuse() { echo "⛔ REFUSES rc=2: $*"; exit 2; }
[ -x "$B/scrip" ] || refuse "$B/scrip is not built -- cannot measure"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || true
SBL="$(sbl_correctness_bin 2>/dev/null || true)"; [ -n "${SBL:-}" ] && [ -x "$SBL" ] || SBL=/home/resources/x64/bin/sbl
[ -x "$SBL" ] || refuse "no sbl oracle -- the expectations are cut from it at run time"
"$HERE/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
D="$(mktemp -d)" || refuse "no scratch dir"
trap 'rm -rf "$D"' EXIT

cat > "$D/conv.sno" <<'SNO'
        I = 0
LI      I = I + 1
        GT(I, 60)                                   :S(END)
        J = 0
LJ      J = J + 1
        GT(J, 97)                                   :S(LI)
        X = (1.0 * I) / J
        OUTPUT = X
        OUTPUT = X * 1000.0
        OUTPUT = X * 1.0E13
        OUTPUT = X / 1.0E7
        OUTPUT = -X * 3.7
        OUTPUT = X * X * 1.0E20                      :(LJ)
END
SNO
cat > "$D/pow.sno" <<'SNO'
        I = 0
LI      I = I + 1
        GT(I, 20)                                   :S(END)
        X = (1.0 * I) / 7
        N = 0
LN      N = N + 1
        GT(N, 40)                                   :S(LI)
        OUTPUT = X ** N
        OUTPUT = X ** (N / 3.0)
        OUTPUT = (-X) ** N
        OUTPUT = (-X) ** (1.0 * N)
        OUTPUT = I ** (-N)
        OUTPUT = (-I) ** (-N)
        OUTPUT = X ** (-N)
        OUTPUT = I ** (N / 7.0)                     :(LN)
END
SNO
cat > "$D/powerr.sno" <<'SNO'
        &ERRLIMIT = 20
        Z = 0.0
        A = Z ** Z
        OUTPUT = 'a ' &ERRTYPE
        B = -2.0
        A = B ** 1.5
        OUTPUT = 'b ' &ERRTYPE
        T = 10.0
        A = T ** 400
        OUTPUT = 'c ' &ERRTYPE
        A = T ** 400.0
        OUTPUT = 'd ' &ERRTYPE
        A = B ** 1.0E30
        OUTPUT = 'e ' &ERRTYPE
        OUTPUT = B ** 3.0
        OUTPUT = B ** -3
        OUTPUT = Z ** -2.5
        OUTPUT = T ** 0
        S = &STLIMIT
        OUTPUT = S ** 1.5
        OUTPUT = 'end ' &ERRLIMIT
END
SNO
cat > "$D/match.sno" <<'SNO'
        R = 0.0
        T = 2.0
        X = '0.'
        X ? R                        :S(A1)F(B1)
A1      OUTPUT = 'var real pattern matches 0.'  :(C1)
B1      OUTPUT = 'var real pattern fails on 0.'
C1      '0' ? R                      :S(A2)F(B2)
A2      OUTPUT = 'var real pattern matches 0'   :(C2)
B2      OUTPUT = 'var real pattern fails on 0'
C2      R ? '0.'                     :S(A3)F(B3)
A3      OUTPUT = 'real subject matches 0.'      :(C3)
B3      OUTPUT = 'real subject fails 0.'
C3      '2.' ? *T                    :S(A4)F(B4)
A4      OUTPUT = 'deferred real matches 2.'     :(C4)
B4      OUTPUT = 'deferred real fails 2.'
C4      '2' ? *T                     :S(A5)F(B5)
A5      OUTPUT = 'deferred real matches 2'      :(C5)
B5      OUTPUT = 'deferred real fails 2'
C5      OUTPUT = 'concat ' R 'z'
        OUTPUT = 'size ' SIZE(R) ' ' SIZE(2.5) ' ' SIZE(T * 1.0E20)
        OUTPUT = 'dupl ' DUPL(T, 2)
END
SNO
cat > "$D/trace.sno" <<'SNO'
        DEFINE('F(A)')
        TRACE('X','VALUE')
        TRACE('F','CALL')
        TRACE('F','RETURN')
        &TRACE = 100
        X = 3.0
        X = 1.1 / 1.5
        F(3.0)                        :(END)
F       F = A * 2.0                   :(RETURN)
END
SNO
ARMS="conv pow powerr match trace"
run_scrip() {  # $1 name $2 mode -> stdout
    if [ "$2" = m3 ]; then (cd "$D" && timeout 60 "$B/scrip" --stlimit "$1.sno" < /dev/null 2>/dev/null)
    else "$B/scrip" --stlimit --compile -o "$D/$1.s" "$D/$1.sno" </dev/null >/dev/null 2>&1 \
           && gcc -no-pie -o "$D/$1.bin" "$D/$1.s" -L"$B/out" -lscrip_rt -lm -Wl,-rpath,"$B/out" 2>/dev/null \
           || refuse "could not build the m4 arm of $1 -- a toolchain failure, not a verdict"
         (cd "$D" && timeout 60 "./$1.bin" < /dev/null 2>/dev/null); fi
}
for a in $ARMS; do (cd "$D" && timeout 60 "$SBL" -bf "$a.sno" < /dev/null 2>/dev/null | grep -v '^$' > "$a.sbl"); done
[ "$(wc -l < "$D/conv.sbl")" = 34920 ] || refuse "the oracle printed $(wc -l < "$D/conv.sbl") of 34920 conversion lines -- the witness is not measuring"
[ "$(wc -l < "$D/pow.sbl")" = 6400 ] || refuse "the oracle printed $(wc -l < "$D/pow.sbl") of 6400 power lines -- the witness is not measuring"
grep -qx "0.694444444444445e+18" "$D/conv.sbl" || refuse "the oracle no longer prints (1/12)**2 * 10**20 as 0.694444444444445e+18 (C's rounding says ...444) -- re-read sbl.min gts10-gts28"
grep -qx "99516432313703.9" "$D/powerr.sbl" && grep -qx "b 311" "$D/powerr.sbl" || refuse "the oracle's exp(y * ln x) witness or its 311 changed -- re-read sbl.min o_exp"
grep -qx "var real pattern fails on 0" "$D/match.sbl" || refuse "the oracle no longer fails '0' ? R for R = 0.0 -- re-read sbl"
grep -q "F(3\.)" "$D/trace.sbl" || refuse "the oracle's trace no longer spells F(3.) -- re-read sbl"
fails=0; arms=0
for m in m3 m4; do
    for a in $ARMS; do
        arms=$((arms+1)); run_scrip "$a" "$m" | grep -v '^$' > "$D/$a.$m"
        if cmp -s "$D/$a.sbl" "$D/$a.$m"; then printf '  ok    %s %-7s %s lines, as sbl\n' "$m" "$a" "$(wc -l < "$D/$a.sbl")"
        else printf '  FAIL  %s %-7s %s of %s lines differ\n' "$m" "$a" "$(diff "$D/$a.sbl" "$D/$a.$m" | grep -c '^>')" "$(wc -l < "$D/$a.sbl")"; diff "$D/$a.sbl" "$D/$a.$m" | head -6 | sed 's/^/          /'; fails=$((fails+1)); fi
    done
done
[ "$fails" -eq 0 ] || { echo "⛔ GATE FAILED: $fails of $arms arm(s) red."
    echo "   conv: string_ops.c real_str is no longer sbl.min's gts10-gts28; pow/powerr: arithmetic.c rt_sno_pow is no longer o_exp;"
    echo "   match: a real reached a match through a conversion other than real_str (pattern_match.c rt_defer_close, bn_size);"
    echo "   trace: core.c trace_spell_value."; exit 1; }
echo "✅ GATE OK: $arms arm(s) -- a real converts to text by sbl's gts routine (34,920 values) and exponentiates by o_exp (6,400 values and its errors), in a match, SIZE and TRACE too, both modes"
exit 0
