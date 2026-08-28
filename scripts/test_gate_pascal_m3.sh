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

# --- INTERIM WITNESS-PAIR CHECK over corpus/benchmarks/pascal (hq_C 2026-08-27, ceo-endorsed) -------------------
# WHY: benchmarks/pascal carries 9 .pas/.ref witness pairs that NO gate reached, so `pascal-uplevel-nested-proc-hang`
# could close DONE+landed and silently un-land with nothing able to say NO. Interim: retires when
# `pascal-refs-regen-from-fpc-oracle` (rank 0) puts this directory on the graded board.
# ⛔ Discovered BY A FILE IT MUST CONTAIN, never `-d` on the container -- the s274 lesson: the container survives a
# re-grid while the contents re-nest, so a `-d` guard passes over an empty/moved corpus and reads as a clean pass.
# ⛔ m3 ONLY, deliberately: 5 of these 9 SIGSEGV in m4 under the already-tracked `pascal-m4-registered-dispatch-segv`
# / `pascal-m4-intermittent-segv-pb30-sieve` rows. Wiring m4 here would re-report those rows as this gate's failure.
# ⛔ 7 of the 9 open with `readln(reps)`; feeding /dev/null yields reps=0, an empty loop and a PLAUSIBLE all-zero
# board that is pure instrument error (measured, hq_C 2026-08-27). The `1` below is load-bearing -- do not remove it.
WCORPUS="${WCORPUS:-$S4E/corpus/benchmarks/pascal}"
WITNESS_XFAIL="${WITNESS_XFAIL:-fbench}"   # quick CURED (seat08, 71175348, void-procedure return-value fix) -- removed 2026-08-28. fbench: original LOWER-time SIGSEGV (lower_assign_var name=NULL on a curried multi-dim array write, e.g. testcase[i,sp] := v -- NOT self-assignment, that hypothesis was wrong) FIXED (seat02, row pascal-fbench-nested-function-self-assign-null-name). Still XFAIL: fixing that unmasked a SEPARATE, already-tracked, still-open defect one step further in -- transitXsurface/traceXline's nested if/elseif-inside-for-loop shape SIGSEGVs, matching pascal-m4-for-spine-leak-64b-per-iter's own "second spine leak" (zd_plan misses IR_BINOP_TEST merge points) exactly. Blocked on that row, not this one.
W_PASS=0; W_FAIL=0; W_EXAMINED=0; W_STALE=0
if [ -f "$WCORPUS/uplevel2.pas" ]; then
    for wpas in "$WCORPUS"/*.pas; do
        wname=$(basename "$wpas" .pas); wref="$WCORPUS/$wname.ref"
        [ -f "$wref" ] || continue
        W_EXAMINED=$((W_EXAMINED+1))
        wout=$(printf '1\n' | timeout 120s "$SCRIP" --run "$wpas" 2>/dev/null); wrc=$?
        if [ $wrc -eq 0 ] && [ "$wout" = "$(cat "$wref")" ]; then wok=1; else wok=0; fi
        case " $WITNESS_XFAIL " in
            *" $wname "*)
                # ⛔ An exception list that only permits FAILING lets a cure rot unnoticed. A listed witness that
                # PASSES is a RED here: the entry is stale and must be deleted in the same commit as the cure.
                if [ $wok -eq 1 ]; then
                    echo -e "witness:$wname\tXFAIL_STALE\t" >> "$RESULTS"
                    W_STALE=$((W_STALE+1)); FAIL=$((FAIL+1))
                else
                    echo -e "witness:$wname\tXFAIL\t" >> "$RESULTS"
                    XFAIL=$((XFAIL+1))
                fi ;;
            *)
                if [ $wok -eq 1 ]; then
                    echo -e "witness:$wname\tPASS\t" >> "$RESULTS"
                    W_PASS=$((W_PASS+1)); PASS=$((PASS+1))
                else
                    echo -e "witness:$wname\tWRONG_rc$wrc\t" >> "$RESULTS"
                    W_FAIL=$((W_FAIL+1)); FAIL=$((FAIL+1))
                fi ;;
        esac
    done
fi

echo "M3: PASS=$PASS FAIL=$FAIL NOREF=$NOREF XFAIL=$XFAIL (suites: $SUITE_EXAMINED families, $SUITE_PASS pass / $SUITE_FAIL fail)"
echo "M3 witnesses (benchmarks/pascal): EXAMINED=$W_EXAMINED PASS=$W_PASS FAIL=$W_FAIL XFAIL_STALE=$W_STALE"
if [ $EXAMINED -eq 0 ] && [ $SUITE_EXAMINED -eq 0 ] && [ $W_EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 .pas files, 0 suite families and 0 benchmark witnesses examined under $CORPUS / $WCORPUS -- corpus-path typo or unpopulated clone, not a clean pass" >&2
    exit 2
fi
[ $FAIL -eq 0 ]
