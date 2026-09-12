#!/usr/bin/env bash
# test_gate_logtalk_population_is_named_and_sums.sh — the invariant gate for the logtalk_iso instrument
# (row prolog-logtalk-iso-3268-conformance-cases-have-no-runner, hq_R 2026-09-12).
#
# ⛔ WHAT IT PROTECTS, AND IT IS NOT THE SCORE. The number on that board will move every week and must be
# free to. What must never move is the three properties that make the number mean anything:
#   (1) THE POPULATION IS WHOLE — every case in every tests.lgt is in exactly one bucket, and the buckets
#       reconcile with the count that was globbed. A case in no bucket is invisible in every number, which
#       is the "never graded business" the package lockdown exists to end.
#   (2) A FILE THAT CANNOT BE PARSED REFUSES rc=2 AND IS NAMED — never dropped, never a shorter denominator.
#   (3) THE ISO HEX ESCAPE IS READ CORRECTLY. This is a REGRESSION PIN on a measured 31-case hole, not a
#       hypothetical: `{writeq('\x2\')}` ends its atom with a backslash (ISO 6.4.2.1), every "backslash
#       escapes the next character" scanner ate the closing quote, and predicates/write_term_3 silently
#       lost cases 115..145. ⭐ It lost them in the file whose whole subject is escapes — a suite is most
#       likely to break the instrument reading it at exactly the construct that suite exists to test — and
#       it lost them WITHOUT AN ERROR, so only a second, independent count could see it.
#
# ⭐ EVERY ARM RUNS ON A SCRATCH FIXTURE IN mktemp, NOT ON THE VENDORED SUITE. A gate that needs the real
# 3617-case run to fire costs two and a half minutes and nobody puts it in make test; these cost under a
# second and are offline. The vendored suite is graded by its runner, which is a board, not a gate.
#
# Usage: bash scripts/test_gate_logtalk_population_is_named_and_sums.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GATE=test_gate_logtalk_population_is_named_and_sums
violations=0; arms=0
TD="$(mktemp -d "${TMPDIR:-/tmp}/lgtgate.XXXXXX")"; trap 'rm -rf "$TD"' EXIT
fail() { echo "GATE FAIL [$GATE]: $*"; violations=$((violations+1)); }

for f in util_logtalk_extract.py util_logtalk_grade.py lib_logtalk_lgtunit.pl test_prolog_logtalk_suite.sh; do
    [ -f "$HERE/$f" ] || { echo "⛔ REFUSED(2) [$GATE]: $HERE/$f is missing -- this gate cannot measure an instrument that is not here"; exit 2; }
done

# ---- ARM 1 — the population is whole, and the parser agrees with an independent line count.
arms=$((arms+1))
mkdir -p "$TD/a/grp"
cat > "$TD/a/grp/tests.lgt" <<'LGT'
support(1).
:- object(tests, extends(lgtunit)).
	test(t_true, true) :-
		{support(1)}.
	test(t_false, false) :-
		{support(2)}.
	test(t_err, error(type_error(evaluable, nope/0))) :-
		{_X is nope}.
	succeeds(t_legacy_succ) :-
		{support(1)}.
	fails(t_legacy_fail) :-
		{support(2)}.
	throws(t_legacy_throw, error(type_error(evaluable, nope/0), _)) :-
		{_Y is nope}.
	helper(ok).
:- end_object.
LGT
out="$(python3 "$HERE/util_logtalk_extract.py" --census "$TD/a" 2>&1)"; rc=$?
case "$out" in
    *"files=1 cases=6"*) : ;;
    *) fail "ARM 1: the six head forms did not census as 6 cases (rc=$rc): $out" ;;
esac

# ---- ARM 2 — a clause the object body defines is DATABASE, not a dropped line. 373 false reds rode on this.
arms=$((arms+1))
got="$(GATE_SCRIPTS="$HERE" python3 - "$TD/a/grp/tests.lgt" <<'PY'
import sys, os
sys.path.insert(0, os.environ["GATE_SCRIPTS"])
import util_logtalk_extract as ex
print(" ".join(sorted(c.split("(")[0] for c in ex.parse_file(sys.argv[1]).db)))
PY
)"
[ "$got" = "helper support" ] || fail "ARM 2: the database is '$got', want 'helper support' -- a support predicate defined INSIDE the object was dropped, which turns every case that calls it into a plausible existence_error red the harness manufactured"

