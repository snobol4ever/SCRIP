#!/bin/bash
# test_gate_pascal_m3.sh — Pascal M3 (--run) gate
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/pascal}"
HARNESS="${HARNESS:-$(dirname "${BASH_SOURCE[0]}")/corpus_suite_harness.py}"
MASTER_SRC="${MASTER_SRC:-$CORPUS/ALL.pas}"
MASTER_REF="${MASTER_REF:-$CORPUS/ALL.ref}"
# ⭐ REPOINTED (seat04, 2026-08-30, row pascal-master-flatten-and-scrip-test-pas): the old dual mechanism
# (a loose-*.pas loop + a hand-maintained SUITE_FAMILIES list over crosscheck/) is retired now that
# util_build_master_suite.py --lang pascal absorbs both shapes into ONE flat ALL.pas/ALL.ref, matching
# test_corpus_snobol4.sh's own cutover. `pcom`/`pint` are gone (confirmed absent, the old skip is dropped).
# ⛔ REVERTED BACK TO THE FLAT PATH (seat04, 2026-08-30) after seat11 (`94c98593`, row pascal-restore-prezeta,
# genuinely well-intentioned) repointed this at `$CORPUS/master/ALL.pas`, believing the flat path "has never
# existed" -- it existed on origin/main a full 30 minutes before that commit (corpus `066a680bb`, 20:16:32,
# vs. `94c98593` at 20:43:53); seat11's own `master/` subdir had already been deleted in that same commit as
# superseded, so their fix pointed the gate at a path that was freshly GONE, not one that had never arrived.
# Read as a stale/unpulled local corpus checkout at the time they investigated, not a real conflict.
# ⭐ WHAT SEAT11 FOUND THAT STAYS FIXED, independent of the path mixup: this gate's own all-arms-zero refusal
# (below) could not have caught the master silently grading 0 entries, because the STDIN_FAMILIES loop (and
# on m3 the benchmark-witness section) always examines >0 -- exactly the kind of "measured fine" vs. "could
# not measure" collision RULES.md's own gate-witness law exists to forbid. MASTER_EXAMINED now refuses
# independently, not just as part of the combined check.
# ⛔ FIVE entries stay loose PERMANENTLY, not a residue of this repoint: read1-4 and pb35 read real stdin,
# and the suite format has no stdin-input concept (hq_C's SNOBOL4-side ruling, 2026-08-24 — see KEEP.md
# section 1). The master builder independently reaches the same conclusion (ALL.excluded.txt names all 5).
# Register a new permanent stdin exception in BOTH this file and test_gate_pascal_m4.sh's identical list.
STDIN_FAMILIES="read1 read2 read3 read4 pb35"
RESULTS="${RESULTS:-/tmp/m3_results.tsv}"
PASS=0; FAIL=0; NOREF=0; XFAIL=0; EXAMINED=0
echo -e "probe\tclass\tnotes" > "$RESULTS"

for name in $STDIN_FAMILIES; do
    pas="$CORPUS/$name.pas"; ref="$CORPUS/$name.ref"
    EXAMINED=$((EXAMINED+1))
    if [ ! -f "$pas" ] || [ ! -f "$ref" ]; then
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

MASTER_PASS=0; MASTER_FAIL=0; MASTER_EXAMINED=0
if [ -f "$MASTER_SRC" ] && [ -f "$MASTER_REF" ]; then
    # ⭐ --by-modes-column (seat11, row pascal-master-eleven-reds-cured, 2026-09-04): ALL.csv marks the 5
    # `parser__*` entries `modes=ast` (graded by --dump-ast diff, never executed) — invoking plain `--modes m3`
    # silently ran them as m3 anyway and misreported all 5 as output-mismatch FAILs. Same bug class already hit
    # Raku once (SCORE.md's raku row: a modes-blind invocation collapsed run-graded entries into the ast bucket).
    board=$(timeout 120s python3 "$HARNESS" run "$MASTER_SRC" "$MASTER_REF" --lang pascal --by-modes-column --modes m3 2>/dev/null)
    p=$(grep -oP '(?<=m3_pass=)\d+' <<<"$board"); f=$(grep -oP '(?<=m3_fail=)\d+' <<<"$board")
    crash=$(grep -oP '(?<=m3_crash=)\d+' <<<"$board"); hang=$(grep -oP '(?<=m3_hang=)\d+' <<<"$board")
    unproven=$(grep -oP '(?<=m3_unproven=)\d+' <<<"$board")
    total=$(grep '^SUITE_BOARD ' <<<"$board" | grep -oP '(?<=total=)\d+')
    ast_p=$(grep -oP '(?<=ast_pass=)\d+' <<<"$board"); ast_f=$(grep -oP '(?<=ast_fail=)\d+' <<<"$board")
    ast_crash=$(grep -oP '(?<=ast_crash=)\d+' <<<"$board"); ast_hang=$(grep -oP '(?<=ast_hang=)\d+' <<<"$board")
    ast_unproven=$(grep -oP '(?<=ast_unproven=)\d+' <<<"$board")
    ast_total=$(grep '^SUITE_BOARD_AST ' <<<"$board" | grep -oP '(?<=total=)\d+')
    if [ -z "$p" ]; then
        echo -e "master:ALL\tHARNESS_UNPROVEN\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    else
        ast_bad=$(( ${ast_f:-0} + ${ast_crash:-0} + ${ast_hang:-0} + ${ast_unproven:-0} ))
        MASTER_EXAMINED=$(( ${total:-0} + ${ast_total:-0} ))
        bad=$((f + crash + hang + unproven + ast_bad))
        echo -e "master:ALL\tPASS=$p FAIL=$f CRASH=$crash HANG=$hang UNPROVEN=$unproven AST_PASS=${ast_p:-0} AST_FAIL=${ast_f:-0}\t" >> "$RESULTS"
        PASS=$((PASS+p+${ast_p:-0})); FAIL=$((FAIL+bad))
        MASTER_PASS=$((p+${ast_p:-0})); MASTER_FAIL=$bad
    fi
else
    echo -e "master:ALL\tMISSING\t" >> "$RESULTS"
    FAIL=$((FAIL+1))
fi

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

echo "M3: PASS=$PASS FAIL=$FAIL NOREF=$NOREF XFAIL=$XFAIL (master: $MASTER_EXAMINED entries, $MASTER_PASS pass / $MASTER_FAIL fail; stdin-loose: $EXAMINED examined)"
echo "M3 witnesses (benchmarks/pascal): EXAMINED=$W_EXAMINED PASS=$W_PASS FAIL=$W_FAIL XFAIL_STALE=$W_STALE"
# ⛔ MASTER_EXAMINED refuses ON ITS OWN, not only when every arm is zero (seat11's finding): the stdin-loose
# loop and the benchmark witnesses both always examine >0, so a master-path defect used to hide completely
# behind them -- ~150 entries silently dropped from the board with a clean-looking exit code either way.
if [ $MASTER_EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 master entries examined under $MASTER_SRC / $MASTER_REF -- path defect or unpopulated master, not a clean pass (this arm cannot hide behind the other two)" >&2
    exit 2
fi
if [ $EXAMINED -eq 0 ] && [ $W_EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 stdin-loose files and 0 benchmark witnesses examined under $CORPUS / $WCORPUS -- corpus-path typo or unpopulated clone, not a clean pass" >&2
    exit 2
fi
[ $FAIL -eq 0 ]
