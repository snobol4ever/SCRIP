#!/usr/bin/env bash
# test_prolog_rung13.sh — assertz/asserta: order, atom, compound, unify, static/dynamic mix
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPBIN="${HERE}/../scrip"
CORPUS="$S4E/corpus/tests/prolog"
FAMILY=rung13_assertz
SNO="$CORPUS/$FAMILY.pl"
REF="$CORPUS/$FAMILY.ref"
PASS=0; FAIL=0
[ -d "$CORPUS" ] || { echo "⛔ REFUSED-TO-GRADE: $CORPUS missing"; exit 2; }

echo "=== rung13: assertz/asserta (order/atom/compound/unify/static-dynamic-mix) ==="

# consolidated 2026-08-28 (tests-consolidate-prolog): ONE entry (assertz_unify, deterministic,
# no backtracking) converted to the suite pair below -- delegate to `run`, which extracts and
# runs each entry alone (a suite file is never run whole, corpus_suite_harness.py's own
# docstring). The other 4 loose siblings (asserta_order, assertz_atom, assertz_compound,
# static_dynamic_mix) all share the same X, write(X), nl, fail. multi-solution shape and
# independently signal-11 crash -- PZ-4 (prolog-multiclause-uninit-lexprep-frame). They stay
# loose ON PURPOSE, not KEEP.md (this is not a permanent design choice, see task file
# tests-consolidate-prolog), and are DELIBERATELY KEPT in this script's own board below rather
# than dropped, so the crash stays visible instead of silently disappearing once PZ-4 lands.
if [ -f "$SNO" ] && [ -f "$REF" ]; then
    out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog --modes m3 2>&1)
    echo "$out" | grep -v '^SUITE_BOARD'
    board=$(echo "$out" | grep '^SUITE_BOARD')
    p=$(echo "$board" | grep -oP 'm3_pass=\K[0-9]+')
    f=$(echo "$board" | grep -oP 'm3_fail=\K[0-9]+')
    c=$(echo "$board" | grep -oP 'm3_crash=\K[0-9]+')
    h=$(echo "$board" | grep -oP 'm3_hang=\K[0-9]+')
    u=$(echo "$board" | grep -oP 'm3_unproven=\K[0-9]+')
    PASS=$((PASS+p)); FAIL=$((FAIL+f+c+h+u))
fi

# ⛔⭐⭐ ABSORBED ARM (hq_B 2026-08-30, hq_C's RETIRE-DO-NOT-RESTORE ruling). This family's witnesses
# were CONSOLIDATED into the one flat suite (corpus fdbe8ff8 verify+delete, THE ONE-FLAT-SUITE
# RULING) -- they were never lost, they were RENAMED, and every grader here kept looking for the
# old loose files and silently graded only the remainder. MEASURED: rung13 lost ALL FIVE and
# graded ZERO; rung14 kept 2 of 5; rung15 kept 4 of 5. Nine gradings were invisible.
# ⛔ KEYED ON THE `origin` COLUMN, NOT ON THE ENTRY NAME AND NOT ON THE FAMILY-NAME PREFIX.
# `origin` is the durable provenance link back to this rung and survives renames; the entry names
# did NOT (rung13_assertz -> assertz_directive_1, _assertz_atom -> _2, _assertz_compound -> _3,
# _static_dynamic_mix -> _4, _asserta_order -> asserta_assertz_directive_1). A census keyed on the
# old names reports the coverage as DELETED when it is intact -- that wrong-key search is exactly
# what produced a false alarm here, and the harness's own extract-family docstring is explicit that
# family membership comes from the CSV, never from a name convention.
ALLP="$CORPUS/ALL.pl"; ALLR="$CORPUS/ALL.ref"; ALLC="$CORPUS/ALL.csv"
if [ -f "$ALLP" ] && [ -f "$ALLR" ] && [ -f "$ALLC" ]; then
    _absdir="$(mktemp -d)"
    for _fam in $(python3 -c "import csv,sys; print(chr(10).join(sorted({r['family'] for r in csv.DictReader(open(sys.argv[1])) if r['origin'].startswith(sys.argv[2])})))" "$ALLC" "$FAMILY"); do
        python3 "$HERE/corpus_suite_harness.py" extract-family "$ALLP" "$ALLR" "$ALLC" "$_fam" "$_absdir/e.pl" "$_absdir/e.ref" >/dev/null 2>&1 || continue
        _out=$(timeout 120 python3 "$HERE/corpus_suite_harness.py" run "$_absdir/e.pl" "$_absdir/e.ref" --lang prolog --modes m3 2>&1)
        echo "$_out" | grep -v '^SUITE_BOARD' | sed "s/^/  [absorbed $_fam] /"
        _b=$(echo "$_out" | grep '^SUITE_BOARD')
        _p=$(echo "$_b" | grep -oP 'm3_pass=\K[0-9]+'); _f=$(echo "$_b" | grep -oP 'm3_fail=\K[0-9]+')
        _c=$(echo "$_b" | grep -oP 'm3_crash=\K[0-9]+'); _h=$(echo "$_b" | grep -oP 'm3_hang=\K[0-9]+')
        _u=$(echo "$_b" | grep -oP 'm3_unproven=\K[0-9]+')
        PASS=$((PASS+${_p:-0})); FAIL=$((FAIL+${_f:-0}+${_c:-0}+${_h:-0}+${_u:-0}))
    done
    rm -rf "$_absdir"
