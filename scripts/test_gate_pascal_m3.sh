#!/bin/bash
# test_gate_pascal_m3.sh — Pascal M3 (--run) gate
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/pascal}"
HARNESS="${HARNESS:-$(dirname "${BASH_SOURCE[0]}")/corpus_suite_harness.py}"
SUITES="${SUITES:-$CORPUS/crosscheck}"
# ⛔ HAND-MAINTAINED, matching test_corpus_snobol4.sh's PRE-autodiscovery shape (deliberate: the
# corpus-suite-family-list-should-autodiscover gate does not currently pass in this tree -- verified
# live, 2026-08-27 -- so a family here that isn't ALSO named below is silently ungraded, not FAIL).
# Register a new family in BOTH this file and test_gate_pascal_m4.sh's identical list, then
# grep -c the name back out of both files to confirm the edit actually landed.
SUITE_FAMILIES="aa arr2d arrrec case char chararr goto misc nestpv pb ptr rec recparam set stdlib vrec with"
RESULTS="${RESULTS:-/tmp/m3_results.tsv}"
PASS=0; FAIL=0; NOREF=0; XFAIL=0; EXAMINED=0
echo -e "probe\tclass\tnotes" > "$RESULTS"
shopt -s nullglob
for pas in "$CORPUS"/*.pas; do
    EXAMINED=$((EXAMINED+1))
    name=$(basename "$pas" .pas)
    ref="$CORPUS/$name.ref"
    [[ "$name" == "pcom" || "$name" == "pint" ]] && continue
    if [ ! -f "$ref" ]; then
        echo -e "$name\tNOREF\t" >> "$RESULTS"
        NOREF=$((NOREF+1)); continue
    fi
    inp="$CORPUS/$name.in"; [ -f "$inp" ] || inp=/dev/null
    out=$(timeout 8s "$SCRIP" --run "$pas" < "$inp" 2>/dev/null)
    rc=$?
    exp=$(cat "$ref")
    if [ $rc -eq 124 ]; then
        echo -e "$name\tTIMEOUT\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    elif [ -z "$out" ] && [ $rc -ne 0 ]; then
        echo -e "$name\tEMPTY_rc$rc\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    elif [ "$out" = "$exp" ]; then
        echo -e "$name\tPASS\t" >> "$RESULTS"
        PASS=$((PASS+1))
    else
        echo -e "$name\tWRONG_rc$rc\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    fi
done

SUITE_PASS=0; SUITE_FAIL=0; SUITE_EXAMINED=0
for fam in $SUITE_FAMILIES; do
    src="$SUITES/$fam.pas"; ref="$SUITES/$fam.ref"
    if [ ! -f "$src" ] || [ ! -f "$ref" ]; then
        echo -e "suite:$fam\tMISSING\t" >> "$RESULTS"
        FAIL=$((FAIL+1)); continue
    fi
    SUITE_EXAMINED=$((SUITE_EXAMINED+1))
    board=$(timeout 120s python3 "$HARNESS" run "$src" "$ref" --lang pascal --modes m3 2>/dev/null)
    p=$(grep -oP '(?<=m3_pass=)\d+' <<<"$board"); f=$(grep -oP '(?<=m3_fail=)\d+' <<<"$board")
    crash=$(grep -oP '(?<=m3_crash=)\d+' <<<"$board"); hang=$(grep -oP '(?<=m3_hang=)\d+' <<<"$board")
    unproven=$(grep -oP '(?<=m3_unproven=)\d+' <<<"$board")
    if [ -z "$p" ]; then
        echo -e "suite:$fam\tHARNESS_UNPROVEN\t" >> "$RESULTS"
        FAIL=$((FAIL+1)); continue
    fi
    bad=$((f + crash + hang + unproven))
    echo -e "suite:$fam\tPASS=$p FAIL=$f CRASH=$crash HANG=$hang UNPROVEN=$unproven\t" >> "$RESULTS"
    PASS=$((PASS+p)); FAIL=$((FAIL+bad))
    SUITE_PASS=$((SUITE_PASS+p)); SUITE_FAIL=$((SUITE_FAIL+bad))
done

echo "M3: PASS=$PASS FAIL=$FAIL NOREF=$NOREF XFAIL=$XFAIL (suites: $SUITE_EXAMINED families, $SUITE_PASS pass / $SUITE_FAIL fail)"
if [ $EXAMINED -eq 0 ] && [ $SUITE_EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 .pas files and 0 suite families examined under $CORPUS -- corpus-path typo or unpopulated clone, not a clean pass" >&2
    exit 2
fi
[ $FAIL -eq 0 ]