# ---- ARM 3 — THE ISO HEX ESCAPE REGRESSION PIN. The three cases after it must still be seen.
arms=$((arms+1))
mkdir -p "$TD/b/grp"
{
  printf ':- object(tests, extends(lgtunit)).\n'
  printf '\ttest(esc_hex, true) :-\n\t\t{writeq('"'"'\\x2\\'"'"')}.\n'
  printf '\ttest(esc_oct, true) :-\n\t\t{writeq('"'"'\\101\\'"'"')}.\n'
  printf '\ttest(after_the_escapes, true) :-\n\t\t{true}.\n'
  printf ':- end_object.\n'
} > "$TD/b/grp/tests.lgt"
out="$(python3 "$HERE/util_logtalk_extract.py" --census "$TD/b" 2>&1)"
case "$out" in
    *"files=1 cases=3"*) : ;;
    *) fail "ARM 3: an ISO numeric escape (\\x2\\ / \\101\\) swallowed the cases after it -- census said: $out
    This is the measured 31-case hole in predicates/write_term_3. A scanner that treats backslash as 'skip the next character' lands ON the escape's terminating backslash and eats the closing quote." ;;
esac

# ---- ARM 4 — an unparseable case REFUSES rc=2 and NAMES the file. Never a shorter denominator.
arms=$((arms+1))
mkdir -p "$TD/c/grp"
{
  printf ':- object(tests, extends(lgtunit)).\n'
  printf '\ttest(fine, true) :-\n\t\t{true}.\n'
  printf '\ttest(unbalanced, true :-\n\t\t{true}.\n'
} > "$TD/c/grp/tests.lgt"
out="$(python3 "$HERE/util_logtalk_grade.py" --suite "$TD/c" --scrip "$HERE/../scrip" --modes m3 2>&1)"; rc=$?
if [ "$rc" -ne 2 ]; then
    fail "ARM 4: an unparseable tests.lgt exited $rc, want 2 -- a board that publishes over a file it could not read is publishing a denominator it invented"
elif ! printf '%s' "$out" | grep -q "grp/tests.lgt"; then
    fail "ARM 4: the refusal did not NAME the file it could not parse: $out"
fi

# ---- ARM 5 — the buckets reconcile with the globbed population, and the runner says so on its own line.
arms=$((arms+1))
if [ -x "$HERE/../scrip" ]; then
    out="$(python3 "$HERE/util_logtalk_grade.py" --suite "$TD/a" --scrip "$HERE/../scrip" --modes m3 2>&1)"; rc=$?
    pop="$(printf '%s' "$out" | sed -n 's/.*population=\([0-9]*\).*/\1/p' | head -1)"
    ident="$(printf '%s' "$out" | grep -c 'identity m3:.*== 6 ✓')"
    if [ "$rc" -ne 0 ] || [ "$pop" != "6" ] || [ "$ident" -ne 1 ]; then
        fail "ARM 5: the six-case fixture did not print a reconciling identity over population 6 (rc=$rc pop=$pop): $out"
    fi
else
    echo "    ARM 5 SKIPPED: no scrip binary at $HERE/../scrip -- the identity arm needs a compiler to grade with"
fi

# ---- ARM 6 — the supported-helper list is READ FROM THE SHIM, never a second list that can disagree.
arms=$((arms+1))
if grep -qE '^SUPPORTED[ =]|^SHIM_HELPERS[ =]' "$HERE/util_logtalk_grade.py"; then
    fail "ARM 6: util_logtalk_grade.py carries a typed list of supported helpers. Two lists that can disagree about what is implemented is the same defect as two counts that can disagree about a population -- read lib_logtalk_lgtunit.pl."
fi
n="$(GATE_SCRIPTS="$HERE" python3 - <<'PY'
import os, sys
sys.path.insert(0, os.environ["GATE_SCRIPTS"])
import util_logtalk_grade as g
print(len(g.shim_helpers()))
PY
)"
[ "${n:-0}" -ge 15 ] || fail "ARM 6: shim_helpers() read $n names out of lib_logtalk_lgtunit.pl -- an empty or tiny set silently marks every helper-using case UNGRADED and the board still reconciles"

# ---- ARM 7 — A DATABASE CLAUSE IN LOGTALK-ONLY SYNTAX IS DROPPED, AND THE FILE STILL GRADES. Measured
# 2026-09-12 against the ceo's independent emulation: one `cleanup :- ^^clean_text_input.` helper travels
# with every case of its file, so the generated program was a PARSE ERROR and all 49 predicates/is_2 cases
# came back `nooutput` -- a verdict on the harness wearing the shape of an arithmetic red. 40 files and
# 1486 of 3617 cases were graded that way. ⭐ The defect could only ever produce reds, which is why it did
# not look like a defect: a board of failures is what a young frontend is expected to print.
arms=$((arms+1))
mkdir -p "$TD/d/grp"
cat > "$TD/d/grp/tests.lgt" <<'LGT'
cleanup :-
	^^clean_text_input.