fi

shopt -s nullglob
for f in "$CORPUS"/rung13_assertz_*.pl; do
    ref="${f%.pl}.expected"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIPBIN" --run "$f" < /dev/null 2>/dev/null)
    rc=$?
    expected=$(cat "$ref")
    # ⛔ rc IS PART OF THE VERDICT, NOT JUST STDOUT -- a crash-after-correct-output program
    # (rung14_retract_all is the measured witness) prints its full expected text and THEN
    # segfaults; a stdout-only compare reads that as PASS. Same FACT RULE as RULES.md's
    # "a suite that ignores exit status cannot see a wrong exit status."
    if [ "$rc" -eq 0 ] && [ "$actual" = "$expected" ]; then
        echo "  PASS $(basename "$f")"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename "$f") (loose -- known PZ-4 prolog-multiclause-uninit-lexprep-frame, rc=$rc)"; FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"
# ⛔⭐ ZERO-GRADED IS A REFUSAL, NOT A PASS (hq_B 2026-08-30). BOTH witness arms above are
# conditional -- the consolidated suite pair on [ -f "$SNO" ] && [ -f "$REF" ], the loose
# siblings on a `shopt -s nullglob` loop -- so a corpus deletion empties BOTH silently and the
# bare `[ "$FAIL" -eq 0 ]` below then reported GREEN having measured nothing at all.
# ⛔ MEASURED, not theoretical: corpus fdbe8ff8 ("delete 226 confirmed-redundant source files")
# removed all five rung13_assertz witnesses; test_prolog_rung13.sh then exited rc=0 with
# PASS=0 FAIL=0 -- and it is the FIRST conjunct of the DONE-WHEN of rank-0 keystone row
# prolog-pz4-gamma-retain-activation-frames, so that row's acceptance criterion had a
# vacuously-true term for anyone who ran it. The other rung graders (rung16+) already refuse
# via their own suite-not-found guard; this family only checked [ -d "$CORPUS" ], which is the
# DIRECTORY, never a witness. RULES.md: a test that cannot measure REFUSES with rc=2 --
# never skip-as-success. The guard asks the question that actually matters (did anything get
# graded), not whether one particular file exists, because this family has TWO witness sources.
if [ $((PASS+FAIL)) -eq 0 ]; then
    echo "REFUSE (rc=2): $FAMILY graded ZERO witnesses -- looked for suite pair $SNO / $REF and loose $CORPUS/${FAMILY}_*.pl, found neither. Cannot measure, not a pass."
    exit 2
fi
[ "$FAIL" -eq 0 ]
