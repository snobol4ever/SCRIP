#!/usr/bin/env bash
# test_pascal_pat_suite.sh — THE ISO 7185 PASCAL DENOMINATOR (row pascal-iso-7185-validation-suite-pat-vendored-and-graded).
# Lon 2026-09-03 20:45: "100% means 100% of the industry standard language." For Pascal the standard is ISO 7185 and the
# public suite that grades it is the Pascal-P5 validation suite, vendored at corpus/packages/pascal/pat (see its README).
#
# TWO POPULATIONS, GRADED BY DIFFERENT QUESTIONS — that is the whole design, not a detail:
#   iso7185prt*  (427)  REJECTION tests. Each carries ONE deliberate violation. A conforming implementation must REFUSE it.
#                       PASS = scrip exits non-zero WITH a diagnostic.  FAIL = scrip accepts it.
#   iso7185pat*  (1)    ACCEPTANCE test. A large conforming program. Graded against `fpc -Miso` as the oracle.
#
# ⛔ THE .cmp/.ecp FILES ARE NOT THE ORACLE AND ARE NEVER DIFFED. They are Pascal-P5's own transcript and open with its
# banner ("P5 Pascal compiler vs. 1.4.x"). Diffing SCRIP against them would grade SCRIP on whether it is P5.
#
# ⛔⭐ A CRASH IS NOT A REJECTION, AND THIS IS THE ONE PLACE THAT DISTINCTION DECIDES THE BOARD. The obvious test for
# "did it reject?" is `rc != 0` — and under it a SIGSEGV scores as a CORRECT REJECTION, because a crashing compiler exits
# non-zero on every malformed program. That would turn the suite's whole point inside out: the worse the compiler behaves,
# the better it scores. Signals (rc >= 128) are counted as CRASH and are NEVER passes; a timeout (rc 124) likewise.
set -u
GATE_NAME=test_pascal_pat_suite
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SUITE="$ROOT/corpus/packages/pascal/pat"
SCRIP="$HERE/../scrip"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -d "$SUITE" ] || refuse "no vendored suite at $SUITE -- a suite that is absent is not a suite that is failing"
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first; a missing binary prints a full, plausible, entirely false all-FAIL board"
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_pascal_pat_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
declare -A P F C
for m in m3 m4; do P[$m]=0; F[$m]=0; C[$m]=0; done
TOTAL=0; NAMED=""
# ---- rejection population -------------------------------------------------------------------------------------------
for f in "$SUITE"/iso7185prt*.pas; do
    [ -e "$f" ] || continue
    b="$(basename "$f" .pas)"; TOTAL=$((TOTAL+1))
    for m in m3 m4; do
        # ⛔⭐ A TIMEOUT ON A REJECTION TEST IS "ACCEPTED", NOT "CRASHED" — and the 2s bound is a CONSEQUENCE of that,
        # not a guess. A program the front end refuses dies in ~10ms; one that reaches the timeout is one scrip
        # AGREED TO RUN, which is already the FAIL this test is looking for. Measured, not assumed: the first cut of
        # this runner used 8s and `rc>=124 => CRASH`, and the m3 arm then EXECUTED all 143 wrongly-accepted programs
        # for 8s each -- a >19-minute runner that also mislabelled its own headline defect as a crash.
        # ⛔⭐ RUN IN A SCRATCH CWD, NEVER IN THE WORKING TREE. A rejection test that scrip WRONGLY ACCEPTS is
        # then EXECUTED -- and these are ISO 7185 programs that do file I/O, so their side effects land wherever
        # the runner happens to stand. MEASURED: this left an empty file named `0` in SCRIP/ on every run, which
        # made the tree dirty, which made util_score_row.py correctly REFUSE to write the leaderboard row --
        # so the suite's own debris silently blocked the FACT RULE the suite exists to satisfy. ⭐ The general
        # form: a runner that executes its subjects in the repo root has made the repo part of the experiment.
        if [ "$m" = m3 ]; then ( cd "$TMP" && exec timeout 2s "$SCRIP" "$f" </dev/null >"$TMP/o" 2>&1 ); rc=$?
        else                   ( cd "$TMP" && exec timeout 8s "$SCRIP" --compile -o /dev/null "$f" </dev/null >"$TMP/o" 2>&1 ); rc=$?; fi
        # ⛔⭐ THE VERDICT IS STABLE; ONLY THE DIAGNOSIS VARIES (ceo ruling 2026-09-03, after their audit read
        # m3 296/129 where mine read 303/123 on the same tree). THE VERDICT is binary and load-independent:
        # a rejection test PASSES only when scrip REFUSES it with a diagnostic, and everything else -- ran to
        # completion, ran until the bound, crashed -- is the same FAIL, because none of them is a refusal.
        # The crash/timeout split is then a LABELLED DIAGNOSIS counted alongside, never inside, the verdict.
        # Before this, a program that SIGABRTs on an idle box hit the 2s bound under load ~20 instead, and
        # moved between the crash and fail columns run to run: two HQs reading the same tree got different
        # boards and had to reconcile numbers that never disagreed about anything that mattered.
        if [ "$rc" -ne 0 ] && [ "$rc" -lt 124 ] && [ -s "$TMP/o" ]; then P[$m]=$((P[$m]+1))
        else
            F[$m]=$((F[$m]+1))
            if   [ "$rc" -eq 124 ]; then C[$m]=$((C[$m]+1)); NAMED="$NAMED $b:$m:FAIL/diag=accepted-and-ran-to-bound"
            elif [ "$rc" -gt 124 ]; then C[$m]=$((C[$m]+1)); NAMED="$NAMED $b:$m:FAIL/diag=crash(rc=$rc)"
            elif [ "$rc" -ne 0 ];   then NAMED="$NAMED $b:$m:FAIL/diag=rejected-silently-no-diagnostic"
            else                         NAMED="$NAMED $b:$m:FAIL/diag=accepted"; fi
        fi
    done
