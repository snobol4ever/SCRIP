#!/usr/bin/env bash
# test_gate_master_resort_preserves_modes.sh -- the invariant gate for the `modes` column across a master
# rewrite (hq_T 2026-09-05, on hq_B's measurement of corpus a6e836ea6).
#
# WHAT IT PROTECTS.  `modes` is what corpus_suite_harness.py GRADES BY: `ast` means diff `--dump-ast` output,
# `m3,m4` means run the program, and UNKNOWN is run-graded.  Every CSV writer in util_build_master_suite.py
# derived the column as `modes_decl.get(fam, "UNKNOWN")`, so a rewrite DISCARDED the recorded per-entry value
# and every entry of an UNDECLARED family became UNKNOWN.  MEASURED: snobol4 ast 28 -> 0, which is the master
# board's FAIL=26/21 against a recorded FAIL=0; pascal ast 5 -> 0 and 40 ladder rungs losing m3,m4; prolog
# GAINING 134 bogus `ast`.  A resort commit asserted content-invariance and was telling the truth about what
# it checked -- entry set and body bytes -- and `modes` was in neither.
#
# ⛔⭐ ARM 2 IS THE LOAD-BEARING ONE AND ARM 3 IS WHAT MAKES IT MEAN ANYTHING: arm 2 asserts an undeclared
# family's recorded value survives, and arm 3 evaluates the OLD expression on the same inputs to show it
# returned UNKNOWN.  Without arm 3, arm 2 could pass on a fixture that never had a value to lose.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILDER="$HERE/util_build_master_suite.py"
fail=0
ck() { if [ "$1" = ok ]; then echo "  ✅ $2"; else echo "  ⛔ $2"; fail=$((fail+1)); fi; }
[ -f "$BUILDER" ] || { echo "⛔ REFUSE(2): no util_build_master_suite.py beside this gate at $BUILDER"; exit 2; }
grep -q '_make_resort_modes' "$BUILDER" || { echo "⛔ REFUSE(2): $BUILDER has no _make_resort_modes -- this gate is measuring a different tool than it was written for"; exit 2; }
out=$(HERE="$HERE" python3 - <<'PY'
import importlib.util, os, sys
spec = importlib.util.spec_from_file_location("ubms", os.path.join(os.environ["HERE"], "util_build_master_suite.py"))
m = importlib.util.module_from_spec(spec)
sys.argv = ["ubms", "--help-nothing"]
try:
    spec.loader.exec_module(m)
except SystemExit:
    pass
mk = m._make_resort_modes
recorded = {"parser_x": "ast", "runner_y": "m3,m4", "orphan_z": ""}
decl = {"declared_fam": "m3,m4"}
f = mk(recorded, decl)
print("A1", f("anything", "declared_fam"))
print("A2", f("parser_x", "undeclared_fam"))
print("A2b", f("runner_y", "undeclared_fam"))
print("A3", f("orphan_z", "undeclared_fam"))
print("A4", f("never_seen", "undeclared_fam"))
print("OLD", decl.get("undeclared_fam", "UNKNOWN"))
PY
) || { echo "⛔ REFUSE(2): could not load the builder module: $out"; exit 2; }
g() { printf '%s\n' "$out" | awk -v k="$1" '$1==k{$1=""; sub(/^ /,""); print}'; }
echo "--- ARM 1: a DECLARED family still wins (modes is a DECLARED field, never derived) ---"
ck "$([ "$(g A1)" = "m3,m4" ] && echo ok || echo no)" "declared family -> its declaration (got '$(g A1)')"
echo "--- ARM 2 (LOAD-BEARING): an UNDECLARED family's RECORDED per-entry value is carried forward ---"
ck "$([ "$(g A2)" = "ast" ] && echo ok || echo no)"     "recorded ast survives an undeclared family (got '$(g A2)')"
ck "$([ "$(g A2b)" = "m3,m4" ] && echo ok || echo no)"  "recorded m3,m4 survives an undeclared family (got '$(g A2b)')"
echo "--- ARM 3 (the control): the OLD expression returned UNKNOWN for those same inputs ---"
ck "$([ "$(g OLD)" = "UNKNOWN" ] && echo ok || echo no)" "modes_decl.get(fam,'UNKNOWN') on an undeclared family is UNKNOWN, so ARM 2 is a real discrimination (got '$(g OLD)')"
echo "--- ARM 4: UNKNOWN is still the honest answer when there is nothing to carry ---"
ck "$([ "$(g A3)" = "UNKNOWN" ] && echo ok || echo no)"  "an empty recorded value does not masquerade as a declaration (got '$(g A3)')"
ck "$([ "$(g A4)" = "UNKNOWN" ] && echo ok || echo no)"  "an entry absent from the CSV is UNKNOWN, never guessed (got '$(g A4)')"
echo "--- ARM 5: --resort still carries the order-only REFUSAL for a modes change ---"
if grep -q 'REFUSED: --resort would change the `modes` column' "$BUILDER"; then
    ck ok "resort refuses rather than repairing -- changing what a suite is graded by is not a reorder"
else
    ck no "the --resort modes-invariance refusal is gone from the builder"
fi
echo "--- ARM 6: no CSV writer in the builder derives modes from the family alone any more ---"
n=$(grep -c 'modes_decl.get(fam, "UNKNOWN")\]' "$BUILDER")
ck "$([ "$n" = 0 ] && echo ok || echo no)" "zero remaining derive-and-discard writerow sites (found $n)"
[ "$fail" = 0 ] && { echo "✅ GATE OK: a master rewrite carries the modes column it was handed"; exit 0; }
echo "⛔ GATE RED: $fail arm(s) failed"; exit 1