support(1).
:- object(tests, extends(lgtunit)).
	test(t_plain, true) :-
		{support(1)}.
:- end_object.
LGT
if [ -x "$HERE/../scrip" ]; then
    out="$(python3 "$HERE/util_logtalk_grade.py" --suite "$TD/d" --scrip "$HERE/../scrip" --modes m3 2>&1)"; rc=$?
    case "$out" in
        *"m3_pass=1"*) : ;;
        *) fail "ARM 7: a file whose database carries a Logtalk-only helper (^^/::) did not grade its plain case (rc=$rc): $out
    The clause cannot be expressed in Prolog, so it must be DROPPED from the generated program -- not emitted, where it makes every case of that file a parse error and reports the construct under test as the failure." ;;
    esac
else
    echo "    ARM 7 SKIPPED: no scrip binary at $HERE/../scrip"
fi

# ---- ARM 8 — THE TESTER-LOADED PROLOG FILE IS INLINED, GUARDS INTACT. Seven directories ship a tester.lgt
# whose initialization loads a plain-Prolog file before the tests run; every predicate the cases call lives
# there. ⛔ It goes in VERBATIM: drop its :- if/endif guards while keeping both branches and the 31 cases
# that exist to prove a branch was NOT compiled all start succeeding -- the harness answering the question
# the suite was asking.
arms=$((arms+1))
mkdir -p "$TD/e/grp"
cat > "$TD/e/grp/file.pl" <<'PL'
:- dynamic(gated_off/0).
:- if(true).
gated_on.
:- endif.
:- if(fail).
gated_off.
:- endif.
PL
cat > "$TD/e/grp/tester.lgt" <<'LGT'
:- initialization((
	logtalk_load_context(directory, Directory),
	atom_concat(Directory, 'file.pl', File),
	'$lgt_load_prolog_file'(File),
	tests::run
)).
LGT
cat > "$TD/e/grp/tests.lgt" <<'LGT'
:- object(tests, extends(lgtunit)).
	test(kept_branch_is_defined, true) :-
		{gated_on}.
	test(dropped_branch_is_not, false) :-
		{gated_off}.
:- end_object.
LGT
if [ -x "$HERE/../scrip" ]; then
    out="$(python3 "$HERE/util_logtalk_grade.py" --suite "$TD/e" --scrip "$HERE/../scrip" --modes m3 2>&1)"; rc=$?
    case "$out" in
        *"m3_pass=2"*) : ;;
        *) fail "ARM 8: the file the directory's tester.lgt loads was not inlined with its conditional-compilation guards intact (rc=$rc): $out
    Without it every case raises existence_error on a predicate that is defined -- and with its guards dropped, the branch the suite proves was NOT compiled becomes defined and the case wrongly passes." ;;
    esac
else
    echo "    ARM 8 SKIPPED: no scrip binary at $HERE/../scrip"
fi

# ---- ARM 9 — A DEVELOPMENT AID THAT GRADED NOTHING REFUSES. `--group predicates/sub_atom_5` (the path,
# where the group is the basename) matched no file and the grader printed a complete board reading
# population=0, identity 0 == 0 ✓, AND per case 0/0: the success shape over an empty measurement, which is
# the one thing every instrument in this tree is forbidden to do.
arms=$((arms+1))
out="$(python3 "$HERE/util_logtalk_grade.py" --suite "$TD/a" --scrip "$HERE/../scrip" --modes m3 --group no/such/group 2>&1)"; rc=$?
if [ "$rc" -ne 2 ]; then
    fail "ARM 9: --group matching no file exited $rc, want 2 -- it printed a board over a population of zero"
elif ! printf '%s' "$out" | grep -q "Known groups"; then
    fail "ARM 9: the refusal did not name the groups it does know: $out"
fi

echo "[$GATE] arms=$arms violations=$violations"
[ "$violations" -eq 0 ] || exit 1
echo "GATE PASS [$GATE]: the population is whole, an unreadable file refuses and is named, the ISO numeric escape stays readable, Logtalk-only database clauses are dropped rather than emitted, the tester-loaded Prolog file is inlined with its guards, and a group that matched nothing refuses"
exit 0
