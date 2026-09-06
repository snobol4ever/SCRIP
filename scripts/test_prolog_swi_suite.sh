#!/usr/bin/env bash
# test_prolog_swi_suite.sh — run SWI plunit conformance suite under --run and --compile
# Iterates corpus/tests/prolog/swi_tests/test_*.pl, loads each with
# plunit.pl shim + a main wrapper, compares PASS/FAIL per suite against .ref.
#
# Matching: set-based (order-independent, deduped — ignores double-run artefacts).
# Default (no --mode given): runs BOTH modes in one invocation, each printing its
# own "Suite totals: ... mode=X" line; overall exit 0 iff both modes clear their
# 80% coverage gate. Pass --mode to grade a single mode instead (one totals line).
# Options:
#   --verbose       show raw scrip output for failing files
#   --file NAME     run only NAME.pl  (e.g. --file test_bips)
#   --mode MODE     --run | --compile  (default: run both, one line each)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_flag_gate.sh unloadable"; exit 2; }
SCRIP="${HERE}/../scrip"
RT="${HERE}/../out"
CORPUS=$S4E/corpus/tests/prolog
SWIT=$S4E/corpus/packages/prolog/swi_tests
PLUNIT=$CORPUS/plunit.pl
MATCH_PY="${HERE}/util_swi_match.py"
REPORT_PY="${HERE}/util_swi_report.py"
WRAP=$(mktemp /tmp/pl_wrap_XXXXXX.pl)
ACTUAL_TMP=$(mktemp /tmp/pl_actual_XXXXXX.txt)
ASM_TMP=$(mktemp /tmp/pl_asm_XXXXXX.s)
PROG_TMP=$(mktemp /tmp/pl_prog_XXXXXX)
trap 'rm -f "$WRAP" "$ACTUAL_TMP" "$ASM_TMP" "$PROG_TMP"' EXIT

VERBOSE=0; ONLY_FILE=""; MODE=""; RECUT=0
while [[ $# -gt 0 ]]; do
    case "$1" in
        --verbose)  VERBOSE=1; shift ;;
        --file)     ONLY_FILE="$2"; shift 2 ;;
        --mode)     MODE="$2"; shift 2 ;;
        --run|--compile) MODE="$1"; shift ;;
        --census-packages) RECUT=1; shift ;;
        *) flaggate_reject "$1" "--verbose --file --mode --run --compile --census-packages" ;;
    esac
done

[ -d "$SWIT" ]   || { echo "⛔ REFUSED-TO-GRADE: $SWIT missing"; exit 2; }

