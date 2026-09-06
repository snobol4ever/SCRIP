#!/usr/bin/env bash
# test_gate_raku_paren_call_passes_its_arguments.sh -- a parenthesized Raku call must pass its ARGUMENTS,
# never one concatenated list (hq_T 2026-09-06, CEO-332 / Lon: "let's not see stuck ever again ... go fix ONE
# program in each and let's begin the climb").
#
# ⛔ THE DEFECT, measured on roast S04-statements/until.t. `is($i, 5, 'desc')` built
#     (TT_FNC __rk_test_is (TT_VAR __rk_test_is) (TT_FNC __rk_arr (TT_VAR __rk_arr) (TT_VAR i) (TT_ILIT 5) ...))
# -- ONE argument, a list -- while `is $i, 5, 'desc'` built the correct three. The grammar HAS a
# `TESTOP '(' arg_list ')'` rule; it is unreachable for a comma list, because `TESTOP arg_list` also matches
# with arg_list being a single parenthesized list expression, and bison resolves it that way. The .y author's
# intent was right and the rule is dead.
#
# ⛔⭐ WHY IT NEEDED A GATE RATHER THAN A BOARD. THE TEST STILL EMITTED VALID TAP. It printed its plan, it
# printed four `not ok` lines with a diagnostic, it exited 0 -- everything a harness looks at was well formed,
# and the only evidence was inside the message text ("got: '55until $i >= 5 {} works'", three arguments
# concatenated). A whole-file board can only ever say 986 to 985. Nothing anywhere would have named the CALL
# FORM. And it is a construct so basic that a reader scanning for it does not see it: every roast file calls
# testops both ways on adjacent lines, so the working form sits beside the broken one on every page.
#
# ⭐ THE ARMS ARE DIFFERENTIAL, AND THAT IS WHAT MAKES THEM CHEAP AND ORACLE-FREE. The property is not "is()
# prints X" -- which would need a hand-authored expectation that can rot -- it is THE TWO CALL FORMS OF THE
# SAME CALL MUST AGREE. f(a, b, c) and f a, b, c are the same program in Raku, so any divergence is a defect
# without anyone having to say which side is right. A hand-written expected value could have been written to
# match the buggy output; this cannot.
#
# ⛔ AND ONE ABSOLUTE ARM, because a differential gate alone is satisfiable by breaking BOTH forms: the arity
# actually reaching the callee is asserted directly, so "they agree" cannot become "they agree at 1 argument".
#
# FAILED ONCE: every arm below was run against the pre-cure binary and arms 1-4 FAILED there (m3 and m4 both:
# got '55paren form' where the listop form printed 'ok 1 - paren form'); arm 5 passed before and after, which
# is the control that says the cure did not simply delete the listop path.
#
# Usage: bash scripts/test_gate_raku_paren_call_passes_its_arguments.sh   (~3s, no oracle, no network)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
GATE_NAME="raku_paren_call_passes_its_arguments"
gate_parse_args "$@"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
violations=0; examined=0
ck() { examined=$((examined + 1)); if [ "$1" = ok ]; then printf '  ok   %s\n' "$2"; else printf '  FAIL %s\n' "$2"; violations=$((violations + 1)); fi; }
# run_both <file> -> prints "m3<TAB>m4" md5s; a mode that will not build prints BUILDFAIL, never an empty
# string, because two empty strings compare equal and would read as agreement.
run_both() {
    local f="$1" o3 o4
    o3="$(timeout 10 "$ROOT/scrip" --run "$f" 2>&1 </dev/null | md5sum | cut -d' ' -f1)"
    if timeout 20 "$ROOT/scrip" --compile --target=x86 "$f" > "$W/c.s" 2>/dev/null \
       && timeout 20 gcc -c "$W/c.s" -o "$W/c.o" 2>/dev/null \
       && timeout 20 gcc "$W/c.o" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/c.bin" 2>/dev/null; then
        o4="$(timeout 10 "$W/c.bin" 2>&1 </dev/null | md5sum | cut -d' ' -f1)"
    else
        o4="BUILDFAIL"
    fi
    printf '%s\t%s\n' "$o3" "$o4"
}
echo "--- ARMS 1-4 — the two call forms of one testop must agree, in BOTH modes ---"
for t in "is \$i, 5" "ok 1" "is 'a', 'a'" "isnt 1, 2"; do
    printf 'use Test;\nplan 1;\nmy $i = 5;\n%s, "d";\n' "$t"       > "$W/listop.raku"
    printf 'use Test;\nplan 1;\nmy $i = 5;\n%s(%s, "d");\n' "${t%% *}" "${t#* }" > "$W/paren.raku"
    a="$(run_both "$W/listop.raku")"; b="$(run_both "$W/paren.raku")"
    l3="${a%%	*}"; l4="${a##*	}"; p3="${b%%	*}"; p4="${b##*	}"
    [ "$l3" = "$p3" ] && ck ok  "m3 agree: $t" || ck fail "m3 DIVERGE: $t  (listop $l3 vs paren $p3)"
    [ "$l4" = "$p4" ] && ck ok  "m4 agree: $t" || ck fail "m4 DIVERGE: $t  (listop $l4 vs paren $p4)"