done
# ---- acceptance population, oracle = fpc -Miso ----------------------------------------------------------------------
FPC="$(command -v fpc || true)"
for f in "$SUITE"/iso7185pat*.pas; do
    [ -e "$f" ] || continue
    b="$(basename "$f" .pas)"
    [ -n "$FPC" ] || { echo "note: fpc absent -- acceptance test $b not graded (its oracle is fpc -Miso); rejection population unaffected"; break; }
    in="$SUITE/$b.inp"; [ -f "$in" ] || in=/dev/null
    # ⛔⭐ THE ORACLE GETS A TIMEOUT TOO, AND THAT IS NOT DEFENSIVE PADDING -- MEASURED: `fpc -Miso` does not
    # finish on iso7185pat.pas (123KB) inside 60s. The first cut of this runner invoked fpc with NO bound, so
    # the whole suite hung on its ORACLE, not on scrip, and read as a >10-minute runner with no output. An
    # un-bounded oracle turns "the reference implementation cannot do this either" into "our runner is broken".
    rm -f "$TMP/oracle"
    ( cd "$TMP" && timeout 60 "$FPC" -Miso -o"$TMP/oracle" "$f" >"$TMP/oracle.err" 2>&1 ); orc=$?
    if [ ! -x "$TMP/oracle" ]; then
        # ⛔ rc=124 IS THE ONLY GENUINE TIMEOUT -- any other nonzero exit is the oracle REJECTING the
        # program (a real, fast compile error), a different fact with a different consequence, and the
        # two must not share one message (found via iso7185pat0001.pas: fpc -Miso rejects it in ~0.1s
        # with a genuine fatal error, not a 60s stall -- the old message called every non-build a timeout).
        if [ "$orc" -eq 124 ]; then
            echo "note: acceptance test $b is NOT COMPILED BY THE ORACLE (fpc -Miso) within 60s -- not graded, and explicitly NOT counted against scrip; a program the oracle cannot build grades nothing"
        else
            echo "note: acceptance test $b is REJECTED BY THE ORACLE (fpc -Miso rc=$orc, not a timeout) -- not graded, and explicitly NOT counted against scrip: $(tail -1 "$TMP/oracle.err")"
        fi
        continue
    fi
    timeout 20s "$TMP/oracle" <"$in" >"$TMP/want" 2>&1 || true
    TOTAL=$((TOTAL+1))
    for m in m3 m4; do
        if [ "$m" = m3 ]; then timeout 20s "$SCRIP" "$f" <"$in" >"$TMP/got" 2>&1; rc=$?
        else timeout 20s "$SCRIP" --compile -o "$TMP/b.s" "$f" </dev/null >/dev/null 2>&1 && \
             gcc -m64 -no-pie "$TMP/b.s" -o "$TMP/bin" -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -lm 2>/dev/null && \
             timeout 20s "$TMP/bin" <"$in" >"$TMP/got" 2>&1; rc=$?; fi
        if [ "$rc" -ge 124 ]; then C[$m]=$((C[$m]+1)); NAMED="$NAMED $b:$m:CRASH(rc=$rc)"
        elif cmp -s "$TMP/want" "$TMP/got"; then P[$m]=$((P[$m]+1))
        else F[$m]=$((F[$m]+1)); NAMED="$NAMED $b:$m:MISMATCH"; fi
    done
done
# ⛔ A RUNNER THAT GRADED NOTHING MUST NEVER PRINT THE SUCCESS SHAPE (the seven-point standard, point 3).
[ "$TOTAL" -gt 0 ] || refuse "graded ZERO programs over $SUITE -- refusing to print a board with no denominator"
echo "PAT_SUITE_BOARD total=$TOTAL m3_pass=${P[m3]} m3_fail=${F[m3]} m4_pass=${P[m4]} m4_fail=${F[m4]}"
echo "  diagnosis (counted INSIDE fail, never beside it): m3 crash-or-ran-to-bound=${C[m3]} · m4 crash-or-ran-to-bound=${C[m4]} — the verdict is REFUSED-WITH-A-DIAGNOSTIC or not, which does not vary with machine load; this split does"
[ -n "${PAT_NAME_REDS:-}" ] && { echo "  reds:"; for x in $NAMED; do echo "    $x"; done | head -40; }
if . "$HERE/lib_gate.sh" 2>/dev/null && command -v gate_stamp >/dev/null 2>&1; then gate_stamp; fi
python3 "$HERE/util_score_row.py" write --lang pascal --column vendor --suite PAT --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" \
    --text "ISO 7185 validation suite (Pascal-P5 1.4.x, vendored corpus/packages/pascal/pat): $TOTAL programs — m3 ${P[m3]}/$TOTAL · m4 ${P[m4]}/$TOTAL (${C[m3]}/${C[m4]} crash). 427 are REJECTION tests graded on whether scrip refuses them, per \`test_pascal_pat_suite.sh\`" \
    2>&1 | sed 's/^/    /'
python3 "$HERE/util_score_row.py" progress 2>/dev/null || true
exit 0