# ⛔⭐⭐ --census-packages: CENSUS THE SHIPPED PLUNIT POPULATION AGAINST REAL swipl. IT WRITES NO REFS.
# ⛔⭐ THE REF CUTTER IS scripts/util_swi_cut_refs.sh (seat12, SCRIP b08893799) AND THIS ARM IS NOT A SECOND
# ONE. That script landed while this arm was being written, and it is BETTER at cutting on three counts, each
# measured by reading it: it runs ONE swipl PROCESS PER (file,unit) so a SIGABRT cannot swallow a sibling
# unit's verdict (this arm loses a whole file to one crash); it reads each verdict from plunit's OWN
# test_summary/2 bookkeeping with the same branching plunit's report/0 uses, rather than inferring it from
# run_tests/1 succeeding, which cannot tell a failed test from an error; and it takes its oracle from
# lib_oracle_flags.sh swipl_bin() instead of hardcoding a path, which is the shared-authority rule this arm
# had broken. TWO WRITERS FOR ONE ARTIFACT IS THE COLLISION CLASS (PROTOCOL 4c), so the writer here is
# DELETED rather than kept beside it -- when the two disagree, a later seat has no way to know which cut a
# ref, and the worse tool wins whenever it is the one that ran.
# ⭐ WHAT THIS ARM UNIQUELY ADDS, and why it survives at all: SCOPE. util_swi_cut_refs.sh defaults to the
# bases that ALREADY HAVE a .ref -- the 9 -- and addresses them by basename, so it cannot see the suites in
# subdirectories. This arm walks the SHIPPED population by PATH (170 plunit suites, not 9) and prints the
# inventory THE PACKAGE LOCKDOWN requires: every shipped program graded, or NAMED UNGRADABLE with the
# oracle's own reason. It is a census, and a census only.
#
# THE FINDING THE CENSUS EXISTS TO SERVE:
# ceo ruling 2026-09-06 ("the oracle is swipl, never our shim"), on hq_C's finding that every .ref in this
# suite was cut by running swipl THROUGH corpus/tests/prolog/plunit.pl -- which swipl cannot load, because it
# refuses to redefine its own set_prolog_flag/2 and current_prolog_flag/2. So the refs recorded the ORACLE
# ERRORING OUT: 51 of 57 lines read EMPTY ("no tests ran"), and three files demanded FAIL for units real swipl
# PASSES. A suite pinned at zero cannot tell you its oracle is broken -- only a passing case can contradict a
# bad ref, and a suite that never passes never produces one.
# ⛔ THIS ARM DELIBERATELY REQUIRES NO scrip AND NO libscrip_rt.so, and is placed ABOVE their preflights: a ref
# is the oracle's answer, and anything of ours in the loop is the defect this arm exists to undo. It never
# reads SCRIP output and never loads $PLUNIT.
# ⛔ The driver goal is INTROSPECTION ONLY -- plunit:current_unit/4, plunit:current_test/5, run_tests/1. It
# defines nothing and redefines nothing, which is the whole difference from the shim. Atoms are built with
# upcase_atom/2 rather than quoted, so the goal survives shell embedding without a single quote in it.
# ⭐ Refs are written BESIDE their .pl (by path), not by basename into $SWIT -- which is also what the by-path
# ruling on row prolog-swi-tests-the-two-copies-of-test-string-... asks for. The GRADING side still resolves by
# basename (see run_one_mode); that is that row's business, not this arm's.
census_packages() {
    local SW=/usr/bin/swipl
    [ -x "$SW" ] || { echo "⛔ REFUSED-TO-GRADE: no swipl at $SW -- a ref can only be cut by the oracle, and there is no oracle here"; return 2; }
    local G outdir f base dir units rc n
    # ⛔⭐⭐ THIS GOAL EMITS NO VERDICT, AND THAT IS A CORRECTION, NOT A LIMITATION. An earlier cut of this arm
    # decided EMPTY by counting DECLARED tests (plunit:current_test/5) and then read run_tests/1 SUCCEEDING as
    # PASS. That is wrong, and it was caught only because seat12's independent cutter disagreed on exactly one
    # file of nine: test_bips unit bips_occurs_check_error DECLARES 7 tests, every one of them carrying
    # condition(error_unification), which needs occurs_check==error -- and this swipl has occurs_check=false.
    # So 7 are declared, ZERO execute, plunit itself prints "No tests to run", and the honest verdict is EMPTY.
    # This arm said PASS. ⭐ DECLARATION IS NOT EXECUTION: the count was a proxy for the subject, it is right
    # nearly everywhere, and being right nearly everywhere is precisely what kept it hidden -- 7 of the 9 files
    # AGREED with the correct cutter. A disagreement of one is what exposed it.
    # ⛔ So verdicts are util_swi_cut_refs.sh's alone (it reads plunit's OWN test_summary/2 bookkeeping, which
    # counts what RAN). This goal reports STRUCTURE only -- the unit names a file declares -- plus, by the
    # process's own exit status, whether the oracle can get through the file at all.
    G='getenv(recut_out, P), open(P, write, S), forall(plunit:current_unit(U,_,_,_), ( aggregate_all(count, plunit:current_test(U,_,_,_,_), N), ( catch(with_output_to(string(_), run_tests(U)), _, true) -> true ; true ), format(S, "~w declared=~w~n", [U, N]) )), close(S), halt(0)'
    outdir=$(mktemp -d /tmp/pl_recut_XXXXXX)
    local shipped=0 graded=0 ungradable=0 lines=0 changed=0
    echo "CENSUS: oracle=$SW (its own library(plunit), no shim). This arm WRITES NOTHING -- scripts/util_swi_cut_refs.sh is the ref cutter."
    for f in $(find "$SWIT" -name '*.pl' | sort); do
        base=$(basename "$f" .pl); dir=$(dirname "$f")
        [ -z "$ONLY_FILE" ] || [ "$base" = "$ONLY_FILE" ] || continue
        # A file that declares no plunit unit is not a suite. Under THE PACKAGE LOCKDOWN it is NAMED here, never
        # silently dropped -- but it is not part of this arm's population either, so it is not counted ungradable.
        grep -q 'begin_tests(' "$f" || continue
        shipped=$((shipped + 1))
        # ⛔⭐ THE PREVIEW MIRRORS THE SOURCE PATH, NEVER A FLAT BASENAME. Measured the hard way, in this very
        # arm: a first cut wrote "$outdir/$base.ref" and same-named files in different subdirectories SILENTLY
        # OVERWROTE each other -- the inventory counted 165 graded over 429 lines while only 161 files and 373
        # lines survived on disk. That is the SAME by-basename defect this suite is already rowed for (the two
        # copies of test_string graded against one ref), reproduced inside the tool written to cure it. A count
        # taken from a loop counter and a count taken from the artifacts must agree, or the loop is lying.
        rel="${f#"$SWIT"/}"; recut_out="$outdir/${rel%.pl}.ref"; mkdir -p "$(dirname "$recut_out")"
        export recut_out; : > "$recut_out"
        set +e
        timeout 120 "$SW" -q -g "$G" -t 'halt(3)' "$f" </dev/null >"${recut_out%.ref}.out" 2>"${recut_out%.ref}.err"
        rc=$?
        set -e
        if [ "$rc" -ne 0 ] || [ ! -s "$recut_out" ]; then
            # ⛔ NAMED UNGRADABLE WITH THE ORACLE'S REASON -- never dropped, never counted as passed.
            ungradable=$((ungradable + 1))
            # ⛔ COUNT THE PARTIAL OUTPUT A CRASH LEFT BEHIND. Skipping it here made the loop's line total
            # smaller than the artifacts on disk, and the cross-check below then convicted an OVERWRITE that
            # had not happened -- a guard firing truthfully for the wrong stated reason.
            lines=$((lines + $(wc -l < "$recut_out")))
            printf '  UNGRADABLE %-40s oracle rc=%s: %s\n' "$rel" "$rc" "$(head -1 "${recut_out%.ref}.err" 2>/dev/null | tr -d '\n' | cut -c1-90)"
            continue
        fi
        graded=$((graded + 1)); n=$(wc -l < "$recut_out"); lines=$((lines + n))
        if [ -f "$dir/$base.ref" ]; then
            printf '  HAS-REF    %-40s %s unit(s) declared, ref carries %s line(s)\n' "$rel" "$n" "$(wc -l < "$dir/$base.ref")"
        else
            changed=$((changed + 1))
            printf '  NO-REF     %-40s %s unit(s) declared -- SHIPPED AND UNGRADED\n' "$rel" "$n"
        fi
    done
    # ⛔⭐ CROSS-CHECK THE LOOP COUNTER AGAINST THE ARTIFACTS ON DISK, and REFUSE if they disagree. This is the
    # guard that caught the flat-basename overwrite above: the loop said 165/429, the disk said 161/373, and
    # nothing in the run was red. A census that counts its own iterations cannot see what it destroyed.
    local disk_files disk_lines
    disk_files=$(find "$outdir" -name '*.ref' | wc -l)
    disk_lines=$(find "$outdir" -name '*.ref' -exec cat {} + 2>/dev/null | wc -l)
    echo "CENSUS inventory: shipped=$shipped loadable=$graded ungradable=$ungradable units-declared=$lines   (⛔ NO VERDICTS HERE -- util_swi_cut_refs.sh cuts those)"
    if [ "$disk_files" -ne "$((graded + ungradable))" ] || [ "$disk_lines" -ne "$lines" ]; then
        echo "⛔ REFUSED-TO-GRADE: the census loop counted graded=$graded ungradable=$ungradable over $lines line(s), but $outdir holds $disk_files ref file(s) over $disk_lines line(s)."
        echo "   TWO CAUSES REACH THIS MESSAGE AND IT NAMES NEITHER AS CERTAIN: (a) a ref was overwritten by a same-named sibling, so the census destroyed evidence it never looked at; (b) a file the loop skipped still left PARTIAL output on disk, so the artifacts legitimately exceed the count. Compare $outdir against the loop before concluding which."
        echo "   Refusing either way: a number the artifacts do not support is not a measurement."
        return 2
    fi
    echo "CENSUS preview dir (not the corpus): $outdir"
    echo "CENSUS: no refs written and no verdicts computed, by design. changed=$changed of the loadable files ship with NO .ref at all. To CUT refs use scripts/util_swi_cut_refs.sh --write (the one cutter: per-unit isolated, verdicts from plunit test_summary/2)."
    # ⭐ ungradable>0 is REPORTED, not a red: an oracle that aborts on a file is a fact about the package, and
    # this arm's job is to state it, not to grade it. The lockdown row's own criterion is where ungraded=0 binds.
    return 0
}
if [ "$RECUT" -eq 1 ]; then census_packages; exit $?; fi
[ -f "$PLUNIT" ] || { echo "⛔ REFUSED-TO-GRADE: $PLUNIT missing"; exit 2; }
[ -x "$SCRIP" ]  || { echo "⛔ REFUSED-TO-GRADE: scrip not built"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_prolog_swi_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSED-TO-GRADE: libscrip_rt.so not built"; exit 2; }

printf 'main :- run_tests.\n:- initialization(main).\n' > "$WRAP"

# run_one_mode: grades the whole SWIT corpus under one scrip mode flag ("--run"
# or "--compile"), prints its own "Suite totals:" line, returns 1 if that mode's
# coverage is <80% (never aborts the script itself — set -e is suspended for a
# function call under `||`, which is what lets the caller run a second mode after
# the first one's gate check fails).
run_one_mode() {
    local mode="$1"
    local PASS=0 FAIL=0 TOTAL=0

    for f in $(find "$SWIT" -name "*.pl" | sort); do   # RECURSIVE (row swi-tests-subdirs-invisible, Lon direct 2026-08-30): the flat test_*.pl glob saw 18 of 249 -- 21 subdirs + test.pl were structurally invisible
        base=$(basename "$f" .pl)
        ref="$SWIT/${base}.ref"
        [ -f "$ref" ] || continue
        [ -z "$ONLY_FILE" ] || [ "$base" = "$ONLY_FILE" ] || continue

        suite_total=$(wc -l < "$ref")
        TOTAL=$((TOTAL + suite_total))

        if [ "$mode" = "--compile" ]; then
            # m4 is TEXT-emit only: --compile dumps asm to stdout, it does not run
            # anything itself. Same pipeline as test_3way_snobol4.sh: emit .s, gcc
            # assembles+links against libscrip_rt.so, then the LINKED BINARY runs.
            : > "$ACTUAL_TMP"
            if timeout 30 "$SCRIP" --compile "$PLUNIT" "$f" "$WRAP" > "$ASM_TMP" 2>/dev/null \
               && [ -s "$ASM_TMP" ] \
               && gcc -no-pie "$ASM_TMP" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$PROG_TMP" 2>/dev/null; then
                timeout 30 "$PROG_TMP" < /dev/null 2>/dev/null \
                    | grep -E '^(PASS|FAIL|EMPTY) ' > "$ACTUAL_TMP" || true
            fi
        else
            timeout 30 "$SCRIP" "$mode" "$PLUNIT" "$f" "$WRAP" < /dev/null 2>/dev/null \
                | grep -E '^(PASS|FAIL|EMPTY) ' > "$ACTUAL_TMP" || true
        fi

        matched=$(python3 "$MATCH_PY" "$ref" "$ACTUAL_TMP")
        PASS=$((PASS + matched))
        FAIL=$((FAIL + suite_total - matched))

        if [ "$matched" -eq "$suite_total" ]; then
            echo "  PASS $base ($suite_total suite-lines)  [$mode]"
        else
            echo "  FAIL $base  match=$matched/$suite_total  [$mode]"
            python3 "$REPORT_PY" "$ref" "$ACTUAL_TMP"
            if [ "$VERBOSE" -eq 1 ]; then
                echo "  --- raw output ---"
                cat "$ACTUAL_TMP"
                echo "  ---"
            fi
        fi
    done

    echo ""
    echo "Suite totals: PASS=$PASS FAIL=$FAIL TOTAL=$TOTAL  mode=$mode"
    # ⭐ PASS/FAIL/TOTAL are `local` to this function, so they do not exist by the time the script ends.
    # Accumulate the board across modes into a global here rather than making the totals global, which
    # would let a later mode silently overwrite an earlier one and report the last mode as "the suite".
    SWI_BOARD="${SWI_BOARD:+$SWI_BOARD · }$mode $PASS/$TOTAL (FAIL=$FAIL)"
    [ "$TOTAL" -gt 0 ] || { echo "⛔ REFUSED-TO-GRADE: no test files found"; exit 2; }
    local pct=$((PASS * 100 / TOTAL))
    echo "Coverage: ${pct}%  (gate: >=80%)"
    [ "$pct" -ge 80 ]
}

OVERALL_RC=0
if [ -n "$MODE" ]; then
    run_one_mode "$MODE" || OVERALL_RC=1
else
    run_one_mode "--run" || OVERALL_RC=1
    echo ""
    run_one_mode "--compile" || OVERALL_RC=1
fi

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board.
# ⛔⭐⭐ A SINGLE-FILE RUN NEVER WRITES THE SUITE CELL (seat09, 2026-09-05, measured the hard way while
# diagnosing this suite: `--file X` unconditionally rewrote prolog/vendor with that ONE file's count as if it
# were the whole suite. They did it NINE TIMES while bisecting and had to re-run the full suite to restore the
# honest reading). The write is not merely premature here, it is IMPOSSIBLE to make correct: a one-file run
# cannot produce a suite-wide number by construction, so there is no text it could write that would be true.
# ⭐ Same shape as the refusals this repo already keeps: a run that CANNOT measure the thing must not publish a
# number for it. The measurement still prints on the terminal -- it is the LEADERBOARD write that is skipped,
# and the skip says so rather than passing silently, because a seat who expected a row and got none should not
# have to infer why from an absence.
if [ -n "${ONLY_FILE:-}" ]; then
    echo "SCORE.md NOT UPDATED (by design): --file ${ONLY_FILE} grades ONE file and cannot produce a suite-wide"
    echo "  number. The leaderboard cell keeps whatever the last FULL run measured. Re-run without --file to record."
else
    python3 "$HERE/util_score_row.py" write --lang prolog --column vendor --suite SWI \
        --measurer "${S4E_SEAT:-}" --text "${SWI_BOARD:-no mode ran} (\`test_prolog_swi_suite.sh\`)" \
        || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
fi
exit "$OVERALL_RC"