done
echo "--- ARM 5 — the ABSOLUTE arm: the arity that reaches the callee, so 'they agree' cannot mean 'both take one argument' ---"
printf 'use Test;\nplan 1;\nis(1, 1, "three args reach the callee");\n' > "$W/arity.raku"
out="$(timeout 10 "$ROOT/scrip" --run "$W/arity.raku" 2>&1 </dev/null)"
printf '%s\n' "$out" | grep -qx 'ok 1 - three args reach the callee' \
    && ck ok "paren call delivers 3 distinct arguments" \
    || { ck fail "paren call did NOT deliver 3 arguments"; printf '%s\n' "$out" | sed 's/^/       /'; }
echo "--- ARM 6 — the AST carries no __rk_arr wrapper around a testop argument list ---"
# ⛔ IT GREPS THE WHOLE DUMP AND FIRST PROVES THE NODE IS THERE. Written as `| tail -1` this arm PASSED on the
# pre-cure binary -- the broken dump wrapped onto a second line, tail -1 returned "))", and "))" contains no
# __rk_arr, so the arm reported the cure present while every other arm reported it absent. A negative assertion
# over a truncated reading is indistinguishable from the property holding, which is this whole gate's own theme
# arriving inside the gate.
ast="$(timeout 10 "$ROOT/scrip" --dump-ast "$W/arity.raku" 2>&1 </dev/null)"
if ! printf '%s' "$ast" | grep -q '__rk_test_is'; then
    ck fail "the AST dump carries no __rk_test_is node at all -- this arm proved nothing"
    printf '%s\n' "$ast" | sed 's/^/       /'
elif printf '%s' "$ast" | grep -q '__rk_arr'; then
    ck fail "the argument list is still wrapped in __rk_arr"
    printf '%s\n' "$ast" | sed 's/^/       /'
else
    ck ok "testop node present and no __rk_arr wrapper"
fi
echo "--- ARM 7 — the roast program this cure flipped stays green in both modes ---"
RT="$ROOT/refs/roast/S04-statements/until.t"
if [ ! -f "$RT" ]; then
    echo "  ⛔ REFUSED(2) [$GATE_NAME]: no $RT -- refs/roast is per-root and gitignored; symlink it to"
    echo "     /home/resources/roast-master. A gate that cannot reach its witness REFUSES; it never passes."
    exit 2
fi
cp "$RT" "$W/until.raku"
for m in 3 4; do
    if [ "$m" = 3 ]; then got="$(timeout 10 "$ROOT/scrip" --run "$W/until.raku" 2>&1 </dev/null)"
    else
        if timeout 20 "$ROOT/scrip" --compile --target=x86 "$W/until.raku" > "$W/u.s" 2>/dev/null \
           && timeout 20 gcc -c "$W/u.s" -o "$W/u.o" 2>/dev/null \
           && timeout 20 gcc "$W/u.o" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/u.bin" 2>/dev/null; then
            got="$(timeout 10 "$W/u.bin" 2>&1 </dev/null)"
        else got="BUILDFAIL"; fi
    fi
    n_ok="$(printf '%s\n' "$got" | grep -cE '^ok [0-9]+')"; n_not="$(printf '%s\n' "$got" | grep -cE '^not ok')"
    { [ "$n_ok" -eq 4 ] && [ "$n_not" -eq 0 ]; } \
        && ck ok "m$m S04-statements/until.t: 4 ok, 0 not ok" \
        || { ck fail "m$m S04-statements/until.t: $n_ok ok, $n_not not ok"; printf '%s\n' "$got" | sed 's/^/       /'; }
done
GATE_EXAMINED="$examined"
gate_verdict "$violations" "violation(s)"
