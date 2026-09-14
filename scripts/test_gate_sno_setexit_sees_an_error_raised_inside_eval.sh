#!/usr/bin/env bash
# test_gate_sno_setexit_sees_an_error_raised_inside_eval.sh -- a SETEXIT handler is told about an error raised
# inside an EVAL'd expression BEFORE the EVAL guard turns that error into a plain failure.
#
# ⛔ THE DEFECT (ceo 2026-09-14, the x64 `setexit` program, which is the suite's own witness for this). Two
# catchers were armed over the same error and the wrong one won. core_runtime_error's FIRST block longjmps to
# whatever errjmp guard is armed -- and eval_chain_run_guarded arms one for the duration of every EVAL -- so an
# error inside an EVAL was converted to a failure and the program's SETEXIT handler NEVER RAN. Measured:
# setexit.sbl printed only `oops` where sbl -bf prints `trace`, `>> error #262, division caused real overflow`,
# then `oops`. SPITBOL's order is handler first; ours was guard first.
#
# ⭐ THE ORDER IS THE WHOLE FIX AND IT IS VISIBLE IN THE OUTPUT: with the handler first, it prints its two
# lines, `:s(CONTINUE)` resumes through the same setjmp the guard uses, the division then fails, EVAL fails, and
# the statement's `:f()` branch prints `oops`. All three lines, in SPITBOL's order.
#
# ⛔ THE ARM THAT KEEPS THIS HONEST IS THE THIRD LINE, NOT THE FIRST TWO: a change that raised the error past
# the guard and never came back would print `trace` and the error and then lose the program. `oops` is the
# proof that CONTINUE still resumes and the EVAL still fails.
#
# rc=0 clean · rc=1 the handler was skipped or the resume was lost · rc=2 REFUSAL (cannot measure).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="$HERE/.."
[ -x "$B/scrip" ] || { echo "⛔ REFUSES rc=2: $B/scrip is not built -- cannot measure"; exit 2; }
D="$(mktemp -d)" || { echo "⛔ REFUSES rc=2: no scratch dir"; exit 2; }
trap 'rm -rf "$D"' EXIT

cat > "$D/w.sno" <<'SNO'
    SETEXIT(.errtrace)
    &ERRLIMIT = 100
    n1 = 1.0
    n0 = 0.1e-308
    r = EVAL('n1 / n0') :s(END)f(oops)
oops
    OUTPUT = "oops"       :(END)
errtrace
    OUTPUT = 'trace'
    OUTPUT = ">> error #" &ERRTYPE ", " &ERRTEXT
    SETEXIT(.errtrace)
    (DIFFER(&ERRTYPE,20) DIFFER(&ERRTYPE,23)) :s(CONTINUE)f(ABORT)
END
SNO
cat > "$D/w.ref" <<'REF'
trace
>> error #262, division caused real overflow
oops
REF

fails=0
for mode in m3 m4; do
    if [ "$mode" = m3 ]; then
        (cd "$D" && timeout 20 "$B/scrip" w.sno </dev/null > "out.$mode" 2>&1)
    else
        "$B/scrip" --compile -o "$D/w.s" "$D/w.sno" </dev/null >/dev/null 2>&1 \
          && as -o "$D/w.o" "$D/w.s" 2>/dev/null \
          && gcc -o "$D/w.bin" "$D/w.o" "$B/out/libscrip_rt.so" -Wl,-rpath,"$B/out" 2>/dev/null \
          || { echo "⛔ REFUSES rc=2: could not build the m4 arm -- a toolchain failure, not a verdict"; exit 2; }
        (cd "$D" && timeout 20 ./w.bin </dev/null > "out.$mode" 2>&1)
    fi
    if diff -q "$D/w.ref" "$D/out.$mode" >/dev/null 2>&1; then
        printf '  ok    %s: the handler runs, names the error, and CONTINUE still lets the EVAL fail\n' "$mode"
    else
        printf '  FAIL  %s:\n' "$mode"; diff "$D/w.ref" "$D/out.$mode" | sed 's/^/          /'; fails=$((fails+1))
    fi
done

[ "$fails" -eq 0 ] || { echo "⛔ GATE FAILED: $fails of 2 mode(s) red."
                        echo "   ⛔ ONLY 'oops' MEANS THE EVAL GUARD SWALLOWED THE ERROR before SETEXIT saw it -- two catchers"
                        echo "      over one error, and the wrong one won."
                        echo "   ⛔ 'trace' AND THE ERROR WITHOUT 'oops' MEANS THE OPPOSITE MISTAKE: the handler ran and the"
                        echo "      resume was lost, so the program never came back to fail the statement."
                        exit 1; }
echo "✅ GATE OK: SETEXIT sees an error raised inside EVAL, and CONTINUE still returns to fail the statement, both modes"
exit 0
