#!/usr/bin/env bash
# test_gate_pl_wall_us_arity1.sh -- CEO-567's precondition: a Prolog kernel must be able to SELF-TIME.
# by_name_dispatch.c registers wall_us/wall_ms at nargs==0 only (function style, the SNOBOL4/Icon
# calling convention). Prolog calls wall_us(T) at ARITY 1 -- a predicate unifying an output argument --
# so every call raises existence_error(procedure,wall_us/1). prelude_swipl.pl's own header states the
# assumption that went unverified: it exists "to supply the self-timing hooks SCRIP has as builtins".
# Under the kernel convention (RULES.md FACT RULE -- THE KERNEL CONVENTION) every kernel self-times, so
# this one shape mismatch blocks the entire Prolog speed axis; it was measured live on 10 of the 23
# corpus/benchmarks/prolog/bench/ kernels, all raising the identical existence_error.
# ⛔ THE ARITY-0 FORM MUST KEEP ANSWERING: SNOBOL4 and Icon call wall_us with no arguments and this gate
# asserts the new leaf did not displace them -- a cure that moves the builtin instead of widening it
# trades one silent breakage for another.
# ⛔ REFUSES rc=2 (never rc=1) when it cannot measure: no ./scrip, no runtime .so, or a witness that
# neither succeeds nor produces the known defect signature -- "could not measure" is not "clean".
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="$HERE/../scrip"
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE(2): no ./scrip at $SCRIP"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cat > "$W/w.pl" <<'EOF'
:- initialization(main).
main :- wall_us(U), wall_ms(M), ( integer(U) -> write(us_ok) ; write(us_bad) ), nl,
        ( integer(M) -> write(ms_ok) ; write(ms_bad) ), nl.
EOF
fail=0; examined=0
for mode in m3 m4; do
  examined=$((examined+1))
  if [ "$mode" = m3 ]; then out="$(timeout 20 "$SCRIP" "$W/w.pl" </dev/null 2>&1)"
  else out="$(timeout 30 "$SCRIP" --compile -o "$W/w.s" "$W/w.pl" </dev/null 2>&1 && cd "$W" && gcc -no-pie w.s -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -o w.bin 2>&1 && timeout 20 ./w.bin </dev/null 2>&1)"; fi
  if printf '%s' "$out" | grep -q 'existence_error(procedure,wall_us/1)'; then
    echo "  FAIL  $mode -- wall_us/1 does not exist: Prolog cannot self-time, so the speed axis cannot be measured at all"; fail=$((fail+1))
  elif printf '%s' "$out" | grep -q 'us_ok' && printf '%s' "$out" | grep -q 'ms_ok'; then
    echo "  ✅ OK  $mode -- wall_us(T) and wall_ms(T) both unify T with an integer"
  else
    echo "⛔ REFUSED-TO-GRADE(2): $mode witness neither self-timed nor produced the known defect -- got: $(printf '%s' "$out" | head -2 | tr '\n' '|')"; exit 2
  fi
done
cat > "$W/a0.sno" <<'EOF'
	t = wall_us()
	OUTPUT = IDENT(DATATYPE(t),'INTEGER') 'arity0_ok'
END
EOF
examined=$((examined+1))
a0="$(timeout 20 "$SCRIP" "$W/a0.sno" </dev/null 2>&1)"
if printf '%s' "$a0" | grep -q 'arity0_ok'; then echo "  ✅ OK  arity-0 wall_us() still answers for SNOBOL4/Icon"
else echo "  FAIL  the arity-0 form stopped answering -- the cure displaced the builtin instead of widening it"; fail=$((fail+1)); fi
echo "------------------------------------------------------------"
[ "$examined" -eq 3 ] || { echo "⛔ REFUSED-TO-GRADE(2): examined=$examined, expected 3"; exit 2; }
if [ "$fail" -eq 0 ]; then echo "✅ GATE OK: 3 of 3 -- Prolog kernels can self-time and the arity-0 form is intact"; exit 0; fi
echo "⛔ GATE FAIL: $fail of $examined check(s) failed"; exit 1
